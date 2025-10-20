#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "FrameGrabber.h"
#include "VideoSurface.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<FrameGrabber>("Stream", 1, 0, "FrameGrabber");
    qmlRegisterType<VideoSurface>("Stream", 1, 0, "VideoSurface");



    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
