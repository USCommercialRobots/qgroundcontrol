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
                        text:               _vehicle.id
                        color:              _textColor
                    }

                    ColumnLayout {
                        Layout.alignment:   Qt.AlignCenter
                        spacing:            _margin

                        QGCLabel {
                            Layout.alignment:           Qt.AlignHCenter
                            text:                       _vehicle.flightMode
                            color:                      _textColor
                        }

                        QGCLabel {
                            Layout.alignment:           Qt.AlignHCenter
                            text:                       _vehicle.armed ? qsTr("Armed") : qsTr("Disarmed")
                            color:                      _textColor
                        }

                        QGCLabel {
                            Layout.alignment:           Qt.AlignHCenter
                            text:                       "Speed: " + _vehicle.getFact("groundSpeed").valueString + _vehicle.getFact("groundSpeed").units
                            color:                      _textColor
                        }

                        QGCLabel {
                            Layout.alignment:           Qt.AlignHCenter
                            text:                       "Flight time: " + _vehicle.getFact("flightTime").valueString
                            color:                      _textColor
                        }
                    }

                    QGCCompassWidget {
                        size:       _widgetHeight
                        vehicle:    _vehicle
                        anchors.top:        parent.top
                        anchors.right:       altitudeWidget.left
                    }

                    QGCAttitudeWidget {
                        id:     altitudeWidget
                        size:       _widgetHeight
                        vehicle:    _vehicle
                        anchors.top:        parent.top
                        anchors.right:      parent.right
                    }
                } // RowLayout

                Grid {
                    width: parent.width;
                    columns: 2
                    spacing: ScreenTools.defaultFontPixelWidth

                    QGCButton {
                        text:       "Arm"
                        onClicked:  _vehicle.armed = true
                        width: 150
                    }

                    QGCButton {
                        text:       "Disarm"
                        width: 150
                        onClicked:  _vehicle.armed = false
                    }

                    QGCButton {
                        text:       "Start Mission"
                        width: 150
                        onClicked:  _vehicle.startMission()
                    }

                    QGCButton {
                        text:       "Pause"
                        width: 150
                        visible:    _vehicle.armed && _vehicle.pauseVehicleSupported
                        onClicked:  _vehicle.pauseVehicle()
                    }

                    QGCButton {
                        text:       "RTL"
                        width: 150
                        visible:    _vehicle.armed && _vehicle.flightMode !== _vehicle.rtlFlightMode
                        onClicked:  _vehicle.flightMode = _vehicle.rtlFlightMode
                    }

                    QGCButton {
                        width: 150
                        text:       "Take control"
                        visible:    _vehicle.armed && _vehicle.flightMode !== _vehicle.takeControlFlightMode
                        onClicked:  _vehicle.flightMode = _vehicle.takeControlFlightMode
                    }
                } // Grid
            } // ColumnLayout
        } // delegate - Rectangle
    } // QGCListView
} // Item
