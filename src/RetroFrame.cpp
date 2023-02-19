#include "RetroFrame.h"
#include "app.h"

RetroFrame::RetroFrame(QQuickItem *parent)
  : QQuickPaintedItem{parent}
  , m_currentImage{":/images/no_image.png"}
{
    App::self()->setRetroFrame(this);
}

void RetroFrame::paint(QPainter *painter)
{
    QRectF bounding_rect = boundingRect();
    QImage scaled = m_currentImage.scaledToHeight(bounding_rect.height());
    QPointF center = bounding_rect.center() - scaled.rect().center();

    if(center.x() < 0)
        center.setX(0);
    if(center.y() < 0)
        center.setY(0);
   painter->drawImage(center, scaled);
}

QImage RetroFrame::image() const
{
    return m_currentImage;
}

void RetroFrame::setImage(const QImage &image)
{
    m_currentImage = image;
    update();
}
