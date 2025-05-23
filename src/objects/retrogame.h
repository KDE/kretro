/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2023-2025 Seshan Ravikumar <seshan@sineware.ca>
*/

#pragma once
#include <QObject>

class RetroGame : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString console READ console WRITE setConsole NOTIFY consoleChanged)
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
public:
    RetroGame(QString name, QString path, QString console, QString icon, QObject *parent = nullptr);

    void setName(const QString name);
    QString name() const;
    
    void setPath(const QString path);
    QString path() const;

    void setConsole(QString console);
    QString console() const;

    void setIcon(QString icon);
    QString icon() const;
Q_SIGNALS:
    void nameChanged();
    void pathChanged();
    void consoleChanged();
    void iconChanged();
private:
    QString m_name;
    QString m_path;
    QString m_console;
    QString m_icon;
};
