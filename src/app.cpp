// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan.r@sineware.ca>

#include "app.h"
#include <KSharedConfig>
#include <KWindowConfig>
#include <QQuickWindow>
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
}

bool core_environment(unsigned cmd, void *data) {
    switch(cmd) {
        case RETRO_ENVIRONMENT_GET_LOG_INTERFACE: {
            auto logstruct = reinterpret_cast<retro_log_callback*>(data);
            logstruct->log = retrolog;
            break;
        }
        case RETRO_ENVIRONMENT_GET_CAN_DUPE: {
            bool *bval = (bool*)data;
            *bval = true;
            break;
        }
        case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT: {
            const enum retro_pixel_format *fmt = (enum retro_pixel_format *)data;
            qDebug() << "Requested pixel format" << *fmt;

            if (*fmt > RETRO_PIXEL_FORMAT_RGB565)
                return false;
            switch (*fmt) {
                case RETRO_PIXEL_FORMAT_XRGB8888:
                    App::self()->setImageFormat(QImage::Format_RGBX8888);
                    return true;
                case RETRO_PIXEL_FORMAT_RGB565:
                    App::self()->setImageFormat(QImage::Format_RGB16);
                    return true;
                default:
                    return false;
            }
            return false;
        }
        case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY:
        case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY:
            *(const char **)data = ".";
            return true;

        default:
            //qDebug() << RETRO_LOG_DEBUG <<  "Unhandled env #" << cmd;
            return false;
    }
    return true;
}

void video_refresh(const void *data, unsigned width, unsigned height, size_t pitch) {
    App::self()->videoRefresh(data, width, height, pitch);
}

void audio_sample(int16_t left, int16_t right) {
}

size_t audio_sample_batch(const int16_t *data,size_t frames) {
    App::self()->audioRefresh(data, frames);
    return 0;
}

void input_poll() {

}

int16_t input_state(unsigned port, unsigned device, unsigned index, unsigned id) {
    if(port || index || device != RETRO_DEVICE_JOYPAD)
        return 0;
    //qDebug() << "ID: " <<id;
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
    m_retroFrame->setImage({reinterpret_cast<const uchar*>(data), static_cast<int>(width), static_cast<int>(height), m_imageFormat});
}
void App::audioRefresh(const int16_t *data, size_t frames) {
    //m_audioBuffer.write(reinterpret_cast<const char*>(data), frames);
}

void App::startRetroCore()
{
    // Load core dynamic library
    void* lrcore = dlopen("/usr/lib/libretro/mednafen_gba_libretro.so", RTLD_LAZY);
    m_lrCore = lrcore;

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
    auto retro_load_game = reinterpret_cast<bool(*)(const struct retro_game_info *game)>(dlsym(lrcore, "retro_load_game"));
    auto retro_run = reinterpret_cast<void(*)(void)>(dlsym(lrcore, "retro_run"));
    auto retro_get_system_info = reinterpret_cast<void(*)(retro_system_info *)>(dlsym(lrcore, "retro_get_system_info"));
    auto retro_get_system_av_info = reinterpret_cast<void(*)(struct retro_system_av_info *info)>(dlsym(lrcore, "retro_get_system_av_info"));

    qDebug() << "Core API version " << retro_api_version();

    retro_set_environment(&core_environment);
    retro_set_video_refresh(&video_refresh);
    retro_set_audio_sample(&audio_sample);
    retro_set_audio_sample_batch(&audio_sample_batch);
    retro_set_input_poll(&input_poll);
    retro_set_input_state(&input_state);

    m_audioBuffer.open(QBuffer::ReadWrite);

    retro_init();

    //retro_reset();
    retro_system_av_info avinfo;
    retro_system_info system = {0};
    retro_game_info info{m_romFilePath.toStdString().c_str(), 0};

    FILE *file = fopen(m_romFilePath.toStdString().c_str(), "rb");
    if (!file) {
        qDebug() << "NO FILE!!!";
        return;
    }

    fseek(file, 0, SEEK_END);
	info.size = ftell(file);
	rewind(file);

    retro_get_system_info(&system);

    if (!system.need_fullpath) {
		info.data = malloc(info.size);

		if (!info.data || !fread((void*)info.data, info.size, 1, file)) {
            qDebug() << "LIBC error for some reason !?";
			return;
        }
	}

    if (!retro_load_game(&info)) {
        qDebug() << "The game failed to load!";
    }
    retro_get_system_av_info(&avinfo);
    m_avInfo = avinfo;

    qDebug() << avinfo.geometry.base_height << "x" << avinfo.geometry.base_width;
    qDebug() << avinfo.timing.fps;

    connect(m_frameTimer, &QTimer::timeout, this, [retro_run]() { retro_run(); });
    m_frameTimer->start(1000 / avinfo.timing.fps);
}
void App::stopRetroCore()
{
    auto retro_unload_game = reinterpret_cast<unsigned(*)(void)>(dlsym(m_lrCore, "retro_unload_game"));
    auto retro_deinit = reinterpret_cast<unsigned(*)(void)>(dlsym(m_lrCore, "retro_deinit"));
    m_frameTimer->stop();
    retro_unload_game();
    retro_deinit();
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
}

bool App::getButtonState(QString button)
{
    return m_inputStates.contains(button) && m_inputStates[button];
}

void App::setImageFormat(QImage::Format format)
{
    m_imageFormat = format;
}

QString App::getEnv(QString key)
{
    return qgetenv(qPrintable(key));
}

void App::setRomFilePath(QString path)
{
    m_romFilePath = path;
}