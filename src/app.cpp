// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023-2025 Seshan Ravikumar <seshan@sineware.ca>

#include "app.h"
#include <KSharedConfig>
#include <QTemporaryFile>
#include <QDir>
#include <dlfcn.h>
#include <cstdarg>
#include <QStandardPaths>
#include "kretroconfig.h"

#ifdef __linux__ 
    #include <alsa/asoundlib.h>
#elif __APPLE__
    #include <AudioToolbox/AudioToolbox.h>
#else
#endif

using namespace Qt::Literals::StringLiterals;

App::App(QObject* parent)
    : QObject(parent)
    , m_retroFrame{nullptr}
    , m_frameTimer{new QTimer{this}}
    , m_coreVariables{}
    , m_appdataDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))
{
    m_gamesDir = Config::self()->romsDirectory();
    m_systemDir = m_appdataDir + u"/"_s + u"system"_s;
}

void retrolog(enum retro_log_level level, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    
    QString message = QString::vasprintf(fmt, ap);
    va_end(ap);
    
    switch (level) {
        case RETRO_LOG_DEBUG:
            qDebug() << message;
            break;
        case RETRO_LOG_INFO:
            qInfo() << message;
            break;
        case RETRO_LOG_WARN:
            qWarning() << message;
            break;
        case RETRO_LOG_ERROR:
            qCritical() << message;
            break;
        default:
            qDebug() << message;
            break;
    }
}

bool core_environment(unsigned cmd, void *data)
{
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
            *(const char **)data = App::self()->systemDir().toLocal8Bit().data();
            qDebug () << "Core System directory" << *(const char **)data;
            return true;
        case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY:
            *(const char **)data = App::self()->appdataDir().toLocal8Bit().data();
            qDebug () << "Core Save directory" << *(const char **)data;
            return true;
        case RETRO_ENVIRONMENT_GET_VARIABLE: {
            struct retro_variable *var = (struct retro_variable *)data;
            // value here is the user overridden option (from one of the options declared in SET_VARIABLES), if it is set
            auto value = KSharedConfig::openConfig()->group(u"LibretroCoreVariables"_s).readEntry(var->key, QString());
            if (value.isEmpty()) {
                var->value = nullptr;
                return false;
            }
            qDebug() << "Get Variable" << var->key << value;
            if (var->value) {
                free((void*)var->value);
            }
            var->value = strdup(value.toLocal8Bit().data());
            return true;
        }
        case RETRO_ENVIRONMENT_SET_VARIABLES: {
            const struct retro_variable *vars = (const struct retro_variable *)data;
            // value here is the variable description and list of possible values (for frontend)
            // not the current value
            while (vars->key) {
                qDebug() << "Set Variable" << vars->key << vars->value;
                App::self()->setCoreVariable(QString::fromLocal8Bit(vars->key), QString::fromLocal8Bit(vars->value));
                vars++;
            }
            break;
        }
        default:
            //qDebug() << RETRO_LOG_DEBUG <<  "Unhandled env #" << cmd;
            return false;
    }
    return true;
}

void video_refresh(const void *data, unsigned width, unsigned height, size_t pitch)
{
    App::self()->videoRefresh(data, width, height, pitch);
}

void audio_sample(int16_t left, int16_t right)
{
}

