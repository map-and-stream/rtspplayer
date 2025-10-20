#pragma once

#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>

class VideoSurface : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QImage frame READ frame WRITE setFrame NOTIFY frameChanged)

public:
    explicit VideoSurface(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

    QImage frame() const;
    void setFrame(const QImage &img);

signals:
    void frameChanged();

private:
    QImage m_frame;
};
