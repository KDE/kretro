// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan.r@sineware.ca>

#pragma once

#include <QObject>
#include <QTimer>

#include "RetroFrame.h"

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

    void startRetroCore();

    void videoRefresh(const void *data, unsigned width, unsigned height, size_t pitch);

    static App *self();

    void setRetroFrame(RetroFrame *rf);

    Q_INVOKABLE void setButtonState(QString button, bool state);
    bool getButtonState(QString button);

    void setImageFormat(QImage::Format format);

private:
    QImage::Format m_imageFormat;

    RetroFrame *m_retroFrame;
    QTimer *m_frameTimer;
    bool m_isRunning;
    QHash<QString, bool> m_inputStates;
};
