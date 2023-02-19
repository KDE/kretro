// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan.r@sineware.ca>

#include "app.h"
#include <KSharedConfig>
#include <KWindowConfig>
#include <QQuickWindow>
#include "libretro.h"
#include <dlfcn.h>
#include <cstdarg>

App::App(QObject* parent)
    : QObject(parent)
    , m_retroFrame{nullptr}
    , m_frameTimer{new QTimer{this}}
{
}


void App::restoreWindowGeometry(QQuickWindow *window, const QString &group) const
{
    KConfig dataResource(QStringLiteral("data"), KConfig::SimpleConfig, QStandardPaths::AppDataLocation);
    KConfigGroup windowGroup(&dataResource, QStringLiteral("Window-") + group);
    KWindowConfig::restoreWindowSize(window, windowGroup);
    KWindowConfig::restoreWindowPosition(window, windowGroup);
}

void App::saveWindowGeometry(QQuickWindow *window, const QString &group) const
{
    KConfig dataResource(QStringLiteral("data"), KConfig::SimpleConfig, QStandardPaths::AppDataLocation);
    KConfigGroup windowGroup(&dataResource, QStringLiteral("Window-") + group);
    KWindowConfig::saveWindowPosition(window, windowGroup);
    KWindowConfig::saveWindowSize(window, windowGroup);
    dataResource.sync();
}

void retrolog(enum retro_log_level level, const char *fmt, ...) {
    va_list ap;
    int d;
    char c, *s;

   va_start(ap, fmt);
    while (*fmt)
        switch (*fmt++) {
        case 's':              /* string */
            s = va_arg(ap, char *);
            printf("string %s\n", s);
            break;
        case 'd':              /* int */
            d = va_arg(ap, int);
            printf("int %d\n", d);
            break;
        case 'c':              /* char */
            /* need a cast here since va_arg only
               takes fully promoted types */
            c = (char) va_arg(ap, int);
            printf("char %c\n", c);
            break;
        }
    va_end(ap);
    qDebug() << "LOG!" << level << fmt;
}

bool core_environment(unsigned cmd, void *data) {
    qDebug() << cmd;
    switch(cmd) {
        case RETRO_ENVIRONMENT_GET_LOG_INTERFACE:
            auto logstruct = reinterpret_cast<retro_log_callback*>(data);
            logstruct->log = retrolog;
    }
    return true;
}
void video_refresh(const void *data, unsigned width, unsigned height, size_t pitch) {
    App::self()->videoRefresh(data, width, height, pitch);
}
void audio_sample(int16_t left, int16_t right) {

}
size_t audio_sample_batch(const int16_t *data,size_t frames) {
    qDebug() << "f";
 return 0;
}
void input_poll() {

}
int16_t input_state(unsigned port, unsigned device, unsigned index, unsigned id) {
    if(port || index || device != RETRO_DEVICE_JOYPAD)
        return 0;
    qDebug() << "ID: " <<id;
    switch(id) {
        case RETRO_DEVICE_ID_JOYPAD_A:
            return App::self()->getButtonState("A");
        case RETRO_DEVICE_ID_JOYPAD_B:
            return App::self()->getButtonState("B");
        case RETRO_DEVICE_ID_JOYPAD_UP:
            return App::self()->getButtonState("UP");
        case RETRO_DEVICE_ID_JOYPAD_DOWN:
            return App::self()->getButtonState("DOWN");
        case RETRO_DEVICE_ID_JOYPAD_LEFT:
            return App::self()->getButtonState("LEFT");
        case RETRO_DEVICE_ID_JOYPAD_RIGHT:
            return App::self()->getButtonState("RIGHT");
        case RETRO_DEVICE_ID_JOYPAD_START:
            return App::self()->getButtonState("START");
        case RETRO_DEVICE_ID_JOYPAD_SELECT:
            return App::self()->getButtonState("SELECT");
    }
    return 0;
}

void App::videoRefresh(const void *data, unsigned width, unsigned height, size_t pitch) {
    m_retroFrame->setImage({reinterpret_cast<const uchar*>(data), static_cast<int>(width), static_cast<int>(height), QImage::Format_RGBX8888});
}

void App::startRetroCore()
{
    // Load core dynamic library
    void* lrcore = dlopen("/home/seshpenguin/tmp/libretro-2048/2048_libretro.so", RTLD_LAZY | RTLD_LOCAL);
    qDebug() << ("Opened core!");
    auto retro_api_version = reinterpret_cast<unsigned(*)(void)>(dlsym(lrcore, "retro_api_version"));
    auto retro_set_environment = reinterpret_cast<void(*)(retro_environment_t)>(dlsym(lrcore, "retro_set_environment"));
    auto retro_set_video_refresh = reinterpret_cast<void(*)(retro_video_refresh_t)>(dlsym(lrcore, "retro_set_video_refresh"));
    auto retro_set_audio_sample = reinterpret_cast<void(*)(retro_audio_sample_t)>(dlsym(lrcore, "retro_set_audio_sample"));
    auto retro_set_audio_sample_batch = reinterpret_cast<void(*)(retro_audio_sample_batch_t)>(dlsym(lrcore, "retro_set_audio_sample_batch"));
    auto retro_set_input_poll = reinterpret_cast<void(*)(retro_input_poll_t)>(dlsym(lrcore, "retro_set_input_poll"));
    auto retro_set_input_state = reinterpret_cast<void(*)(retro_input_state_t)>(dlsym(lrcore, "retro_set_input_state"));
    auto retro_init = reinterpret_cast<void(*)(void)>(dlsym(lrcore, "retro_init"));
    auto retro_reset = reinterpret_cast<void(*)(void)>(dlsym(lrcore, "retro_reset"));
    auto retro_load_game = reinterpret_cast<void(*)(const struct retro_game_info *game)>(dlsym(lrcore, "retro_load_game"));
    auto retro_run = reinterpret_cast<void(*)(void)>(dlsym(lrcore, "retro_run"));

    qDebug() << "Core API version " << retro_api_version();

    retro_set_environment(&core_environment);
    retro_set_video_refresh(&video_refresh);
    retro_set_audio_sample(&audio_sample);
    retro_set_audio_sample_batch(&audio_sample_batch);
    retro_set_input_poll(&input_poll);
    retro_set_input_state(&input_state);
    retro_init();
    retro_reset();
    retro_game_info info = {.path = NULL, .data = NULL, .size = 0, .meta = NULL};
    retro_load_game(&info);

    connect(m_frameTimer, &QTimer::timeout, this, [retro_run]() { retro_run(); });
    m_frameTimer->start(33);
}
App* App::self()
{
    static App* a = new App();
    return a;
}

void App::setRetroFrame(RetroFrame *rf)
{
    m_retroFrame = rf;
}

void App::setButtonState(QString button, bool state)
{
    m_inputStates[button] = state;
    qDebug() << state;
}
bool App::getButtonState(QString button)
{
    return m_inputStates.contains(button) && m_inputStates[button];
}
