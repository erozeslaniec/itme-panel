import QtQuick 2.0

Rectangle {
    id: rootItem
    property int diameter: 20
    property int registerAddress: -1
    property bool latching: false
    property color colorOff: "darkGrey"
    property color colorOn: "green"

    property bool currentState: false
    property string label: ""
    property int textSize: 14
    color: colorOff

    width: diameter
    height: diameter
    radius: diameter / 2

    signal refresh

    onRefresh: {
        rootItem.color = currentState ? colorOn : colorOff
    }

    Connections {
        target: PLC
        onValuesUpdated: {
            if (latching)
                currentState = PLC.rL(registerAddress)
            else
                currentState = PLC.rM(registerAddress)

            refresh()
        }
    }

    Text {
        anchors.centerIn: parent
        text: label
        font.pointSize: textSize
        horizontalAlignment: Text.AlignHCenter
    }


    Behavior on color {
        ColorAnimation {
            duration: 100
        }
    }
}
