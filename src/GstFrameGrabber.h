#pragma once

#include <QObject>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <QtGlobal>
#include <QString>

struct RawFrame {
    uchar* data;
    int width;
    int height;
};

class GstFrameGrabber : public QObject
{
    Q_OBJECT
public:
    explicit GstFrameGrabber(QObject *parent = nullptr);
    ~GstFrameGrabber();

    Q_INVOKABLE bool start(const QString &rtspUrl);
    Q_INVOKABLE void stop();
    Q_INVOKABLE void setProcessingEnabled(bool enabled);

signals:
    void rawFrameReady(uchar *data, int width, int height);

private:
    GstElement *pipeline = nullptr;
    GstElement *appsink = nullptr;
    GstMapInfo lastMap;
    GstSample *lastSample = nullptr;
    bool processingEnabled = false;

    RawFrame grabRawFrame();
    static GstFlowReturn onNewSample(GstAppSink *sink, gpointer user_data);
    void processRawBuffer(guchar *data, int width, int height);
};
