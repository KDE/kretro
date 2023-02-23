#include "retrogamemodel.h"
#include "app.h"
#include "objects/retrogame.h"
#include <QDir>

RetroGameModel::RetroGameModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_count(0)
{
    append(new RetroGame{"2048", "", "TWENTY_FORTY_EIGHT", "qrc:/2048_icon.png", this});

    // Game Boy Advance
    QString homeDir = QDir::homePath();
    QDir romDir{homeDir + "/Documents/Games"};
    QStringList roms = romDir.entryList(QStringList() << "*.gba", QDir::Files);
    for (QString rom : roms) {
        QString path = romDir.absoluteFilePath(rom);
        append(new RetroGame{rom, path, "GBA", "qrc:/gba_default_icon.png", this});
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
    emit countChanged(m_count);
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
    emit countChanged(count());
    
    endInsertRows();
}

void RetroGameModel::insert(QObject *o, int i)
{
    beginInsertRows(QModelIndex(), i, i);
    m_data.insert(i, o);

    // Emit changed signals
    emit countChanged(count());

    endInsertRows();
}
