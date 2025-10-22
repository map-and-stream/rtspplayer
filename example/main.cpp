#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "VideoSurfaceGL.h"
#include "GstFrameGrabber.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<VideoSurfaceGL>("Stream", 1, 0, "VideoSurfaceGL");
    qmlRegisterType<GstFrameGrabber>("Stream", 1, 0, "GstFrameGrabber");

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/qml/main.qml"_qs);
    engine.load(url);

    if (engine.rootObjects().isEmpty()) {
        qWarning() << "QML engine failed to load root object!";
        return -1;
    }

    return app.exec();
}
