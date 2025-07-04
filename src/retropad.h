#pragma once 

#include <QObject>
#include <QQmlEngine>
#include <QHash>
#include <QTimer>
#include <SDL3/SDL.h>
#include <qdebug.h>
#include <qnamespace.h>

class RetroPad : public QObject {
    Q_OBJECT
    QML_ELEMENT

public:
    RetroPad(QObject *parent = nullptr);
    ~RetroPad();

    enum InputType { Keyboard, Controller };
    Q_ENUM(InputType);

    // Describes the target mapping for an input device
    struct InputMapping {
        InputType type;
        int16_t sdl_key_or_button;
    };
    // Describes a libretro "requested" input device (from the core polling)
    struct InputDevice {
        bool operator==(const InputDevice &other) const {
            return port == other.port && libretro_device_id == other.libretro_device_id;
        }
        unsigned port;
        unsigned libretro_device_id;
    };

    // Called by libretro to get input state
    Q_INVOKABLE int16_t getInputState(InputDevice input_device);

    // Called to set up input mappings
    Q_INVOKABLE void setMapping(InputDevice input_device, InputMapping mapping);

    // Keyboard input from qml
    Q_INVOKABLE void updateInputStates(Qt::Key key, bool pressed);

private Q_SLOTS:
    void pollSDLEvents();

private:
    // Maps SDL keys/buttons to their current state (pressed/released)
    QHash<Qt::Key, int16_t> m_keyboardStates;
    QHash<SDL_GamepadButton, int16_t> m_controllerStates;

    QList<SDL_Gamepad*> m_gamepads;

    // Maps libretro device IDs to SDL keys/buttons
    QHash<InputDevice, InputMapping> m_inputMappings;

    void initializeSDL();
    void cleanupSDL();

    QTimer *m_sdlEventPollTimer;
};

inline size_t qHash(const RetroPad::InputDevice& device, size_t seed = 0) {
    return qHashMulti(seed, device.port, device.libretro_device_id);
}