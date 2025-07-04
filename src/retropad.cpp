#include "retropad.h"
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_init.h>
#include <qdebug.h>

RetroPad::RetroPad(QObject *parent)
    : QObject(parent)
    , m_keyboardStates()
    , m_controllerStates()
    , m_inputMappings()
{
    initializeSDL();
    m_sdlEventPollTimer = new QTimer(this);
    connect(m_sdlEventPollTimer, &QTimer::timeout, this, &RetroPad::pollSDLEvents);
    m_sdlEventPollTimer->start(16); // 60fps polling rate
}

RetroPad::~RetroPad() {
    cleanupSDL();
    m_keyboardStates.clear();
    m_controllerStates.clear();
    m_sdlEventPollTimer->stop();
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
        qDebug() << "Event type: " << event.type;
    }
}

int16_t RetroPad::getInputState(InputDevice input_device) {
    if( m_inputMappings.contains(input_device)) {
        InputMapping mapping = m_inputMappings[input_device];
        if (mapping.type == InputType::Keyboard) {
            return m_keyboardStates.value(static_cast<Qt::Key>(mapping.sdl_key_or_button), 0);
        } else if (mapping.type == InputType::Controller) {
            return m_controllerStates.value(static_cast<SDL_GamepadButton>(mapping.sdl_key_or_button), 0);
        }
    }
    return 0;
}
void RetroPad::setMapping(InputDevice input_device, InputMapping mapping) {
    m_inputMappings[input_device] = mapping;
}
void RetroPad::updateInputStates(Qt::Key key, bool pressed) {
    qDebug() << "from retropad Key pressed:" << key << "State:" << pressed;
    m_keyboardStates[key] = pressed ? 1 : 0;
}
