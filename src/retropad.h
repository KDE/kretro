// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2025 Seshan Ravikumar <seshan@sineware.ca>

#pragma once 

#include <QObject>
#include <QQmlEngine>
#include <QHash>
#include <QTimer>
#include <QMutex>
#include <QThread>
#include <SDL3/SDL.h>
#include "libretro.h"

using namespace Qt::Literals::StringLiterals;

namespace RetroPadTypes {
    enum InputType { Keyboard, Controller };
        
    // Describes the target mapping for an input device
    struct InputMapping {
        Q_GADGET
        Q_PROPERTY(InputType type MEMBER type)
        Q_PROPERTY(int key_or_button MEMBER key_or_button)
    public:
        InputType type;
        int key_or_button;
    };

    // Describes a libretro "requested" input device (from the core polling)
    struct InputDevice {
        Q_GADGET
        Q_PROPERTY(unsigned port MEMBER port)
        Q_PROPERTY(unsigned libretro_device_id MEMBER libretro_device_id)
    public:
        bool operator==(const InputDevice &other) const {
            return port == other.port && libretro_device_id == other.libretro_device_id;
        }
        unsigned port;
        unsigned libretro_device_id;
    };

    inline size_t qHash(const RetroPadTypes::InputDevice& device, size_t seed = 0) {
        return qHashMulti(seed, device.port, device.libretro_device_id);
    }
}

class RetroPad : public QObject {
    Q_OBJECT
    QML_ELEMENT
    
    Q_PROPERTY(QList<QVariantMap> buttonNames READ buttonNames CONSTANT)
    Q_PROPERTY(QVariantMap currentMappings READ currentMappings NOTIFY currentMappingsChanged CONSTANT)
public:
    using InputType = RetroPadTypes::InputType;
    using InputMapping = RetroPadTypes::InputMapping;
    using InputDevice = RetroPadTypes::InputDevice;

    explicit RetroPad(QObject *parent = nullptr);
    ~RetroPad() override;

    Q_ENUM(InputType);

    // Called by libretro to get input state
    Q_INVOKABLE int16_t getInputState(InputDevice input_device);

    // Called to set a input mapping and save it to KConfig
    Q_INVOKABLE void setMapping(InputDevice input_device, InputMapping mapping);
    Q_INVOKABLE void setMapping(unsigned port, unsigned libretro_device_id, InputType type, int key_or_button);
    Q_INVOKABLE void populateMappings();
    Q_INVOKABLE void resetMappings();

    // Keyboard input from qml
    Q_INVOKABLE void updateInputStates(Qt::Key key, bool pressed);

    QList<QVariantMap> buttonNames() const;
    QVariantMap currentMappings() const;

Q_SIGNALS:
    // Controller or keyboard input events
    void inputStateChanged(InputMapping mapping);
    void currentMappingsChanged();

private Q_SLOTS:
    void pollSDLEvents();

private:
    // Maps SDL keys/buttons to their current state (pressed/released)
    QHash<Qt::Key, int16_t> m_keyboardStates;
    QHash<SDL_GamepadButton, int16_t> m_controllerStates;
    QMutex m_stateMutex;

    QList<SDL_Gamepad*> m_gamepads;

    // Maps libretro device IDs to SDL keys/buttons
    QHash<InputDevice, InputMapping> m_inputMappings;

    void initializeSDL();
    void cleanupSDL();

    QThread *m_sdlEventThread;
    bool m_running;

    const QList<std::tuple<unsigned, QString, QString>> m_buttonNames = {
        //  DEVICE_ID                         KConfig key    Friendly name
        { RETRO_DEVICE_ID_JOYPAD_A,       u"A_"_s,      u"A"_s },
        { RETRO_DEVICE_ID_JOYPAD_B,       u"B_"_s,      u"B"_s },
        { RETRO_DEVICE_ID_JOYPAD_X,       u"X_"_s,      u"X"_s },
        { RETRO_DEVICE_ID_JOYPAD_Y,       u"Y_"_s,      u"Y"_s },
        { RETRO_DEVICE_ID_JOYPAD_L,       u"L_"_s,      u"L"_s },
        { RETRO_DEVICE_ID_JOYPAD_R,       u"R_"_s,      u"R"_s },
        { RETRO_DEVICE_ID_JOYPAD_UP,      u"Up_"_s,     u"Up"_s },
        { RETRO_DEVICE_ID_JOYPAD_DOWN,    u"Down_"_s,   u"Down"_s },
        { RETRO_DEVICE_ID_JOYPAD_LEFT,    u"Left_"_s,   u"Left"_s },
        { RETRO_DEVICE_ID_JOYPAD_RIGHT,   u"Right_"_s,  u"Right"_s },
        { RETRO_DEVICE_ID_JOYPAD_START,  u"Start_"_s,  u"Start"_s },
        { RETRO_DEVICE_ID_JOYPAD_SELECT, u"Select_"_s, u"Select"_s }
    };
};

Q_DECLARE_METATYPE(RetroPadTypes::InputMapping);
Q_DECLARE_METATYPE(RetroPadTypes::InputDevice);
