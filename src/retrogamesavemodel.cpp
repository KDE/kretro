#include "retrogamesavemodel.h"
#include "app.h"
#include <QDir>
#include <QStandardPaths>
#include <QFileInfo>

using namespace Qt::Literals::StringLiterals;

namespace {
QString pathToSlot(const QString &path)
{
    return path.split(QLatin1Char('/'))
        .last()
        .split(QLatin1Char('.'))
        .first();
}
}

RetroGameSaveModel::RetroGameSaveModel(QObject *parent)
    : QAbstractListModel(parent)
{
    // Game Boy Advance
    const QDir romDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
     + QLatin1Char('/') + App::self()->getRomFilePath().split(QLatin1Char('/')).last() + QLatin1Char('/'));

    qDebug() << romDir;

    const QStringList roms = romDir.entryList(QStringList() << u"*.state"_s, QDir::Files);
    for (const QString &rom : roms) {
        const QString path = romDir.absoluteFilePath(rom);
        qDebug() << "Found save file: " << path;
        append(path);
    }
}

int RetroGameSaveModel::rowCount(const QModelIndex &p) const
{
    Q_UNUSED(p);
    return m_saves.size();
}

QVariant RetroGameSaveModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(checkIndex(index, CheckIndexOption::IndexIsValid | CheckIndexOption::DoNotUseParent));

    const auto &save = m_saves[index.row()];

    switch (role) {
    case SlotRole:
        return save.slot;
    case PathRole:
        return save.path;
    case LastModifiedRole:
        return QFileInfo(save.path).lastModified();
    default:
        return {};
    }
}

QHash<int, QByteArray> RetroGameSaveModel::roleNames() const
{
    return {
        {SlotRole, "slot"},
        {PathRole, "path"},
        {LastModifiedRole, "lastModified"},
    };
}

void RetroGameSaveModel::append(const QString &path) {
    int row = m_saves.size();

    beginInsertRows({}, row, row);
    m_saves.push_back(RetroGameSave { pathToSlot(path), path });
    endInsertRows();
}

void RetroGameSaveModel::insert(int row, const QString &path)
{
    beginInsertRows({}, row, row);
    m_saves.insert(std::next(m_saves.cbegin(), row), RetroGameSave { pathToSlot(path), path });
    endInsertRows();
}

void RetroGameSaveModel::removeSaveSlot(int row)
{
    beginRemoveRows({}, row, row);
    const auto path = m_saves[row].path;
    QFile file{path};
    file.remove();
    m_saves.erase(std::next(m_saves.cbegin(), row));
    endRemoveRows();
}
