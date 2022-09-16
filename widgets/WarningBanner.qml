import QtQuick 2.0
import "../Vars.js" as Vars


Rectangle {
    width: 200
    height: 100
    property int registerAddress: -1
    property string label: ""
    color: "orange"
    signal refresh
    opacity: 0.7
    radius: 10
    visible: true || Vars.alarmVisibility
    property int moduleNumber: parent.moduleNumber ? parent.moduleNumber : (parent.parent.moduleNumber ? parent.parent.moduleNumber : (parent.parent.parent.moduleNumber ? parent.parent.parent.moduleNumber : 0))


    property string positionIDx: "_x_Warning_" + moduleNumber + "_M" + registerAddress
    property string positionIDy: "_y_Warning_" + moduleNumber + "_M" + registerAddress
    property bool created: false
    property bool changePositionEnable:true
    property bool changingPosition: false
    property bool currentObjectChangingPosition: false



    onRefresh: {
        visible = PLC.rM(registerAddress) || Vars.alarmVisibility
    }

    MouseArea{
        enabled: Vars.positionConfiguration && changePositionEnable

        anchors.fill: parent
        drag.target: parent
        drag.axis: Drag.XAndYAxis

    }

    onXChanged: {
        if(created && Vars.positionConfiguration){
            positionIDx="_x_Warning_" + moduleNumber + "_M" + registerAddress
            Parameters.editEntry(positionIDx, parseInt(x))
            Parameters.saveParameters()
        }
    }

    onYChanged: {
        if(created && Vars.positionConfiguration){
            positionIDy="_y_Warning_" + moduleNumber + "_M" + registerAddress
            Parameters.editEntry(positionIDy, parseInt(y))
            Parameters.saveParameters()
        }
    }

    Component.onCompleted: {
        created=true

        if(Parameters.getRegisterNum(positionIDx)>0){
            x=Parameters.getRegisterAddress(positionIDx)
        }

        if(Parameters.getRegisterNum(positionIDy)>0){
            y = Parameters.getRegisterAddress(positionIDy)
        }
    }

    Text {
        anchors.centerIn: parent
        text: label
        font.pixelSize: 15
    }

    Connections {
        target: PLC
        onValuesUpdated: refresh()
    }
}
