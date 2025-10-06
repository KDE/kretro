// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan@sineware.ca>

#pragma once

#include <QObject>
#include <QTimer>
#include <QAudioFormat>
#include <QAudioSink>
#include <QIODevice>
#include <QBuffer>
#include <QMediaDevices>
#include <QThread>

#include <qcontainerfwd.h>
#include <qqmlintegration.h>

#include "retroframe.h"
#include "libretro.h"
#include "objects/retrogame.h"
#include "retropad.h"

using namespace Qt::Literals::StringLiterals;

class QQuickWindow;

class App : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString error READ error WRITE setError NOTIFY errorChanged)
    Q_PROPERTY(QVariantMap coreVariables READ coreVariables NOTIFY coreVariablesChanged)
public:
    static App *self();
    static App *create(QQmlEngine *qmlEngine, QJSEngine *);

    Q_INVOKABLE void startRetroCore();
    Q_INVOKABLE void stopRetroCore();
    Q_INVOKABLE void resetRetroCore();

    void videoRefresh(const void *data, unsigned width, unsigned height, size_t pitch);
    void audioRefresh(const int16_t *data,size_t frames);

    void setRetroFrame(RetroFrame *rf);

    void setImageFormat(QImage::Format format);

    retro_system_av_info m_avInfo;
    
    Q_INVOKABLE void setRomFilePath(const QString &path);
    Q_INVOKABLE void setRomConsole(const QString &console);

    // Map of core variables (names and options list for UI)
    void setCoreVariable(const QString &key, const QString &value);
    QString getCoreVariable(const QString &key) const;
    void clearCoreVariables();
    QVariantMap coreVariables() const;
    // Allows a user to override the default core variable from QML
    Q_INVOKABLE QString getUserCoreVariable(const QString &key) const;
    Q_INVOKABLE void saveUserCoreVariable(const QString &key, const QString &value);
    Q_INVOKABLE void resetUserCoreVariable(const QString &key);
    
    void setError(const QString &author);
    QString error() const;

    QString getRomFilePath() const;

    Q_INVOKABLE void loadSaveSlot(const QString &slot);
    Q_INVOKABLE void saveSaveSlot(const QString &slot);
    Q_INVOKABLE QString saveNewSaveSlot();

    Q_INVOKABLE QString appdataDir() const;
    Q_INVOKABLE QString systemDir() const;
    Q_INVOKABLE QString gamesDir() const;

    Q_INVOKABLE void startRetroPad();
    Q_INVOKABLE void stopRetroPad();
    Q_INVOKABLE RetroPad *getRetroPad() const { return m_retroPad; }

    Q_INVOKABLE bool isFlatpak() const;

Q_SIGNALS:
    void errorChanged();
    void coreVariablesChanged();

private:
    explicit App(QObject* parent = nullptr);

    QImage::Format m_imageFormat;

    RetroFrame *m_retroFrame;
    bool m_isRunning;

    QString m_romFilePath;
    QString m_romConsole;
    QVariantMap m_coreVariables;

    QString m_error;

    void *m_lrCore;
    QThread *m_retroCoreThread;
    QMutex m_retroCoreMutex;


    QAudioSink* m_audioSink = nullptr;
    QIODevice* m_audioDevice = nullptr;
    QAudioFormat m_audioFormat;

    QString m_appdataDir;
    QString m_systemDir;
    QString m_gamesDir;

    RetroPad *m_retroPad;

};
