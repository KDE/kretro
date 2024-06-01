#pragma once
#include <QObject>

class RetroGameSave : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString slot READ slot WRITE setSlot NOTIFY slotChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
public:
    RetroGameSave(QString slot, QString path,  QObject *parent = nullptr);
    void setSlot(QString slot);
    QString slot() const;

    void setPath(QString path);
    QString path() const;

Q_SIGNALS:
    void slotChanged();
    void pathChanged();
private:
    QString m_slot;
    QString m_path;
};
