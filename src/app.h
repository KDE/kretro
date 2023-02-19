// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan.r@sineware.ca>

#pragma once

#include <QObject>
#include <QTimer>

#include "RetroFrame.h"
#include "libretro.h"
#include <qbuffer.h>

class QQuickWindow;

class App : public QObject
{
    Q_OBJECT
public:
    App(QObject* parent = nullptr);
    // Restore current window geometry
    Q_INVOKABLE void restoreWindowGeometry(QQuickWindow *window, const QString &group = QStringLiteral("main")) const;
    // Save current window geometry
    Q_INVOKABLE void saveWindowGeometry(QQuickWindow *window, const QString &group = QStringLiteral("main")) const;

    Q_INVOKABLE void startRetroCore();

    void videoRefresh(const void *data, unsigned width, unsigned height, size_t pitch);
    void audioRefresh(const int16_t *data,size_t frames);

    static App *self();

    void setRetroFrame(RetroFrame *rf);

    Q_INVOKABLE void setButtonState(QString button, bool state);
    bool getButtonState(QString button);

    void setImageFormat(QImage::Format format);

    retro_system_av_info m_avInfo;

private:
    QImage::Format m_imageFormat;

    RetroFrame *m_retroFrame;
    QTimer *m_frameTimer;
    bool m_isRunning;
    QHash<QString, bool> m_inputStates;

    QBuffer m_audioBuffer;
};
