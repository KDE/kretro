/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2023-2025 Seshan Ravikumar <seshan@sineware.ca>
*/

#pragma once
#include <QAbstractListModel>
#include <QObject>
#include <QHash>
#include <qqmlintegration.h>
#include "objects/retrogame.h"

class RetroGameModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit RetroGameModel(QObject *parent = nullptr);
    Q_INVOKABLE void append(RetroGame* o);
    Q_INVOKABLE void insert(RetroGame* o, int i);
    Q_INVOKABLE QVariant get(int index) const;

    int rowCount(const QModelIndex &p) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    int count() const;
Q_SIGNALS:
    void countChanged(int count);
public Q_SLOTS:
    void setCount(int count);
private:
    int m_count;
    QList<RetroGame*> m_data;
};
