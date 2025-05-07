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
    
    void paint(QPainter *painter);
    Q_INVOKABLE void updateFrameData(const void *data, unsigned width, unsigned height, size_t pitch, QImage::Format format);
    
    void paint(QPainter *painter) override;

private:
    QImage m_frameBuffer;
    bool m_needsRgbSwap = false;
};