// RetroFrame.h
#ifndef RETROFRAME_H
#define RETROFRAME_H
#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>
#include <QImage>

class RetroFrame : public QQuickPaintedItem
{
Q_OBJECT
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
public:
    RetroFrame(QQuickItem *parent = nullptr);
    Q_INVOKABLE void setImage(const QImage &image);
    void paint(QPainter *painter);
    QImage image() const;
signals:
    void imageChanged();
private:
    QImage m_currentImage;
};
#endif // RETROFRAME_H
