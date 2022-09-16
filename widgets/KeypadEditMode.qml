import QtQuick 2.2
import "../Vars.js" as Vars
import "../widgets"

Rectangle {
    width: 250
    height: 480
    id: keyboard
    border.width: 1
    border.color: "grey"
    color: "lightGrey"
    MouseArea {
        anchors.fill: parent
    }
    property string keys: "MLDIxE7894561230C"
    signal hideKeyboard
    signal letterClicked(string letter)
    property int number: 30
    property int decimalPart: 0
    property int integerPart: 0
    property bool decimalSelected: false
    property string decimalPartStr: "0"
    property int numberSign: 1
    onHideKeyboard: {
        visible = false
    }

    property int registerAddress: -1

    property real value: 0

    Rectangle {
        id: rectangle2
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 0
        width: 0
        height: 0
        color: "#ffffff"
        visible: true
    }
    onVisibleChanged: {
        keyboard.value = 0
        integerPart = 0
        decimalPart = 0
        decimalPartStr = ""
        decimalSelected = false
        numberSign = 1
        text1.refresh()
        idText.text=Vars.currentRegisterID
    }
    Rectangle {
        id: rectangle1
        anchors.bottom: grid1.top
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        width: 100
        height: 50
        color: "#ffffff"
        visible: true
        Text {
            id: text1
            text: ""
            anchors.centerIn: parent
            signal refresh
            onRefresh: {
                if (Vars.currentRegisterType < 2)Vars.currentRegisterType = 2
                text = keys.charAt(Vars.currentRegisterType - 2) +Vars.currentRegisterAddress
            }
        }
    }

    Text{
        id:idText
        anchors.left:parent.left
        anchors.bottom:parent.bottom
        anchors.leftMargin: 5
        anchors.bottomMargin: 5
        text:Vars.currentRegisterID

    }

    Grid {

        id: grid1
        anchors.bottom: idText.top
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        rows: 6
        columns: 3
        spacing: 1

        Repeater {
            id: repeater1
            model: keys.length
            Button {
                checkable: true
                width: 65
                height: 65
                signal refresh

                Timer {
                    running: true
                    repeat: true
                    interval: 1000
                    onTriggered: checked =Vars.currentRegisterType == index + 2

                }

                onClicked: {
                    switch (index) {
                    case 0:
                        Vars.currentRegisterType = 2
                        break
                    case 1:
                        Vars.currentRegisterType = 3
                        break
                    case 2:
                        Vars.currentRegisterType = 4
                        break
                    case 3:
                        Vars.currentRegisterType = 5
                        break
                    case 4:
                        keyboard.visible = false
                        Vars.editPadVisible=false
                        break
                    case 5:
                        keyboard.visible = false
                        Vars.editPadVisible=false
                        Parameters.editEntry(Vars.currentRegisterID,Vars.currentRegisterAddress.valueOf(),Vars.currentRegisterType)
                        Parameters.saveParameters()
                        break
                    case 16:
                        Vars.currentRegisterAddress=""
                        break

                    default:
                        Vars.currentRegisterAddress = Vars.currentRegisterAddress + text
                        if(Vars.currentRegisterAddress.valueOf()>65536)Vars.currentRegisterAddress="65536"
                        if(Vars.currentRegisterAddress.valueOf()<0)Vars.currentRegisterAddress="0"

                    }
                    text1.refresh()
                }

                text: {
                    switch (index) {
                    case 3:
                        "DWORD"
                        break
                    default:
                        keys.charAt(index)
                        break
                    }
                }
            }
        }
    }
}
