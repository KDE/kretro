/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2025 Seshan Ravikumar <seshan@sineware.ca>
*/
#pragma once
#include <QObject>
#include <QQmlEngine>
#include <QDBusConnection>
#include <QDBusInterface>
#include <qqmlintegration.h>

class IdleInhibit : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    IdleInhibit(QObject *parent = nullptr);
    ~IdleInhibit();

    static IdleInhibit *create(QQmlEngine *qmlEngine, QJSEngine *);

    Q_INVOKABLE void acquire();
    Q_INVOKABLE void release();

private:
    QDBusConnection m_connection;
    QDBusInterface m_interface;
    uint m_cookie = 0;
};