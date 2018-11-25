/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick          2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts  1.2

import QGroundControl               1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Controls      1.0
import QGroundControl.Palette       1.0
import QGroundControl.Vehicle       1.0
import QGroundControl.FlightMap     1.0

Item {
    property var guidedActionsController

    property real   _margin:        ScreenTools.defaultFontPixelWidth / 2
    property real   _widgetHeight:  ScreenTools.defaultFontPixelHeight * 3
    property color  _textColor:     "black"
    property real   _rectOpacity:   0.8

    QGCPalette { id: qgcPal }

    QGCListView {
        id:                 missionItemEditorListView
        anchors.left:       parent.left
        anchors.right:      parent.right
        anchors.topMargin:  _margin
        anchors.top:        parent.top
        anchors.bottom:     parent.bottom
        spacing:            ScreenTools.defaultFontPixelHeight / 2
        orientation:        ListView.Vertical
        model:              QGroundControl.multiVehicleManager.vehicles
        cacheBuffer:        _cacheBuffer < 0 ? 0 : _cacheBuffer
        clip:               true

        property real _cacheBuffer:     height * 2

        delegate: Rectangle {
            width:      parent.width
            height:     innerColumn.y + innerColumn.height + _margin
            color:      qgcPal.missionItemEditor
            opacity:    _rectOpacity
            radius:     _margin

            property var    _vehicle:   object

            ColumnLayout {
                id:                 innerColumn
                width:              parent.width
                anchors.margins:    _margin
                anchors.top:        parent.top
                anchors.left:       parent.left
                anchors.right:      parent.left
                spacing:            _margin
                Layout.fillWidth:       true

                RowLayout {
                    Layout.fillWidth:       true

                    QGCLabel {
                        Layout.alignment:   Qt.AlignTop
                        text:               "Vehicle " + _vehicle.id
                        color:              _textColor
                    }

                } // RowLayout

                Grid {
                    width: parent.width;
                    columns: 2
                    spacing: ScreenTools.defaultFontPixelWidth

                    QGCButton {
                        text:       "Upload mission"
                        onClicked:  _vehicle.armed = true
                        width: 150
                    }

                    QGCButton {
                        text:       "Upload geofence"
                        width: 150
                        onClicked:  _vehicle.armed = false
                    }

                    Timer {
                        id: delayClear
                        interval: 2000; running: false; repeat: false
                        onTriggered: {
                            console.log(QGroundControl.multiVehicleManager.activeVehicle.id)
                            masterController.removeAllFromVehicle()
                        }
                    }

                    Timer {
                        id: delayUpload
                        interval: 2000; running: false; repeat: false
                        onTriggered: {
                            console.log(QGroundControl.multiVehicleManager.activeVehicle.id)
                            masterController.upload()
                        }
                    }

                    QGCButton {
                        text:               qsTr("Clear Mission")
                        Layout.fillWidth:   true
                        Layout.columnSpan:  2
                        enabled:            !masterController.offline && !masterController.syncInProgress
                        visible:            !QGroundControl.corePlugin.options.disableVehicleConnection
                        onClicked: {
                            console.log("clear clicked")
                            console.log(_vehicle.id)
                            console.log(QGroundControl.multiVehicleManager.activeVehicle.id)
                            if (delayClear.running) {
                                console.error("nested calls to setTimeout are not supported!");
                                return;
                            }
                            QGroundControl.multiVehicleManager.activeVehicle = _vehicle
                            delayClear.start();

                        }
                    }

                    QGCButton {
                        text:               qsTr("Upload")
                        Layout.fillWidth:   true
                        enabled:            !masterController.offline && !masterController.syncInProgress && _visualItems.count > 1
                        visible:            !QGroundControl.corePlugin.options.disableVehicleConnection
                        onClicked: {
                            console.log("upload clicked")
                            console.log(_vehicle.id)
                            console.log(QGroundControl.multiVehicleManager.activeVehicle.id)
                            if (delayUpload.running) {
                                console.error("nested calls to setTimeout are not supported!");
                                return;
                            }
                            QGroundControl.multiVehicleManager.activeVehicle = _vehicle
                            delayUpload.start();
                        }
                    }

                } // Grid
            } // ColumnLayout
        } // delegate - Rectangle
    } // QGCListView
} // Item
