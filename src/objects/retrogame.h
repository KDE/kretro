#pragma once
#include <QObject>

class RetroGame : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString console READ console WRITE setConsole NOTIFY consoleChanged)
public:
    RetroGame(QString name, QString path, QString console, QObject *parent = nullptr);

    void setName(const QString name);
    QString name() const;
    
    void setPath(const QString path);
    QString path() const;

    void setConsole(QString console);
    QString console() const;
signals:
    void nameChanged();
    void pathChanged();
    void consoleChanged();
private:
    QString m_name;
    QString m_path;
    QString m_console;
};