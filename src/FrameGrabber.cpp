#include "FrameGrabber.h"
#include <gst/video/video.h>
#include <QDebug>
#include <QImage>
#include <QSharedPointer>

FrameGrabber::FrameGrabber(QObject *parent)
    : QObject(parent)
{
    gst_init(nullptr, nullptr);
}

void FrameGrabber::start(const QString &uri)
{
    stop();

    pipeline = gst_parse_launch(
        QString("rtspsrc location=%1 ! decodebin ! videoconvert ! video/x-raw,format=RGB ! appsink name=sink")
            .arg(uri).toUtf8().constData(),
        nullptr);

    appsink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    gst_app_sink_set_emit_signals(GST_APP_SINK(appsink), true);
    gst_app_sink_set_max_buffers(GST_APP_SINK(appsink), 1);
    gst_app_sink_set_drop(GST_APP_SINK(appsink), true);

    g_signal_connect(appsink, "new-sample", G_CALLBACK(FrameGrabber::onNewSample), this);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);
}

void FrameGrabber::stop()
{
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
        pipeline = nullptr;
        appsink = nullptr;
    }
}

void FrameGrabber::setProcessingEnabled(bool enabled)
{
    processingEnabled = enabled;
}

bool FrameGrabber::isProcessingEnabled() const
{
    return processingEnabled;
}

GstFlowReturn FrameGrabber::onNewSample(GstAppSink *sink, gpointer user_data)
{
    auto *grabber = static_cast<FrameGrabber *>(user_data);
    GstSample *sample = gst_app_sink_pull_sample(sink);
    if (sample) {
        grabber->handleSample(sample);
        gst_sample_unref(sample);
    }
    return GST_FLOW_OK;
}

void FrameGrabber::handleSample(GstSample *sample)
{
    GstBuffer *buffer = gst_sample_get_buffer(sample);
    GstCaps *caps = gst_sample_get_caps(sample);
    GstStructure *s = gst_caps_get_structure(caps, 0);

    int width = 0, height = 0;
    gst_structure_get_int(s, "width", &width);
    gst_structure_get_int(s, "height", &height);

    const gchar *format = gst_structure_get_string(s, "format");
    if (strcmp(format, "RGB") != 0) {
        qWarning() << "Unsupported format:" << format;
        return;
    }

    GstMapInfo map;
    if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
        if (processingEnabled) {
            processRawBuffer(map.data, width, height);
        }

        QImage img(map.data, width, height, QImage::Format_RGB888);
        auto image = QSharedPointer<QImage>::create(img.copy());

        gst_buffer_unmap(buffer, &map);

        emit frameReady(*image);
    }
}

void FrameGrabber::processRawBuffer(guchar *data, int width, int height)
{
    int boxX = width / 4;
    int boxY = height / 4;
    int boxW = width / 2;
    int boxH = height / 2;
    int thickness = 4;

    // Top border
    for (int y = boxY; y < boxY + thickness; ++y) {
        for (int x = boxX; x < boxX + boxW; ++x) {
            int index = (y * width + x) * 3;
            data[index + 0] = 255;
            data[index + 1] = 0;
            data[index + 2] = 0;
        }
    }

    // Bottom border
    for (int y = boxY + boxH - thickness; y < boxY + boxH; ++y) {
        for (int x = boxX; x < boxX + boxW; ++x) {
            int index = (y * width + x) * 3;
            data[index + 0] = 255;
            data[index + 1] = 0;
            data[index + 2] = 0;
        }
    }

    // Left and right borders
    for (int y = boxY; y < boxY + boxH; ++y) {
        for (int x = boxX; x < boxX + thickness; ++x) {
            int index = (y * width + x) * 3;
            data[index + 0] = 255;
            data[index + 1] = 0;
            data[index + 2] = 0;
        }
        for (int x = boxX + boxW - thickness; x < boxX + boxW; ++x) {
            int index = (y * width + x) * 3;
            data[index + 0] = 255;
            data[index + 1] = 0;
            data[index + 2] = 0;
        }
    }
}
