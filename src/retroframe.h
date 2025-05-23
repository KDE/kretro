/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2023-2025 Seshan Ravikumar <seshan@sineware.ca>
*/

#pragma once
#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>
#include <QImage>
#include <qqmlintegration.h>

class RetroFrame : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT

public:
    RetroFrame(QQuickItem *parent = nullptr);
    
    void paint(QPainter *painter) override;
    Q_INVOKABLE void updateFrameData(const void *data, unsigned width, unsigned height, size_t pitch, QImage::Format format);
    

private:
    QImage m_frameBuffer;
    bool m_needsRgbSwap = false;
};