// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2025 Seshan Ravikumar <seshan@sineware.ca>

#include "retropad.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_init.h>
#include "libretro.h"

RetroPad::RetroPad(QObject *parent)
    : QObject(parent)
    , m_keyboardStates()
    , m_controllerStates()
    , m_inputMappings()
    , m_sdlEventThread(nullptr)
    , m_running(true)
{

    // todo kconfig and qml config for input mappings
    m_inputMappings.insert(InputDevice{0, RETRO_DEVICE_ID_JOYPAD_UP}, InputMapping{InputType::Controller, SDL_GAMEPAD_BUTTON_DPAD_UP});
    m_inputMappings.insert(InputDevice{0, RETRO_DEVICE_ID_JOYPAD_DOWN}, InputMapping{InputType::Controller, SDL_GAMEPAD_BUTTON_DPAD_DOWN});
    m_inputMappings.insert(InputDevice{0, RETRO_DEVICE_ID_JOYPAD_LEFT}, InputMapping{InputType::Controller, SDL_GAMEPAD_BUTTON_DPAD_LEFT});
    m_inputMappings.insert(InputDevice{0, RETRO_DEVICE_ID_JOYPAD_RIGHT}, InputMapping{InputType::Controller, SDL_GAMEPAD_BUTTON_DPAD_RIGHT});
    m_inputMappings.insert(InputDevice{0, RETRO_DEVICE_ID_JOYPAD_A}, InputMapping{InputType::Controller, SDL_GAMEPAD_BUTTON_LABEL_A});
    m_inputMappings.insert(InputDevice{0, RETRO_DEVICE_ID_JOYPAD_B}, InputMapping{InputType::Controller, SDL_GAMEPAD_BUTTON_LABEL_B});
    m_inputMappings.insert(InputDevice{0, RETRO_DEVICE_ID_JOYPAD_X}, InputMapping{InputType::Controller, SDL_GAMEPAD_BUTTON_LABEL_X});
    m_inputMappings.insert(InputDevice{0, RETRO_DEVICE_ID_JOYPAD_Y}, InputMapping{InputType::Controller, SDL_GAMEPAD_BUTTON_LABEL_Y});
    m_inputMappings.insert(InputDevice{0, RETRO_DEVICE_ID_JOYPAD_START}, InputMapping{InputType::Controller, SDL_GAMEPAD_BUTTON_START});
    m_inputMappings.insert(InputDevice{0, RETRO_DEVICE_ID_JOYPAD_SELECT}, InputMapping{InputType::Controller, SDL_GAMEPAD_BUTTON_BACK});
    m_inputMappings.insert(InputDevice{0, RETRO_DEVICE_ID_JOYPAD_L}, InputMapping{InputType::Controller, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER});
    m_inputMappings.insert(InputDevice{0, RETRO_DEVICE_ID_JOYPAD_R}, InputMapping{InputType::Controller, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER});   
        
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
                break;
            }
            case SDL_EVENT_GAMEPAD_BUTTON_UP: {
                SDL_GamepadButton button = static_cast<SDL_GamepadButton>(event.gbutton.button);
                qDebug() << "Gamepad button up:" << button;
                QMutexLocker locker(&m_stateMutex);
                m_controllerStates[button] = 0;
                break;
            }
        }
    }
}

int16_t RetroPad::getInputState(InputDevice input_device) {
    QMutexLocker locker(&m_stateMutex);
    if( m_inputMappings.contains(input_device)) {
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
    m_inputMappings[input_device] = mapping;
}

void RetroPad::updateInputStates(Qt::Key key, bool pressed) {
    qDebug() << "from retropad Key pressed:" << key << "State:" << pressed;
    m_keyboardStates[key] = pressed ? 255 : 0;
}
