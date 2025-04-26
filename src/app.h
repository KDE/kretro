// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan@sineware.ca>

#pragma once
#include <QObject>
#include <QTimer>
#include "retroframe.h"
#include "libretro.h"
#include <qbuffer.h>
#include "objects/retrogame.h"
#ifdef __linux__ 
    #include <alsa/asoundlib.h>
#elif __APPLE__
    
#else
#endif

using namespace Qt::Literals::StringLiterals;

class QQuickWindow;

class App : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString error READ error WRITE setError NOTIFY errorChanged)
public:
    App(QObject* parent = nullptr);
    // Restore current window geometry
    Q_INVOKABLE void restoreWindowGeometry(QQuickWindow *window, const QString &group = u"main"_s) const;
    // Save current window geometry
    Q_INVOKABLE void saveWindowGeometry(QQuickWindow *window, const QString &group = u"main"_s) const;

    Q_INVOKABLE void startRetroCore();
    Q_INVOKABLE void stopRetroCore();

    void videoRefresh(const void *data, unsigned width, unsigned height, int pitch);
    void audioRefresh(const int16_t *data,size_t frames);

    static App *self();

    void setRetroFrame(RetroFrame *rf);

    Q_INVOKABLE void setButtonState(QString button, bool state);
    bool getButtonState(QString button);

    void setImageFormat(QImage::Format format);

    retro_system_av_info m_avInfo;
    
    Q_INVOKABLE void setRomFilePath(QString path);
    Q_INVOKABLE void setRomConsole(QString console);

    void setError(const QString &author);
    QString error() const;


    QString getRomFilePath() const;

    Q_INVOKABLE void loadSaveSlot(const QString &slot);
    Q_INVOKABLE void saveSaveSlot(const QString &slot);
    Q_INVOKABLE QString saveNewSaveSlot();

    QString appdataDir() const;

Q_SIGNALS:
    void errorChanged();

private:
    QImage::Format m_imageFormat;

    RetroFrame *m_retroFrame;
    QTimer *m_frameTimer;
    bool m_isRunning;
    QHash<QString, bool> m_inputStates;

    QBuffer *m_audioBuffer;

    QString m_romFilePath;
    QString m_romConsole;

    QString m_error;

    void *m_lrCore;
#ifdef __linux__
    snd_pcm_t *m_pcm;
#endif

    QString m_appdataDir;
};
