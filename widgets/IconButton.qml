import QtQuick 2.2
import "../Vars.js" as Vars

Button {
    id:rootItem
    border.color: "darkgrey"
    border.width: 0
//    radius: 5
    property url iconSource: ""
    width: 120
    height: 120
    property real iconMargins:5
    //necessary security level to be visible
    property int securityLevel:-1
    Component.onCompleted: {
    if(Vars.securityLevel<securityLevel)
        rootItem.visible=false
    }

    Image {
        id:icon
        fillMode: Image.PreserveAspectFit
        anchors.fill: parent
        source: iconSource
        anchors.margins:iconMargins
    }
}
