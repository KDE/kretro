/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2023-2025 Seshan Ravikumar <seshan@sineware.ca>
*/

#include "retrogame.h"
RetroGame::RetroGame(const QString &name, const QString &path, const QString &console, const QString &icon, QObject *parent)
    : QObject(parent)
{
    setName(name);
    setPath(path);
    setConsole(console);
    setIcon(icon);
}

void RetroGame::setName(const QString &name)
{
    if (m_name == name)
        return;
    m_name = name;
    Q_EMIT nameChanged();
}
QString RetroGame::name() const
{
    return m_name;
}

void RetroGame::setPath(const QString &path)
{
    if (m_path == path)
        return;
    m_path = path;
    Q_EMIT pathChanged();
}
QString RetroGame::path() const
{
    return m_path;
}

void RetroGame::setConsole(const QString &console)
{
    if (m_console == console)
        return;
    m_console = console;
    Q_EMIT consoleChanged();
}
QString RetroGame::console() const
{
    return m_console;
}

void RetroGame::setIcon(const QString &icon)
{
    if (m_icon == icon)
        return;
    m_icon = icon;
    Q_EMIT iconChanged();
}
QString RetroGame::icon() const
{
    return m_icon;
}
