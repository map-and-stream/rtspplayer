import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Stream 1.0

Window {
    visible: true
    width: 1200
    height: 800
    title: "StreamViewPro"

    property int viewMode: 0

    Column {
        spacing: 20
        anchors.fill: parent
        anchors.margins: 20

        ComboBox {
            id: viewModeSelector
            model: ["Single View", "Dual View", "Quad View"]
            currentIndex: 0
            onCurrentIndexChanged: {
                // console.log("Switching view mode to:", currentIndex)
                for (let i = 0; i < cameraGrid.children.length; ++i) {
                    if (cameraGrid.children[i].grabber) {
                        // console.log("Stopping stream for camera", i)
                        cameraGrid.children[i].grabber.stop()
                    }
                }
                viewMode = currentIndex
            }
            width: parent.width
        }

        Grid {
            id: cameraGrid
            columns: viewMode === 0 ? 1 : (viewMode === 1 ? 2 : 2)
            rows: viewMode === 2 ? 2 : 1
            spacing: 10
            width: parent.width
            height: parent.height - viewModeSelector.height - 40

            Repeater {
                model: viewMode === 0 ? 1 : (viewMode === 1 ? 2 : 4)
                delegate: CameraView {
                    id: cameraView
                    cameraIndex: index
                    width: cameraGrid.width / cameraGrid.columns - 10
                    height: cameraGrid.height / cameraGrid.rows - 10
                }
            }
        }
    }
}
