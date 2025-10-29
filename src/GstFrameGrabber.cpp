#include "GstFrameGrabber.h"
#include "VideoSurfaceGL.h"
#include <QMetaObject>
#include <QDebug>

GstFrameGrabber::GstFrameGrabber(QObject *parent)
    : QObject(parent)
{
    gst_init(nullptr, nullptr);
}

GstFrameGrabber::~GstFrameGrabber() {
    stop();
}

void GstFrameGrabber::stop() {
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
        pipeline = nullptr;
        appsink = nullptr;
    }

    if (lastSample) {
        gst_buffer_unmap(gst_sample_get_buffer(lastSample), &lastMap);
        gst_sample_unref(lastSample);
        lastSample = nullptr;
    }
}

bool GstFrameGrabber::start(const QString &rtspUrl) {
    stop(); // Clean previous pipeline

    QString pipelineStr = QString(
                              "rtspsrc location=%1 latency=0 ! "
                              "rtph264depay ! avdec_h264 ! videoconvert ! "
                              "video/x-raw,format=RGB ! appsink name=sink sync=false"
                              ).arg(rtspUrl);

    GError *error = nullptr;
    pipeline = gst_parse_launch(pipelineStr.toUtf8().constData(), &error);
    if (!pipeline || error) {
        qWarning() << "Failed to create pipeline:" << (error ? error->message : "Unknown error");
        return false;
    }

    appsink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    if (!appsink) {
        qWarning() << "appsink not found in pipeline";
        return false;
    }

    gst_app_sink_set_emit_signals(GST_APP_SINK(appsink), true);
    g_signal_connect(appsink, "new-sample", G_CALLBACK(&GstFrameGrabber::onNewSample), this);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    return true;
}

void GstFrameGrabber::setProcessingEnabled(bool enabled) {
    processingEnabled = enabled;
}

RawFrame GstFrameGrabber::grabRawFrame() {
    if (lastSample) {
        gst_buffer_unmap(gst_sample_get_buffer(lastSample), &lastMap);
        gst_sample_unref(lastSample);
        lastSample = nullptr;
    }

    lastSample = gst_app_sink_pull_sample(GST_APP_SINK(appsink));
    if (!lastSample) return {nullptr, 0, 0};

    GstBuffer *buffer = gst_sample_get_buffer(lastSample);
    GstCaps *caps = gst_sample_get_caps(lastSample);
    GstStructure *s = gst_caps_get_structure(caps, 0);

    int width = 0, height = 0;
    gst_structure_get_int(s, "width", &width);
    gst_structure_get_int(s, "height", &height);

    if (!gst_buffer_map(buffer, &lastMap, GST_MAP_READ)) {
        gst_sample_unref(lastSample);
        lastSample = nullptr;
        return {nullptr, 0, 0};
    }

    if (processingEnabled) {
        processRawBuffer(lastMap.data, width, height);
    }

    return {lastMap.data, width, height};
}

void GstFrameGrabber::processRawBuffer(guchar *data, int width, int height)
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

GstFlowReturn GstFrameGrabber::onNewSample(GstAppSink *sink, gpointer user_data) {
    auto *self = static_cast<GstFrameGrabber *>(user_data);
    RawFrame frame = self->grabRawFrame();

    if (frame.data && frame.width > 0 && frame.height > 0) {
        emit self->rawFrameReady(frame.data, frame.width, frame.height);
    }

    return GST_FLOW_OK;
}
