#pragma once

#include <QQuickFramebufferObject>
#include <QtGlobal> //  uchar decl

class VideoSurfaceGL : public QQuickFramebufferObject
{
    Q_OBJECT
public:
    explicit VideoSurfaceGL(QQuickItem *parent = nullptr);
    Renderer *createRenderer() const override;

    Q_INVOKABLE void setRawFrame(uchar *data, int w, int h);

    uchar *rawData = nullptr;
    int rawWidth = 0;
    int rawHeight = 0;
};
