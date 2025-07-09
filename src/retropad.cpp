// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2025 Seshan Ravikumar <seshan@sineware.ca>

#include "retropad.h"
#include <KConfig>
#include <KConfigGroup>
#include <QKeySequence>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_init.h>

using namespace Qt::Literals::StringLiterals;

RetroPad::RetroPad(QObject *parent)
    : QObject(parent)
    , m_keyboardStates()
    , m_controllerStates()
    , m_inputMappings()
    , m_sdlEventThread(nullptr)
    , m_running(true)
{
    qRegisterMetaType<InputMapping>("InputMapping");
    qRegisterMetaType<InputDevice>("InputDevice");

    populateMappings();

    initializeSDL();
    m_sdlEventThread = QThread::create([this]() {
        qDebug() << "SDL Event Thread started";
        while (m_running) {
            pollSDLEvents();
            QThread::msleep(16); // 60fps polling rate
        }
    });
    m_sdlEventThread->setParent(this);
    m_sdlEventThread->start();
}

RetroPad::~RetroPad() {
    m_running = false;
    m_sdlEventThread->wait();

    cleanupSDL();
}

void RetroPad::populateMappings() {
    KConfig config(u"kretrorc"_s);
    KConfigGroup inputGroup(&config, u"Input"_s);

    auto isController = inputGroup.readEntry(u"Port_Type_0"_s, u"Keyboard"_s) == u"Controller"_s;
    auto insertMapping = [&](unsigned device_id, const QString &configKey, int defaultKey) {
        m_inputMappings.insert(
            InputDevice{0, device_id},
            InputMapping{
                isController ? InputType::Controller : InputType::Keyboard
                , inputGroup.readEntry(configKey, static_cast<int>(defaultKey))}
        );
    };
    insertMapping(RETRO_DEVICE_ID_JOYPAD_A,      u"A_0"_s,      Qt::Key::Key_Z);
    insertMapping(RETRO_DEVICE_ID_JOYPAD_B,      u"B_0"_s,      Qt::Key::Key_X);
    insertMapping(RETRO_DEVICE_ID_JOYPAD_X,      u"X_0"_s,      Qt::Key::Key_C);
    insertMapping(RETRO_DEVICE_ID_JOYPAD_Y,      u"Y_0"_s,      Qt::Key::Key_V);
    insertMapping(RETRO_DEVICE_ID_JOYPAD_L,      u"L_0"_s,      Qt::Key::Key_Q);
    insertMapping(RETRO_DEVICE_ID_JOYPAD_R,      u"R_0"_s,      Qt::Key::Key_W);
    insertMapping(RETRO_DEVICE_ID_JOYPAD_UP,     u"Up_0"_s,     Qt::Key::Key_Up);
    insertMapping(RETRO_DEVICE_ID_JOYPAD_DOWN,   u"Down_0"_s,   Qt::Key::Key_Down);
    insertMapping(RETRO_DEVICE_ID_JOYPAD_LEFT,   u"Left_0"_s,   Qt::Key::Key_Left);
    insertMapping(RETRO_DEVICE_ID_JOYPAD_RIGHT,  u"Right_0"_s,  Qt::Key::Key_Right);
    insertMapping(RETRO_DEVICE_ID_JOYPAD_START,  u"Start_0"_s,  Qt::Key::Key_A);
    insertMapping(RETRO_DEVICE_ID_JOYPAD_SELECT, u"Select_0"_s, Qt::Key::Key_S);

    for (const auto &key : m_inputMappings.keys()) {
        const InputMapping &mapping = m_inputMappings[key];
        qDebug() << "Port:" << key.port
                 << "Device ID:" << key.libretro_device_id
                 << "Type:" << mapping.type
                 << "Key/Button:" << mapping.key_or_button;
    }

    Q_EMIT currentMappingsChanged();
}

void RetroPad::resetMappings() {
    KConfig config(u"kretrorc"_s);
    KConfigGroup inputGroup(&config, u"Input"_s);

    inputGroup.deleteGroup();

    m_inputMappings.clear();
    populateMappings();
}

void RetroPad::initializeSDL() {
    if (!SDL_Init(SDL_INIT_GAMEPAD)) {
        qWarning() << "Failed to initialize SDL: " << SDL_GetError();
    }

    int numGamepads;
    SDL_JoystickID *gamepads = SDL_GetGamepads(&numGamepads);
    qDebug()  << "Available gamepads: " << numGamepads;

    for (int i = 0; i < numGamepads; ++i) {
        SDL_Gamepad *gamepad = SDL_OpenGamepad(gamepads[i]);
        if (gamepad) {
            qDebug() << "Gamepad" << i << "opened successfully.";
            m_gamepads.append(gamepad);
        } else {
            qWarning() << "Failed to open gamepad" << i << ": " << SDL_GetError();
        }
    }

    qDebug() << "SDL initialized successfully.";
}