size_t audio_sample_batch(const int16_t *data,size_t frames)
{
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
            return App::self()->getButtonState(u"A"_s);
        case RETRO_DEVICE_ID_JOYPAD_B:
            return App::self()->getButtonState(u"B"_s);
        case RETRO_DEVICE_ID_JOYPAD_X:
            return App::self()->getButtonState(u"X"_s);
        case RETRO_DEVICE_ID_JOYPAD_Y:
            return App::self()->getButtonState(u"Y"_s);
        case RETRO_DEVICE_ID_JOYPAD_L:
            return App::self()->getButtonState(u"L1"_s);
        case RETRO_DEVICE_ID_JOYPAD_R:
            return App::self()->getButtonState(u"R1"_s);
        case RETRO_DEVICE_ID_JOYPAD_UP:
            return App::self()->getButtonState(u"UP"_s);
        case RETRO_DEVICE_ID_JOYPAD_DOWN:
            return App::self()->getButtonState(u"DOWN"_s);
        case RETRO_DEVICE_ID_JOYPAD_LEFT:
            return App::self()->getButtonState(u"LEFT"_s);
        case RETRO_DEVICE_ID_JOYPAD_RIGHT:
            return App::self()->getButtonState(u"RIGHT"_s);
        case RETRO_DEVICE_ID_JOYPAD_START:
            return App::self()->getButtonState(u"START"_s);
        case RETRO_DEVICE_ID_JOYPAD_SELECT:
            return App::self()->getButtonState(u"SELECT"_s);
    }
    return 0;
}

void App::videoRefresh(const void *data, unsigned width, unsigned height, size_t pitch) {
    if (m_retroFrame) {
        m_retroFrame->updateFrameData(data, width, height, pitch, m_imageFormat);
    }
}
void App::audioRefresh(const int16_t* data, size_t frames) {
    if (!m_audioDevice) return;

    size_t byteSize = frames * m_audioFormat.channelCount() * sizeof(int16_t);
    if (m_audioSink->bytesFree() < static_cast<qint64>(byteSize))return;
    
    m_audioDevice->write(reinterpret_cast<const char*>(data), byteSize);
}

