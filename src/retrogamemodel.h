#pragma once
#include <QAbstractListModel>
#include <QObject>
#include <QHash>

class RetroGameModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit RetroGameModel(QObject *parent = nullptr);
    Q_INVOKABLE void append(QObject* o);
    Q_INVOKABLE void insert(QObject* o, int i);

    int rowCount(const QModelIndex &p) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    int count() const;
signals:
    void countChanged(int count);
public slots:
    void setCount(int count);
private:
    int m_count;
    QList<QObject*> m_data;
};
