#pragma once

#include <QObject>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <QImage>
#include <QSharedPointer>

class FrameGrabber : public QObject
{
    Q_OBJECT

public:
    explicit FrameGrabber(QObject *parent = nullptr);


    Q_INVOKABLE void start(const QString &uri);
    Q_INVOKABLE void stop();
    Q_INVOKABLE void setProcessingEnabled(bool enabled);
    Q_INVOKABLE bool isProcessingEnabled() const;

signals:

    void frameReady(const QImage &frame);
    // void frameReady(QSharedPointer<QImage> frame);//Normal Mode

private:
    GstElement *pipeline = nullptr;
    GstElement *appsink = nullptr;

    bool processingEnabled = false;

    static GstFlowReturn onNewSample(GstAppSink *sink, gpointer user_data);
    void handleSample(GstSample *sample);
    void processRawBuffer(guchar *data, int width, int height);
};