void App::startRetroCore()
{
    QDir().mkdir(m_appdataDir);
    QDir().mkdir(m_systemDir);

    // Load core dynamic library
    QString coreName = u""_s;
    if(m_romConsole == u"TWENTY_FORTY_EIGHT"_s) {
        coreName = u"2048_libretro.so"_s;
    } else if (m_romConsole == u"GBA"_s) {
        coreName = Config::self()->gbaCore();
    } else if (m_romConsole == u"SNES"_s) {
        coreName = Config::self()->snesCore();
    } else if (m_romConsole == u"NES"_s) {
        coreName = Config::self()->nesCore();
    } else if (m_romConsole == u"SMS"_s) {
        coreName = Config::self()->smsCore();
    } else if (m_romConsole == u"GENESIS"_s) {
        coreName = Config::self()->genesisCore();
    }
    QString core_full_path = (coreName == u"2048_libretro.so"_s) ? 
        QTemporaryFile::createNativeFile(u":/cores/"_s + QSysInfo::buildCpuArchitecture() + u"/"_s + coreName)->fileName() : 
        Config::self()->libretroCoresDirectory() + u"/"_s + coreName;

    qDebug() << "Loading core from" << core_full_path;

    // check if core exists
    if (!QFile::exists(core_full_path)) {
        qDebug() << "Core not found!";
        setError(u"Core not found!"_s);
        return;
    }

    void* lrcore = dlopen(core_full_path.toLocal8Bit().data(), RTLD_LAZY);
    if (!lrcore) {
        qDebug() << "Failed to load core!";
        setError(u"Failed to load core!"_s);
        return;
    }
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

    retro_init();

    //retro_reset();
    retro_system_av_info avinfo;
    retro_system_info system = {0};

    if(m_romFilePath != u""_s) {
        retro_game_info info{m_romFilePath.toStdString().c_str(), 0};
        FILE *file = fopen(m_romFilePath.toStdString().c_str(), "rb");
        if (!file) {
                qDebug() << "NO FILE!!!";
                return;
        }

        fseek(file, 0, SEEK_END);
        info.size = ftell(file);
        rewind(file);
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
    } else {
        retro_game_info info = {.path = NULL, .data = NULL, .size = 0, .meta = NULL};
        if (!retro_load_game(&info)) {
            qDebug() << "The game failed to load!";
        }
    }
    
    retro_get_system_info(&system);
    retro_get_system_av_info(&avinfo);
    m_avInfo = avinfo;

    qDebug() << avinfo.geometry.base_height << "x" << avinfo.geometry.base_width;
    qDebug() << avinfo.timing.fps;
    qDebug() << avinfo.timing.sample_rate;

    // Load save state if it exists
    auto retro_unserialize = reinterpret_cast<bool(*)(const void *data, size_t size)>(dlsym(m_lrCore, "retro_unserialize"));
    // load state from ~/.local/share
    QFile stateFile{m_appdataDir + u"/"_s + m_romFilePath.split(u"/"_s).last() + u"/0.state"_s};
    if(stateFile.exists()) {
        stateFile.open(QIODevice::ReadOnly);
        QByteArray stateData = stateFile.readAll();
        retro_unserialize(stateData.data(), stateData.size());
        qDebug() << "Loaded state!";
    }

    m_audioFormat.setSampleRate(avinfo.timing.sample_rate);
    m_audioFormat.setChannelCount(2);
    m_audioFormat.setSampleFormat(QAudioFormat::Int16);

    const QAudioDevice &defaultDeviceInfo = QMediaDevices::defaultAudioOutput();
    m_audioSink = new QAudioSink(defaultDeviceInfo, m_audioFormat, this);
    m_audioDevice = m_audioSink->start();
    
    m_frameTimer = new QTimer{this};
    connect(m_frameTimer, &QTimer::timeout, this, [retro_run]() { retro_run(); });
    m_frameTimer->setTimerType(Qt::PreciseTimer);
    m_frameTimer->start(1000 / avinfo.timing.fps);

    m_isRunning = true;
}
void App::stopRetroCore()
{
    if(!m_isRunning) {
        return;
    }
    auto retro_unload_game = reinterpret_cast<unsigned(*)(void)>(dlsym(m_lrCore, "retro_unload_game"));
    auto retro_deinit = reinterpret_cast<unsigned(*)(void)>(dlsym(m_lrCore, "retro_deinit"));
    m_frameTimer->stop();

    // serialize state and save it to ~/.local/share
    auto retro_serialize_size = reinterpret_cast<size_t(*)(void)>(dlsym(m_lrCore, "retro_serialize_size"));
    auto retro_serialize = reinterpret_cast<bool(*)(void*, size_t)>(dlsym(m_lrCore, "retro_serialize"));
    auto size = retro_serialize_size();
    void* data = malloc(size);
    if(retro_serialize(data, size)) {
        QDir().mkdir(m_appdataDir + u"/"_s + m_romFilePath.split(u"/"_s).last());
        QFile file{m_appdataDir + u"/"_s + m_romFilePath.split(u"/"_s).last() + u"/0.state"_s};
        file.open(QIODevice::WriteOnly);
        file.write((char*)data, size);
        file.close();
        qDebug() << "Saved state!";
    }

    retro_unload_game();
    retro_deinit();

    if (m_audioSink) {
        m_audioSink->stop();
        delete m_audioSink;
        m_audioSink = nullptr;
    }
    m_audioDevice = nullptr;

    delete m_frameTimer;

    clearCoreVariables();

    m_isRunning = false;
    qDebug() << "Stopped core!";
}

void App::resetRetroCore()
{
    auto retro_reset = reinterpret_cast<void(*)(void)>(dlsym(m_lrCore, "retro_reset"));
    retro_reset();
}


App* App::self()
{
    static App* a = new App();
    return a;
}

App *App::create(QQmlEngine *qmlEngine, QJSEngine *)
{
    return App::self();
}

void App::setRetroFrame(RetroFrame *rf)
{
    m_retroFrame = rf;
}

void App::setButtonState(const QString &button, bool state)
{
    m_inputStates[button] = state;
}

bool App::getButtonState(const QString &button)
{
    const auto val = m_inputStates.contains(button) && m_inputStates[button];
    return val;
}

