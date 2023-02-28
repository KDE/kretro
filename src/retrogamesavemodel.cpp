#include "retrogamesavemodel.h"
#include "app.h"
#include "objects/retrogamesave.h"
#include <QDir>

RetroGameSaveModel::RetroGameSaveModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_count(0)
{
    // Game Boy Advance
    QString homeDir = QDir::homePath();
    QDir romDir{QDir::homePath() + "/.local/share/kretro/" + App::self()->getRomFilePath().split("/").last() + "/"};
    QStringList roms = romDir.entryList(QStringList() << "*.state", QDir::Files);
    for (QString rom : roms) {
        QString path = romDir.absoluteFilePath(rom);
        qDebug() << "Found save file: " << path;
        append(new RetroGameSave{path.split("/").last().split(".").first(), path, this});
    }
}

int RetroGameSaveModel::count() const
{
    return m_count;
}

void RetroGameSaveModel::setCount(int count)
{
    if (m_count == count)
        return;

    m_count = count;
    emit countChanged(m_count);
}

int RetroGameSaveModel::rowCount(const QModelIndex &p) const
{
    Q_UNUSED(p);
    return m_data.size();
}

QVariant RetroGameSaveModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);
    return QVariant::fromValue(m_data.at(index.row()));
}

QHash<int, QByteArray> RetroGameSaveModel::roleNames() const
{
    return {{Qt::UserRole, "save"}};
}

void RetroGameSaveModel::append(QObject *o) {
    int i = m_data.size();
    beginInsertRows(QModelIndex(), i, i);
    m_data.append(o);
    
    // Emit changed signals
    emit countChanged(count());
    
    endInsertRows();
}

void RetroGameSaveModel::insert(QObject *o, int i)
{
    beginInsertRows(QModelIndex(), i, i);
    m_data.insert(i, o);

    // Emit changed signals
    emit countChanged(count());

    endInsertRows();
}
