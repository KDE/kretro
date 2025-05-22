#include "retrogamemodel.h"
#include "app.h"
#include "objects/retrogame.h"
#include <QDir>

using namespace Qt::Literals::StringLiterals;

RetroGameModel::RetroGameModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_count(0)
{
    append(new RetroGame{u"2048"_s, u""_s, u"TWENTY_FORTY_EIGHT"_s, u"qrc:/2048_icon.png"_s, this});

    // Game Boy Advance
    QString homeDir = QDir::homePath();
    QDir romDir{homeDir + u"/Documents/Games"_s};
    QStringList roms = romDir.entryList(QStringList() << u"*.gba"_s, QDir::Files);
    for (QString rom : roms) {
        QString path = romDir.absoluteFilePath(rom);
        append(new RetroGame{rom, path, u"GBA"_s, u"qrc:/gba_default_icon.png"_s, this});
    }

    QStringList snes_roms = romDir.entryList(QStringList() << u"*.smc"_s, QDir::Files);
    for (QString rom : snes_roms) {
        QString path = romDir.absoluteFilePath(rom);
        append(new RetroGame{rom, path, u"SNES"_s, u"qrc:/snes_default_icon.png"_s, this});
    }

    QStringList nes_roms = romDir.entryList(QStringList() << u"*.nes"_s, QDir::Files);
    for (QString rom : nes_roms) {
        QString path = romDir.absoluteFilePath(rom);
        append(new RetroGame{rom, path, u"NES"_s, u"qrc:/nes_default_icon.png"_s, this});
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

void RetroGameModel::append(QObject *o) {
    int i = m_data.size();
    beginInsertRows(QModelIndex(), i, i);
    m_data.append(o);
    
    // Emit changed signals
    Q_EMIT countChanged(count());
    
    endInsertRows();
}

void RetroGameModel::insert(QObject *o, int i)
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
