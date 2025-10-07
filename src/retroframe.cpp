/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2023-2025 Seshan Ravikumar <seshan@sineware.ca>
*/

#include "retroframe.h"
#include "app.h"

RetroFrame::RetroFrame(QQuickItem *parent)
  : QQuickPaintedItem{parent}
{
    App::self()->setRetroFrame(this);
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
    setFillColor(Qt::black);
    
    m_frameBuffer = QImage(1, 1, QImage::Format_RGB32);
    m_frameBuffer.fill(Qt::black);
}

void RetroFrame::updateFrameData(const void *data, unsigned width, unsigned height, size_t pitch, QImage::Format format)
{
    if (!data) {
        return;
    }

    if (m_frameBuffer.size() != QSize(width, height) || m_frameBuffer.format() != format) {
        m_frameBuffer = QImage(width, height, format);
        m_needsRgbSwap = (format == QImage::Format_RGBX8888);
    }
    
    uchar *dst = m_frameBuffer.bits();
    const uchar *src = static_cast<const uchar *>(data);

    int bytesPerLine = m_frameBuffer.bytesPerLine();
    int copyWidth = qMin<int>(bytesPerLine, pitch);

    for (unsigned y = 0; y < height; ++y) {
        memcpy(dst + y * bytesPerLine, src + y * pitch, copyWidth);
    }
    
    // Because this method is called from a non-GUI thread (libretro core thread),
    // we need to schedule the update on the GUI thread.
    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

void RetroFrame::paint(QPainter *painter)
{
    if (m_frameBuffer.isNull()) {
        return;
    }
    
    const QRectF boundingRect = this->boundingRect();
    
    const QImage displayImage = m_needsRgbSwap ? m_frameBuffer.rgbSwapped() : m_frameBuffer;
    
    const QSizeF sourceSize = displayImage.size();
    const QSizeF targetSize = sourceSize.scaled(boundingRect.size(), Qt::KeepAspectRatio);
    
    const QPointF position(
        (boundingRect.width() - targetSize.width()) / 2.0,
        (boundingRect.height() - targetSize.height()) / 2.0
    );
    
    painter->drawImage(
        QRectF(position, targetSize),
        displayImage,
        QRectF(0, 0, displayImage.width(), displayImage.height())
    );
}

#include "moc_retroframe.cpp"
