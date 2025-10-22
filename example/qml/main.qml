import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Stream 1.0

Window {
    visible: true
    width: 1600
    height: 1000
    title: "GstQuickRenderer"

    property int viewMode: 0
    property int viewCount: 1
    property int gridColumns: 1
    property int gridRows: 1
    property var fullscreenItem: null
    property var originalWrapper: null
    property var originalParent: null
    property int originalIndex: -1

    function updateLayout(mode) {
        switch (mode) {
        case 0: viewCount = 1; gridColumns = 1; gridRows = 1; break;
        case 1: viewCount = 2; gridColumns = 2; gridRows = 1; break;
        case 2: viewCount = 4; gridColumns = 2; gridRows = 2; break;
        case 3: viewCount = 8; gridColumns = 4; gridRows = 2; break;
        case 4: viewCount = 16; gridColumns = 4; gridRows = 4; break;
        case 5: viewCount = 24; gridColumns = 6; gridRows = 4; break;
        }
    }

    Component.onCompleted: updateLayout(viewModeSelector.currentIndex)

    ColumnLayout {
        anchors.fill: parent
        spacing: 20
        Layout.margins: 20

        ComboBox {
            id: viewModeSelector
            model: ["Single View", "Dual View", "Quad View", "8 View", "16 View", "24 View"]
            currentIndex: 0
            onCurrentIndexChanged: {
                viewMode = currentIndex
                updateLayout(viewMode)
                fullscreenItem = null
                originalWrapper = null
                originalParent = null
                originalIndex = -1
            }
            Layout.fillWidth: true
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Grid {
                id: cameraGrid
                columns: gridColumns
                rows: gridRows
                spacing: 10
                anchors.fill: parent

                Repeater {
                    model: viewCount
                    delegate: Item {
                        id: wrapper
                        width: cameraGrid.width / gridColumns - 10
                        height: cameraGrid.height / gridRows - 10

                        CameraView {
                            id: cam
                            anchors.fill: parent
                            cameraIndex: index

                            onRequestFullscreen: (view, w) => {
                                fullscreenItem = view
                                originalWrapper = w
                                originalParent = w.parent
                                originalIndex = index
                                cam.isFullscreen = true
                                w.parent = fullscreenLayer
                                w.anchors.fill = fullscreenLayer
                                w.z = 999
                            }

                            onExitFullscreen: {
                                cam.isFullscreen = false
                                originalWrapper.parent = originalParent
                                originalWrapper.anchors.fill = undefined
                                originalWrapper.width = cameraGrid.width / gridColumns - 10
                                originalWrapper.height = cameraGrid.height / gridRows - 10
                                originalWrapper.z = 0
                                fullscreenItem = null
                                originalWrapper = null
                                originalParent = null
                                originalIndex = -1
                            }
                        }
                    }
                }
            }

            Item {
                id: fullscreenLayer
                anchors.fill: parent
                z: 998
                visible: fullscreenItem !== null

                Rectangle {
                    anchors.fill: parent
                    color: "black"
                    z: -1
                }
            }
        }
    }
}
