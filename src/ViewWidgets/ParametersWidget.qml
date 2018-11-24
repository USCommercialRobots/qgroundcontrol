/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick                  2.3
import QtQuick.Controls         1.2
import QtQuick.Controls.Styles  1.4
import QtQuick.Dialogs          1.2
import QtLocation               5.3
import QtPositioning            5.3
import QtQuick.Layouts          1.2
import QtQuick.Window           2.2
import QtQml.Models             2.1

import QGroundControl               1.0
import QGroundControl.Airspace      1.0
import QGroundControl.Controllers   1.0
import QGroundControl.Controls      1.0
import QGroundControl.FactSystem    1.0
import QGroundControl.FlightDisplay 1.0
import QGroundControl.FlightMap     1.0
import QGroundControl.Palette       1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Vehicle       1.0

QGCView {
    id:         root

    QGCPalette { id: qgcPal; colorGroupEnabled: enabled }

    property alias  guidedController:   guidedActionsController

    property var    _planMasterController:  masterController
    property var    _missionController:     _planMasterController.missionController
    property real   _margins:               ScreenTools.defaultFontPixelWidth / 2
    property alias  _guidedController:      guidedActionsController
    property alias  _altitudeSlider:        altitudeSlider

    Text {
        id: asd
        text: qsTr("aee")
    }
    MultiVehicleList {
        anchors.margins:            _margins
        anchors.top:                root.top
        anchors.left:               root.left
        width:                      ScreenTools.defaultFontPixelWidth * 30
        z:                          1
        guidedActionsController:    _guidedController
    }

    GuidedActionsController {
        id:                 guidedActionsController
        missionController:  _missionController
        confirmDialog:      guidedActionConfirm
        actionList:         guidedActionList
        altitudeSlider:     _altitudeSlider
        z:                  root.z + 5

        onShowStartMissionChanged: {
            if (showStartMission) {
                confirmAction(actionStartMission)
            }
        }

        onShowContinueMissionChanged: {
            if (showContinueMission) {
                confirmAction(actionContinueMission)
            }
        }

        onShowLandAbortChanged: {
            if (showLandAbort) {
                confirmAction(actionLandAbort)
            }
        }

        /// Close all dialogs
        function closeAll() {
            mainWindow.enableToolbar()
            rootLoader.sourceComponent  = null
            guidedActionConfirm.visible = false
            guidedActionList.visible    = false
            altitudeSlider.visible      = false
        }
    }

    GuidedActionConfirm {
        id:                         guidedActionConfirm
        anchors.margins:            _margins
        anchors.bottom:             parent.bottom
        anchors.horizontalCenter:   parent.horizontalCenter
        guidedController:           _guidedController
        altitudeSlider:             _altitudeSlider
    }

    GuidedActionList {
        id:                         guidedActionList
        anchors.margins:            _margins
        anchors.bottom:             parent.bottom
        anchors.horizontalCenter:   parent.horizontalCenter
        guidedController:           _guidedController
    }

    //-- Altitude slider
    GuidedAltitudeSlider {
        id:                 altitudeSlider
        anchors.margins:    _margins
        anchors.right:      parent.right
        anchors.topMargin:  ScreenTools.toolbarHeight + _margins
        anchors.top:        parent.top
        anchors.bottom:     parent.bottom
        z:                  _guidedController.z
        radius:             ScreenTools.defaultFontPixelWidth / 2
        width:              ScreenTools.defaultFontPixelWidth * 10
        color:              qgcPal.window
        visible:            false
    }

    PlanMasterController {
        id:                     masterController
        Component.onCompleted:  start(true /* flyView */)
    }
}
