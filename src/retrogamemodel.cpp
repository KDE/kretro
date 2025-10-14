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

    const QString romsPath = generalGroup.readPathEntry(u"romsDirectory"_s,
                                                QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + u"/Games"_s);
    QDir romDir{romsPath};

    // ROM system definitions: {extensions, system, icon}
    static const std::array<std::tuple<QStringList, QString, QString>, 5> romSystems{{
        {{u"*.gba"_s}, u"GBA"_s, u"qrc:/gba_default_icon.png"_s},
        {{u"*.smc"_s}, u"SNES"_s, u"qrc:/snes_default_icon.png"_s},
        {{u"*.nes"_s}, u"NES"_s, u"qrc:/nes_default_icon.png"_s},
        {{u"*.sms"_s, u"*.gg"_s}, u"SMS"_s, u"qrc:/sms_default_icon.png"_s},
        {{u"*.gen"_s, u"*.md"_s}, u"GENESIS"_s, u"qrc:/genesis_default_icon.png"_s}
    }};

    auto scanDirectory = [this](const QDir &dir, auto &self) -> void {
        for (const auto &[extensions, system, icon] : romSystems) {
            for (const QString &rom : dir.entryList(extensions, QDir::Files)) {
                append(new RetroGame{rom, dir.absoluteFilePath(rom), system, icon, this});
            }
        }
        for (const QString &subdir : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            self(QDir{dir.absoluteFilePath(subdir)}, self);
        }
    };
    scanDirectory(romDir, scanDirectory);
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

#include "moc_retrogamemodel.cpp"