void RetroPad::cleanupSDL() {
    SDL_Quit();
    for (SDL_Gamepad *gamepad : m_gamepads) {
        if (gamepad) {
            SDL_CloseGamepad(gamepad);
            qDebug() << "Gamepad closed.";
        }
    }
    m_gamepads.clear();
}

void RetroPad::pollSDLEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
                SDL_GamepadButton button = static_cast<SDL_GamepadButton>(event.gbutton.button);
                qDebug() << "Gamepad button down:" << button;
                QMutexLocker locker(&m_stateMutex);
                m_controllerStates[button] = 255;
                Q_EMIT inputStateChanged(InputMapping{InputType::Controller, static_cast<int>(button)});
                break;
            }
            case SDL_EVENT_GAMEPAD_BUTTON_UP: {
                SDL_GamepadButton button = static_cast<SDL_GamepadButton>(event.gbutton.button);
                qDebug() << "Gamepad button up:" << button;
                QMutexLocker locker(&m_stateMutex);
                m_controllerStates[button] = 0;
                Q_EMIT inputStateChanged(InputMapping{InputType::Controller, static_cast<int>(button)});
                break;
            }
        }
    }
}

int16_t RetroPad::getInputState(InputDevice input_device) {
    QMutexLocker locker(&m_stateMutex);
    if(m_inputMappings.contains(input_device)) {
        InputMapping mapping = m_inputMappings[input_device];
        if (mapping.type == InputType::Keyboard) {
            return m_keyboardStates.value(static_cast<Qt::Key>(mapping.key_or_button), 0);
        } else if (mapping.type == InputType::Controller) {
            return m_controllerStates.value(static_cast<SDL_GamepadButton>(mapping.key_or_button), 0);
        }
    }
    return 0;
}

void RetroPad::setMapping(InputDevice input_device, InputMapping mapping) {
    KConfig config(u"kretrorc"_s);
    KConfigGroup inputGroup(&config, u"Input"_s);
    
    // get KConfig key from m_buttonNames
    QString kconfigKey;
    for (const auto &button : m_buttonNames) {
        if (std::get<0>(button) == input_device.libretro_device_id) {
            kconfigKey = std::get<1>(button);
            break;
        }
    }
    if (kconfigKey.isEmpty()) {
        qWarning() << "No KConfig key found for device ID:" << input_device.libretro_device_id;
        return;
    }

    inputGroup.writeEntry(kconfigKey + QString::number(input_device.port), mapping.key_or_button);
    inputGroup.writeEntry(u"Port_Type_" + QString::number(input_device.port), 
                         mapping.type == InputType::Controller ? u"Controller"_s : u"Keyboard"_s);

    m_inputMappings[input_device] = mapping;

    Q_EMIT currentMappingsChanged();
}
void RetroPad::setMapping(unsigned port, unsigned libretro_device_id, InputType type, int key_or_button) {
    InputDevice device{port, libretro_device_id};
    InputMapping mapping{type, key_or_button};
    setMapping(device, mapping);
    qDebug() << "Set mapping from QML for device:" << device.port
             << "ID:" << device.libretro_device_id
             << "Type:" << static_cast<int>(mapping.type)
             << "Key/Button:" << mapping.key_or_button;
}

void RetroPad::updateInputStates(Qt::Key key, bool pressed) {
    QMutexLocker locker(&m_stateMutex);
    m_keyboardStates[key] = pressed ? 255 : 0;
    Q_EMIT inputStateChanged(InputMapping{InputType::Keyboard, static_cast<int>(key)});
}

QList<QVariantMap> RetroPad::buttonNames() const {
    QList<QVariantMap> buttonList;
    for (const auto &button : m_buttonNames) {
        QVariantMap buttonMap;
        buttonMap[u"id"_s] = std::get<0>(button);
        buttonMap[u"kconfig_key"_s] = std::get<1>(button);
        buttonMap[u"friendly_name"_s] = std::get<2>(button);
        buttonList.append(buttonMap);
    }
    return buttonList;
}
QVariantMap RetroPad::currentMappings() const {
    QVariantMap mappings;
    
    for (const auto &button : m_buttonNames) {
        unsigned id = std::get<0>(button);
        if(m_inputMappings.contains(InputDevice{0, id})) {
            const InputMapping &mapping = m_inputMappings[InputDevice{0, id}];
            QString mappingString;
            if (mapping.type == InputType::Keyboard) {
                mappingString = QKeySequence(mapping.key_or_button).toString();
            } else if (mapping.type == InputType::Controller) {
                mappingString = QString::fromStdString(SDL_GetGamepadStringForButton(
                    static_cast<SDL_GamepadButton>(mapping.key_or_button)
                ));
            }
            mappings[QString::number(id)] = mappingString;
        }
    }
    
    return mappings;
}