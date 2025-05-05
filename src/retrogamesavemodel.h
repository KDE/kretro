#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QHash>
#include <QDateTime>
#include <qqmlintegration.h>

struct RetroGameSave
{
    QString slot;
    QString path;
};

using RetroGameSaveList = std::vector<RetroGameSave>;

class RetroGameSaveModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit RetroGameSaveModel(QObject *parent = nullptr);

    enum ExtraRoles {
        SlotRole = Qt::UserRole + 1,
        PathRole,
        LastModifiedRole,
    };

    Q_INVOKABLE void append(const QString &path);
    Q_INVOKABLE void insert(int row, const QString &path);
    Q_INVOKABLE void removeSaveSlot(int row);

    int rowCount(const QModelIndex &p) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    RetroGameSaveList m_saves;
};
