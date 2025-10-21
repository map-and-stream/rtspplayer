#include "VideoSurface.h"

VideoSurface::VideoSurface(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
    setAntialiasing(true);
}

QImage VideoSurface::frame() const
{
    return m_frame;
}

void VideoSurface::setFrame(const QImage &img)
{
    if (m_frame != img) {
        m_frame = img;
        update();
        emit frameChanged();
    }
}

void VideoSurface::paint(QPainter *painter)
{
    if (!m_frame.isNull()) {
        painter->drawImage(boundingRect(), m_frame);
    }
}
