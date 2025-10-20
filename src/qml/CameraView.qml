import QtQuick 2.15
import QtQuick.Controls 2.15
import Stream 1.0

Item {
    id: root
    property int cameraIndex: 0

    FrameGrabber {
        id: grabber
    }

    Connections {
        target: grabber
        function onFrameReady(frame) {
            //console.log("Camera", cameraIndex, "received frame")
            if (video) {
                video.frame = frame
            } else {
                // console.warn("VideoSurface not available for camera", cameraIndex)
            }
        }
    }

    Component.onDestruction: {
        // console.log("Camera", cameraIndex, "destroyed, stopping stream")
        grabber.stop()
    }

    Column {
        spacing: 8
        anchors.fill: parent

        TextField {
            id: urlField
            placeholderText: "RTSP URL for Camera " + (cameraIndex + 1)
            text: "rtsp://127.0.0.1:8550/test1"
            width: parent.width
        }

        Row {
            spacing: 10
            width: parent.width

            Button {
                text: "Start"
                onClicked: {
                    //console.log("Camera", cameraIndex, "starting stream:", urlField.text)
                    grabber.start(urlField.text)
                }
            }

            Button {
                text: "Stop"
                onClicked: {
                    //console.log("Camera", cameraIndex, "stopping stream")
                    grabber.stop()
                }
            }

            CheckBox {
                id: processingToggle
                text: "Enable Processing"
                checked: false
                onCheckedChanged: {
                    //console.log("Camera", cameraIndex, "processing enabled:", checked)
                    grabber.setProcessingEnabled(checked)
                }
            }
        }

        VideoSurface {
            id: video
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: parent.height - urlField.height - 60
        }
    }
}
