#include "retrogame.h"
RetroGame::RetroGame(QString name, QString path, QString console, QObject *parent)
    : QObject(parent)
{
    setName(name);
    setPath(path);
    setConsole(console);
}

void RetroGame::setName(const QString name)
{
    if (m_name == name)
        return;
    m_name = name;
    emit nameChanged();
}
QString RetroGame::name() const
{
    return m_name;
}

void RetroGame::setPath(const QString path)
{
    if (m_path == path)
        return;
    m_path = path;
    emit pathChanged();
}
QString RetroGame::path() const
{
    return m_path;
}

void RetroGame::setConsole(QString console)
{
    if (m_console == console)
        return;
    m_console = console;
    emit consoleChanged();
}
QString RetroGame::console() const
{
    return m_console;
}