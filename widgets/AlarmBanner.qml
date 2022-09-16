import QtQuick 2.0
import "../Vars.js" as Vars

Rectangle {
    id: rect
    width: 200
    height: 100
    property int registerAddress: -1
    property string label: ""
    property int moduleNumber: parent.moduleNumber ? parent.moduleNumber : (parent.parent.moduleNumber ? parent.parent.moduleNumber : (parent.parent.parent.moduleNumber ? parent.parent.parent.moduleNumber : 0))
    color: "red"
    signal refresh
    opacity: 0.7
    radius: 10

    property string positionIDx: "_x_Alarm_" + moduleNumber + "_M" + registerAddress
    property string positionIDy: "_y_Alarm_" + moduleNumber + "_M" + registerAddress
    property bool created: false
    property bool changePositionEnable:true
    property bool changingPosition: false
    property bool currentObjectChangingPosition: false


    MouseArea{
        enabled: Vars.positionConfiguration && changePositionEnable

        anchors.fill: parent
        drag.target: rect
        drag.axis: Drag.XAndYAxis
    }

    onXChanged: {
        if(created && Vars.positionConfiguration){
            positionIDx="_x_Alarm_" + moduleNumber + "_M" + registerAddress
            Parameters.editEntry(positionIDx, parseInt(x))
            Parameters.saveParameters()
        }
    }

    onYChanged: {
        if(created && Vars.positionConfiguration){
            positionIDy="_y_Alarm_" + moduleNumber + "_M" + registerAddress
            Parameters.editEntry(positionIDy, parseInt(y))
            Parameters.saveParameters()
        }
    }

    Component.onCompleted: {


        PLC.addAlarmEntry(label, registerAddress,moduleNumber)

        created=true


        if(Parameters.getRegisterNum(positionIDx)>0){
            x=Parameters.getRegisterAddress(positionIDx)
        }

        if(Parameters.getRegisterNum(positionIDy)>0){
            y = Parameters.getRegisterAddress(positionIDy)
        }
    }

    visible: false || Vars.alarmVisibility
    onRefresh: {
        if(visible!==PLC.rM(registerAddress))
        visible = PLC.rM(registerAddress) || Vars.alarmVisibility
    }

    Text {
        anchors.centerIn: parent
        text: label
        font.pixelSize: 20
    }

    Connections {
        target: PLC
        onValuesUpdated: refresh()
    }
}
