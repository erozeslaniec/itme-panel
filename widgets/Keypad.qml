import QtQuick 2.2
import "../Vars.js" as Vars
import "../widgets"

Rectangle {
    width: 450
    height: 480
    id: keyboard
    border.width: 1
    border.color: "grey"
    color: "lightGrey"
    MouseArea {
        anchors.fill: parent
    }
    property string keys: "789x456<123C,0-E"
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
        Vars.numpadVisible = false
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
        Text {
            id: text2
            text: "min= " + (Vars.numpadMinimum / Math.pow(
                      10,
                      Vars.keypadDecimalPlaces)).toFixed(Vars.keypadDecimalPlaces) + "\nmax= " + (Vars.numpadMaximum / Math.pow(
                      10, Vars.keypadDecimalPlaces)).toFixed(Vars.keypadDecimalPlaces)
            signal refresh2
            onRefresh2: {

                text = "min= " + (Vars.numpadMinimum / Math.pow(
                            10,
                            Vars.keypadDecimalPlaces)).toFixed(Vars.keypadDecimalPlaces) + "\nmax= " + (Vars.numpadMaximum / Math.pow(
                            10, Vars.keypadDecimalPlaces)).toFixed(Vars.keypadDecimalPlaces)
            }
        }
    }
    onVisibleChanged: {
        keyboard.value = 0
        integerPart = 0
        decimalPart = 0
        decimalPartStr = ""
        decimalSelected = false
        numberSign = 1
        text2.refresh2()
    }
    Rectangle {
        id: rectangle1
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 5
        width: 100
        height: 50
        color: "#ffffff"
        visible: true
        Text {
            id: text1
            text: keyboard.value
            anchors.centerIn: parent
            signal refresh
            onRefresh: {
                var numberOfPlaces = decimalPartStr.length
                keyboard.value = (integerPart + decimalPart / Math.pow(
                                      10, numberOfPlaces)) * numberSign
                text = keyboard.value
            }
        }
    }

    Grid {

        id: grid1
        anchors.top: rectangle1.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        anchors.horizontalCenterOffset: 0
        anchors.topMargin: 5
        rows: 6
        columns: 4
        spacing: 1

        Repeater {
            id: repeater1
            model: keys.length
            StateButton {
                width: 100
                height: 100
                iconMargins: 20
                onClicked: {

                    if (Vars.numpadReset == true) {
                        keyboard.value = 0
                        text1.refresh()
                        Vars.numpadReset = false
                    }

                    // else{
                    //clear
                    switch (index) {
                    case 7:
                        if (!decimalSelected)
                            integerPart = integerPart / 10
                        else {
                            if (decimalPart == 0)
                                decimalSelected = false
                            else
                                decimalPart = decimalPart / 10
                        }

                        text1.refresh()
                        break
                    case 11:
                        keyboard.value = 0
                        integerPart = 0
                        decimalPart = 0
                        decimalPartStr = ""
                        decimalSelected = false
                        text1.refresh()
                        break
                    case 3:
                        hideKeyboard()
                        break
                    case 12:
                        decimalSelected = true
                        break
                    case 15:
                        if (keyboard.value * Math.pow(
                                    10,
                                    Vars.keypadDecimalPlaces) > Vars.numpadMaximum) {

                        } else if (keyboard.value * Math.pow(
                                       10,
                                       Vars.keypadDecimalPlaces) < Vars.numpadMinimum) {

                        } else {
                            if (!Vars.numpadDoubleWord)
                                PLC.d(Vars.numpadAddress, value * Math.pow(
                                          10, Vars.keypadDecimalPlaces)*Vars.currentScaler)
                            if (Vars.numpadDoubleWord)
                                PLC.dW(Vars.numpadAddress, value * Math.pow(
                                           10, Vars.keypadDecimalPlaces)*Vars.currentScaler)

                            hideKeyboard()
                        }
                        break
                    case 14:
                        numberSign = numberSign * (-1)
                        text1.refresh()
                        break
                    default:
                        if (!decimalSelected)
                            integerPart = integerPart + keys.charAt(
                                        index).valueOf()
                        else {
                            decimalPart = decimalPart + keys.charAt(
                                        index).valueOf()
                            decimalPartStr = decimalPartStr + keys.charAt(
                                        index).valueOf()
                        }

                        text1.refresh()
                        break
                    }
                }

                label: {
                    switch (index) {
                    case 3:
                        return ""
                    case 7:
                        return ""
                    case 12:
                        return ","
                    case 11:
                        return "CLR"
                    case 15:
                        return ""
                    default:
                        keys.charAt(index)
                    }
                }
                iconSource: {
                    switch (index) {
                    case 7:
                        return Qt.resolvedUrl("../icons/delete-1.svg")
                    case 3:
                        return Qt.resolvedUrl("../icons/delete.svg")
                    case 15:
                        return Qt.resolvedUrl("../icons/arrows-1.svg")
                    default:
                        return ""
                    }
                }
            }
        }
    }
}
