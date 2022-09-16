import QtQuick 2.2

Rectangle {
    id: button
    property bool checkable: false
    property bool checked: false
    property string text: ""
    property alias fontSize: label.font.pointSize
    property color colorOff: "#c0c0c0"
    property color colorOn: "#c0c0c0"
    property alias buttonPressed:mouseArea.pressed
    color: colorOff
    width: 120
    height: 120
    signal clicked
    signal pressed
    signal released

//    border.width: 1
//    border.color: "grey"
    state: "buttonOff"

    Text {
        id: label
        text: parent.text
        anchors.centerIn: parent
    }

    MouseArea {
        id:mouseArea
        anchors.fill: parent
        //onClicked: parent.clicked()
        onPressed: {
            button.pressed()
            button.scale = 0.9
            parent.clicked()
        }
        onReleased: {
            button.released()
            button.scale = 1.0
        }
    }

    Behavior on color {
        ColorAnimation {
            duration: 100
        }
    }

    Behavior on scale{
        NumberAnimation{
            duration: 100
        }
    }
}
