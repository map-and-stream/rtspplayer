import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Stream 1.0

Item {
    id: root
    property int cameraIndex: 0
    property bool isFullscreen: false
    property bool isStreaming: false
    property string streamUrl: ""

    signal requestFullscreen(CameraView view, Item wrapper)
    signal exitFullscreen()

    GstFrameGrabber {
        id: grabber
    }

    Connections {
        target: grabber
        function onRawFrameReady(data, width, height) {
            videoSurface.setRawFrame(data, width, height)
        }
    }

    VideoSurfaceGL {
        id: videoSurface
        anchors.fill: parent
        z: -1
    }

    Item {
        id: controlPanel
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 100
        z: 10
        visible: !isFullscreen

        ColumnLayout {
            anchors.fill: parent
            spacing: 8

            TextField {
                id: urlField
                placeholderText: "RTSP URL for Camera " + (cameraIndex + 1)
                text: "rtsp://127.0.0.1:8550/test1"
                Layout.fillWidth: true
                focus: true
                z: 10
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Button {
                    text: "Start"
                    z: 10
                    onClicked: {
                        grabber.start(urlField.text)
                        streamUrl = urlField.text
                        isStreaming = true
                    }
                }

                Button {
                    text: "Stop"
                    z: 10
                    onClicked: {
                        grabber.stop()
                        isStreaming = false
                    }
                }

                CheckBox {
                    text: "Enable Processing"
                    checked: false
                    z: 10
                    onCheckedChanged: grabber.setProcessingEnabled(checked)
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        z: 5
        propagateComposedEvents: true
        onDoubleClicked: {
            if (isStreaming && !isFullscreen) {
                requestFullscreen(root, root.parent)
            } else if (isFullscreen) {
                exitFullscreen()
            }
        }
    }

    Button {
        text: "Exit Fullscreen"
        visible: isFullscreen
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 20
        z: 20
        onClicked: {
            exitFullscreen()
        }
    }
}
