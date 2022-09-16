import QtQuick 2.2
import "../Vars.js" as Vars

IconButton {
    id: button

    property int mode: 0 //0-alternate, 1- momentary, 2 set, 3 reset
    property bool currentState: false
    property bool prevState: currentState

    property string label: ""

//    property int radius: 1

//    border.color: "darkgrey"
//    border.width: 2

    property int textSize: 14

    property int registerAddress: 0
    property bool latching: false
    property alias iconURL: button.iconSource
    property bool optional: false
    property int moduleNumber: parent.moduleNumber ? parent.moduleNumber : (parent.parent.moduleNumber ? parent.parent.moduleNumber : (parent.parent.parent.moduleNumber ? parent.parent.parent.moduleNumber : (parent.parent.parent.parent.moduleNumber ? parent.parent.parent.parent.moduleNumber : 0)))


    property string plcID: "_" + moduleNumber+"_"+"_"+x+"_"+y+"_"
    property string comment: label+"_"+iconURL+"_"+iconSource

    property string optionID: "_" + moduleNumber+"_"+"_"+x+"_"+y+"_"+registerAddress
    property bool momentaryClicked: false

    signal refresh
    signal plcrefresh

    property string positionIDx: "_x_" + button.comment + moduleNumber + "_M" + registerAddress +"_"+ label.length
    property string positionIDy: "_y_" + button.comment + moduleNumber + "_M" + registerAddress +"_"+ label.length

    property bool created: false
    property bool changePositionEnable: true
    property bool changingPosition: false
    property bool currentObjectChangingPosition: false



    width: 120
    height: 120
    opacity: 0.9

    MouseArea{
        enabled: Vars.positionConfiguration && changePositionEnable

        anchors.fill: parent
        drag.target: button
        drag.axis: Drag.XAndYAxis
    }

    onXChanged: {
        if(created && Vars.positionConfiguration){
            Parameters.editEntry(positionIDx, parseInt(x))
            Parameters.saveParameters()
        }
    }

    onYChanged: {
        if(created && Vars.positionConfiguration){
            Parameters.editEntry(positionIDy, parseInt(y))
            Parameters.saveParameters()
        }
    }


    Component.onCompleted: {
        if (optional)
            Options.addOptionalFeature(optionID, button.moduleNumber,comment)

        created=true
    }

    onPlcrefresh: {
        button.color = currentState ? colorOn : colorOff
    }

    Text {
        color: "#343434"
        anchors.centerIn: parent
        text: label
        font.pointSize: textSize
        horizontalAlignment: Text.AlignHCenter
    }

    Text {
        visible: Vars.registerVisibility
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        text: {
            if (button.registerAddress>0){
                if(latching)  text="L"+button.registerAddress
                else text="M"+button.registerAddress
            }
            else text=""
        }

        font.pointSize: 6
    }

    Connections {
        target: PLC
        onValuesUpdated: {
            button.plcrefresh()
            if (optional)
                button.visible = Options.getOptionState(optionID)


            if(registerAddress>0){
            if (latching)
                currentState = PLC.rL(registerAddress)
            else
                currentState = PLC.rM(registerAddress)
            }

            button.refresh()

            if (prevState != currentState)

            prevState = currentState

            if (button.mode == 1 && button.latching && !button.buttonPressed
                    && button.momentaryClicked) {
                PLC.l(registerAddress, 0)
                button.momentaryClicked = false
            }
            if (button.mode == 1 && !button.latching && !button.buttonPressed
                    && button.momentaryClicked) {
                PLC.m(registerAddress, 0)
                button.momentaryClicked = false
            }
            if (!button.buttonPressed && button.scale < 1.0)
                button.scale = 1.0
        }
    }

    onPressed: {
//        if (changingPosition==1){
//            currentObjectChangingPosition=1
//        }

        if (button.mode == 1)
            if (!latching) {
                PLC.m(button.registerAddress, 1)
            } else {
                PLC.l(button.registerAddress, 1)
            }
        button.momentaryClicked = true
    }

    onReleased: {
        if (button.mode == 1)
            if (!latching) {
                PLC.m(button.registerAddress, 0)
            } else {
                PLC.l(button.registerAddress, 0)
            }
        button.momentaryClicked = false
    }

    onClicked: {
        if (Vars.recipesRegisterMode && registerAddress > 0) {
            if (button.latching) {
                PLC.addLtoRecipe(registerAddress,label)
                Vars.recipeStatusText = "Added L" + registerAddress + " to recipe"
            } else {
                PLC.addMtoRecipe(registerAddress,label)
                Vars.recipeStatusText = "Added M" + registerAddress + " to recipe"
            }
        }

        if (!Vars.recipesRegisterMode) {
            if (!Vars.editMode) {
                if (!button.latching)
                    switch (button.mode) {
                    case 0:
                        //button.currentState = !button.currentState
                        PLC.m(button.registerAddress, !button.currentState)
                        break
                    case 2:
                        button.currentState = 1
                        PLC.m(button.registerAddress, button.currentState)
                        break
                    case 3:
                        button.currentState = 0
                        PLC.m(button.registerAddress, button.currentState)
                        break
                    }

                if (button.latching)
                    switch (button.mode) {
                    case 0:
                        //button.currentState = !button.currentState
                        PLC.l(button.registerAddress, !button.currentState)
                        break
                    case 2:
                        button.currentState = 1
                        PLC.l(button.registerAddress, button.currentState)
                        break
                    case 3:
                        button.currentState = 0
                        PLC.l(button.registerAddress, button.currentState)
                        break
                    }
                button.plcrefresh()
            } else {
                Vars.currentRegisterID = button.plcID
                Vars.currentRegisterAddress = Parameters.getRegisterAddress(
                            button.plcID)
                Vars.currentRegisterType = Parameters.getRegisterType(
                            button.plcID)
                Vars.editPadVisible = true
            }
        }
    }
}

/*##^## Designer {
    D{i:0;height:120;width:120}
}
 ##^##*/

