/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2023-2025 Seshan Ravikumar <seshan@sineware.ca>
*/

#include "retrogamemodel.h"
#include <QDir>

#include "kretroconfig.h"

using namespace Qt::Literals::StringLiterals;

RetroGameModel::RetroGameModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_count(0)
{
    append(new RetroGame{u"2048"_s, u""_s, u"TWENTY_FORTY_EIGHT"_s, u"qrc:/2048_icon.png"_s, this});

    KSharedConfigPtr config = KSharedConfig::openConfig();
    KConfigGroup generalGroup = config->group(u"General"_s);

    QString romsPath = generalGroup.readPathEntry(u"romsDirectory"_s,
                                                QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + u"/Games"_s);
    QDir romDir{romsPath};

    // Game Boy Advance
    QStringList gba_roms = romDir.entryList(QStringList() << u"*.gba"_s, QDir::Files);
    for (QString rom : gba_roms) {
        QString path = romDir.absoluteFilePath(rom);
        append(new RetroGame{rom, path, u"GBA"_s, u"qrc:/gba_default_icon.png"_s, this});
    }

    // Super Nintendo Entertainment System
    QStringList snes_roms = romDir.entryList(QStringList() << u"*.smc"_s, QDir::Files);
    for (QString rom : snes_roms) {
        QString path = romDir.absoluteFilePath(rom);
        append(new RetroGame{rom, path, u"SNES"_s, u"qrc:/snes_default_icon.png"_s, this});
    }

    // Nintendo Entertainment System
    QStringList nes_roms = romDir.entryList(QStringList() << u"*.nes"_s, QDir::Files);
    for (QString rom : nes_roms) {
        QString path = romDir.absoluteFilePath(rom);
        append(new RetroGame{rom, path, u"NES"_s, u"qrc:/nes_default_icon.png"_s, this});
    }

    // Sega Master System / Game Gear
    QStringList sms_roms = romDir.entryList(QStringList() << u"*.sms"_s << u"*.gg"_s, QDir::Files);
    for (QString rom : sms_roms) {
        QString path = romDir.absoluteFilePath(rom);
        append(new RetroGame{rom, path, u"SMS"_s, u"qrc:/sms_default_icon.png"_s, this});
    }

    // Sega Genesis / Mega Drive
    QStringList genesis_roms = romDir.entryList(QStringList() << u"*.gen"_s << u"*.md"_s, QDir::Files);
    for (QString rom : genesis_roms) {
        QString path = romDir.absoluteFilePath(rom);
        append(new RetroGame{rom, path, u"GENESIS"_s, u"qrc:/genesis_default_icon.png"_s, this});
    }
}

int RetroGameModel::count() const
{
    return m_count;
}

void RetroGameModel::setCount(int count)
{
    if (m_count == count)
        return;

    m_count = count;
    Q_EMIT countChanged(m_count);
}

int RetroGameModel::rowCount(const QModelIndex &p) const
{
    Q_UNUSED(p);
    return m_data.size();
}

QVariant RetroGameModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);
    return QVariant::fromValue(m_data.at(index.row()));
}

QHash<int, QByteArray> RetroGameModel::roleNames() const
{
    return {{Qt::UserRole, "rom"}};
}

void RetroGameModel::append(RetroGame *o) {
    int i = m_data.size();
    beginInsertRows(QModelIndex(), i, i);
    m_data.append(o);
    
    // Emit changed signals
    Q_EMIT countChanged(count());
    
    endInsertRows();
}

void RetroGameModel::insert(RetroGame *o, int i)
{
    beginInsertRows(QModelIndex(), i, i);
    m_data.insert(i, o);

    // Emit changed signals
    Q_EMIT countChanged(count());

    endInsertRows();
}

QVariant RetroGameModel::get(int index) const
{
    if (index < 0 || index >= m_data.size()) {
        return QVariant();
    }
    return QVariant::fromValue(m_data.at(index));
}
