import QtQuick 2.10
import QtQuick.Window 2.10
import "widgets"
import "Vars.js" as Vars

Window {
    visible: true
    width: 1280
    height: 1024

    //visibility: "FullScreen"
    id: root

    Connections {
        target: Qt.application
        onStateChanged: {
            switch (Qt.application.state) {
            case Qt.ApplicationInactive:
                PLC.stopThread()
                break
            case Qt.ApplicationActive:
                PLC.startThread()
                break
            }
        }
    }

    Item {
        width: root.width
        height: root.height
        anchors.centerIn: parent
        rotation: 0

        Loader {
            id: pageLoader
            anchors.fill: parent
            source: "screens/MainScreen.qml"
        }

        Timer {
            id: mainTimer
            interval: 100
            repeat: true
            running: true
            onTriggered: {
                textEditMode.visible = Vars.editMode
                keypad1.visible = Vars.numpadVisible
                keypadEditMode1.visible = Vars.editPadVisible
                //Vars.securityLevel = PLC.RD(2188)
                if (Vars.securityLevel < 0 && Vars.currentScreen != 6)
                    Vars.currentScreen = 10

                if (PLC.rM(2020))
                    Vars.currentScreen = 12

                if (PLC.rM(2003))
                    Vars.currentScreen = 17

                //if (PLC.RM(2287) && Vars.securityLevel >= 0)
                //    Vars.currentScreen = 7
                if (Vars.securityLevel >= 0 && Vars.currentScreen == 10)
                    Vars.currentScreen = 0

                if (PLC.rM(2289) && 0) {
                    // var readResult = WitteJobProcessing.readJob(
                    //            PLC.getBarcodeText())
                    //if (readResult > 0) {
                    CERM.setFilePath(PLC.getBarcodeText())
                    CERM.readCuttingLines()
                    Vars.currentScreen = 15

                    // }
                    PLC.m(2289, 0)
                }

                plcConnectionAlarm.visible = !PLC.isOpen()

                switch (Vars.currentScreen) {
                case 0:
                    pageLoader.source = "screens/MainScreen.qml"
                    break

                default:
                    pageLoader.source = "screens/MainScreen.qml"
                }
            }
        }

        Timer {
            //JMF timer
            interval: 30000
            running: true
            repeat: true
            onTriggered: {
                if (PLC.rL(2081)) {
                    CERM.postJMF()
                }
            }
        }

        Keypad {
            visible: false
            id: keypad1
            anchors.bottom: parent.bottom
        }

        KeypadEditMode {
            visible: false
            id: keypadEditMode1
            anchors.bottom: parent.bottom
        }

        Text {
            visible: false
            id: textEditMode
            color: "red"
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Edit Mode"
            font.pixelSize: 20
        }

        Rectangle {
            id: plcConnectionAlarm
            height: 100
            width: 300
            anchors.centerIn: parent
            color: "darkRed"
            opacity: 0.7
            radius: 5
            visible: true

            Text {
                anchors.centerIn: parent
                text: qsTr("PLC connection lost")
                font.pixelSize: 20
            }
        }

        Text {
            id: recipeText
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 16
            font.bold: true
            color: "red"
            signal refresh
            onRefresh: {
                if (Vars.recipesRegisterMode) {
                    text = Vars.recipeStatusText
                    visible = true
                } else {
                    text = ""
                    visible = false
                }
            }

            Connections {

                target: PLC
                onValuesUpdated: recipeText.refresh()
            }
        }
    }
}