void App::setImageFormat(QImage::Format format)
{
    m_imageFormat = format;
}

void App::setRomFilePath(QString path)
{
    m_romFilePath = path;
}
void App::setRomConsole(QString console)
{
    m_romConsole = console;
}
void App::setError(const QString &error)
{
    if (m_error == error) {
        return;
    }
    m_error = error;
    Q_EMIT errorChanged();
}
QString App::error() const
{
    return m_error;
}

QString App::getRomFilePath() const
{
    return m_romFilePath;
}

void App::loadSaveSlot(const QString &path)
{
    auto retro_unserialize = reinterpret_cast<bool(*)(const void *data, size_t size)>(dlsym(m_lrCore, "retro_unserialize"));
    QFile stateFile{path};
    if(stateFile.exists()) {
        stateFile.open(QIODevice::ReadOnly);
        QByteArray stateData = stateFile.readAll();
        retro_unserialize(stateData.data(), stateData.size());
        qDebug() << "Loaded state!";
    }
}

void App::saveSaveSlot(const QString &path)
{
    auto retro_serialize_size = reinterpret_cast<size_t(*)(void)>(dlsym(m_lrCore, "retro_serialize_size"));
    auto retro_serialize = reinterpret_cast<bool(*)(void*, size_t)>(dlsym(m_lrCore, "retro_serialize"));
    auto size = retro_serialize_size();
    void* data = malloc(size);
    if(retro_serialize(data, size)) {
        QFile file{path};
        file.open(QIODevice::WriteOnly);
        file.write((char*)data, size);
        file.close();
        qDebug() << "Saved state!";
    }
}

QString App::saveNewSaveSlot()
{
    auto retro_serialize_size = reinterpret_cast<size_t(*)(void)>(dlsym(m_lrCore, "retro_serialize_size"));
    auto retro_serialize = reinterpret_cast<bool(*)(void*, size_t)>(dlsym(m_lrCore, "retro_serialize"));
    auto size = retro_serialize_size();
    void* data = malloc(size);
    if (retro_serialize(data, size)) {
        QDir().mkdir(m_appdataDir + u"/"_s + m_romFilePath.split(u"/"_s).last());
        QFile file{m_appdataDir + u"/"_s + m_romFilePath.split(u"/"_s).last() + u"/"_s + QString::number(QDir(m_appdataDir + u"/"_s + m_romFilePath.split(u"/"_s).last()).entryList().count()) + u".state"_s};
        file.open(QIODevice::WriteOnly);
        file.write((char*)data, size);
        file.close();
        qDebug() << "Saved state!";
        return file.fileName();
    }

    return {};
}

QVariantMap App::coreVariables() const
{
    return m_coreVariables;
}

void App::setCoreVariable(const QString &key, const QString &value)
{
    m_coreVariables[key] = value;
    Q_EMIT coreVariablesChanged();
}

void App::clearCoreVariables()
{
    m_coreVariables.clear();
    Q_EMIT coreVariablesChanged();
}

QString App::getUserCoreVariable(const QString &key) const
{
    return KSharedConfig::openConfig()->group(u"LibretroCoreVariables"_s).readEntry(key, QString());
}

void App::saveUserCoreVariable(const QString &key, const QString &value)
{
    KSharedConfig::openConfig()->group(u"LibretroCoreVariables"_s).writeEntry(key, value);
    KSharedConfig::openConfig()->sync();
}

void App::resetUserCoreVariable(const QString &key)
{
    KSharedConfig::openConfig()->group(u"LibretroCoreVariables"_s).deleteEntry(key);
    KSharedConfig::openConfig()->sync();
}

QString App::appdataDir() const
{
    return m_appdataDir;
}
QString App::systemDir() const
{
    return m_systemDir;
}

QString App::gamesDir() const
{
    return m_gamesDir;
}