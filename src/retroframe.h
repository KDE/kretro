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
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
public:
    RetroFrame(QQuickItem *parent = nullptr);
    Q_INVOKABLE void setImage(const QImage &image);
    void paint(QPainter *painter);
    QImage image() const;
Q_SIGNALS:
    void imageChanged();
private:
    QImage m_currentImage;
};

