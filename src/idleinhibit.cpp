/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2025 Seshan Ravikumar <seshan@sineware.ca>
*/
#include "idleinhibit.h"
#include <QDBusInterface>
#include <QDBusReply>

using namespace Qt::Literals::StringLiterals;

IdleInhibit::IdleInhibit(QObject *parent)
    : QObject(parent)
    #ifdef Q_OS_LINUX
    , m_connection(QDBusConnection::sessionBus())
    , m_interface(u"org.freedesktop.ScreenSaver"_s, u"/org/freedesktop/ScreenSaver"_s, u"org.freedesktop.ScreenSaver"_s, m_connection)
    #endif
{
    qInfo() << "Inhibiting idle on org.freedesktop.ScreenSaver";
    acquire();
}

IdleInhibit::~IdleInhibit()
{
    qInfo() << "Releasing idle inhibition on org.freedesktop.ScreenSaver";
    release();
}

void IdleInhibit::acquire()
{
    if(!m_interface.isValid()) {
        qWarning() << "Could not inhibit idle, failed to connect to org.freedesktop.ScreenSaver: "
                   << m_connection.lastError().message();
        return;
    }

    QDBusReply<uint> reply = m_interface.call(
        u"Inhibit"_s,
        u"kretro"_s,
        u"KRetro is playing a game"_s
    );

    if (!reply.isValid()) {
        qWarning() << "Failed to inhibit idle: " << reply.error().message();
    } else {
        qInfo() << "Idle inhibition acquired with cookie:" << reply.value();
    }

    m_cookie = reply.value();
}

void IdleInhibit::release()
{
    if (m_cookie == 0) {
        qWarning() << "No idle inhibition to release.";
        return;
    }

    if(!m_interface.isValid()) {
        qWarning() << "Could not release idle inhibition, failed to connect to org.freedesktop.ScreenSaver: "
                   << m_connection.lastError().message();
        return;
    }

    QDBusReply<void> reply = m_interface.call(u"UnInhibit"_s, m_cookie);

    if (!reply.isValid()) {
        qWarning() << "Failed to release idle inhibition: " << reply.error().message();
    } else {
        qInfo() << "Idle inhibition released.";
    }

    m_cookie = 0;
}
