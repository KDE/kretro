/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2023-2025 Seshan Ravikumar <seshan@sineware.ca>
*/

#include "retrogamesave.h"
RetroGameSave::RetroGameSave(QString slot, QString path, QObject *parent)
    : QObject(parent)
{
    setSlot(slot);
    setPath(path);
}

void RetroGameSave::setSlot(QString slot)
{
    if (m_slot == slot)
        return;

    m_slot = slot;
    Q_EMIT slotChanged();
}
QString RetroGameSave::slot() const
{
    return m_slot;
}


void RetroGameSave::setPath(QString path)
{
    if (m_path == path)
        return;

    m_path = path;
    Q_EMIT pathChanged();
}
QString RetroGameSave::path() const
{
    return m_path;
}


#include "moc_retrogamesave.cpp"
