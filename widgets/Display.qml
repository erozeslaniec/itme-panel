import QtQuick 2.2
import "../Vars.js" as Vars

Rectangle {
    id: display
    width: 200
    height: 100
    color: "#c0c0c0"

    //border.color: "darkgrey"
    //border.width: 2
    //radius: 2

    property string units: ""
    property string label: ""
    property int decimalPoints: 0
    property int maximumValue: 10000
    property int minimumValue: 0
    property bool allowInput: true
    property int rawValue: -1
    property int prevRawValue: rawValue


    property string positionIDx: "_x_" + moduleNumber + "_D" + registerAddress
    property string positionIDy: "_y_" + moduleNumber + "_D" + registerAddress
    property bool created: false
    property bool changePositionEnable:true
    property bool changingPosition: false
    property bool currentObjectChangingPosition: false
    property string comment: label+"_["+units+"]"

    /*
    color: {
        if (allowInput)
            "LightGrey"
        else
            Qt.darker("LightGrey")
    }
    */

    property int textSize: 15
    property bool cheating: false
    property real scaler: 1.0

    property string plcID: "_"+ moduleNumber + "_" + display.width + "_" + display.height + "_" + Vars.currentScreen +"_D"+display.registerAddress

    property int registerAddress: 0
    property bool doubleWord: true
    property int moduleNumber:parent.moduleNumber ? parent.moduleNumber : (parent.parent.moduleNumber ? parent.parent.moduleNumber : (parent.parent.parent.moduleNumber ? parent.parent.parent.moduleNumber : 0) )
    property bool optional:false
    property string optionID:plcID


    signal clicked
    signal refresh



    onXChanged: {
        if(created && Vars.positionConfiguration){
            positionIDx="_x_" + moduleNumber + "_D" + registerAddress
            Parameters.editEntry(positionIDx, parseInt(x))
            Parameters.saveParameters()
        }
    }

    onYChanged: {
        if(created && Vars.positionConfiguration){
            positionIDy="_y_" + moduleNumber + "_D" + registerAddress
            Parameters.editEntry(positionIDy, parseInt(y))
            Parameters.saveParameters()
        }
    }

    Component.onCompleted: {
        if(optional)
        Options.addOptionalFeature(optionID,parent.moduleNumber,comment)

        created=true


    }



    Text {
        visible: Vars.registerVisibility
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        text: "D"+registerAddress
        font.pointSize: 6
        color: "#343434"
    }

    Text {
        id: labelText
        anchors.top: parent.top
        anchors.topMargin: 5

        anchors.horizontalCenter: parent.horizontalCenter
        text: label
        font.pointSize: textSize
        color: "#343434"
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        y: 50
        id: value
        font.pointSize: textSize + 11
        text: "0000 " + units
        horizontalAlignment: Text.AlignHCenter
        color: "#343434"
    }

    Connections {
        target: PLC
        onValuesUpdated: {
            if(PLC.rL(2080))
                makeImperialConversion()
            else
                makeMetricConversion()

            if(optional)
            display.visible=Options.getOptionState(optionID)

            prevRawValue = rawValue

            if (doubleWord)
                rawValue = PLC.rDW(registerAddress)
            else
                rawValue = PLC.rD(registerAddress)

            if (prevRawValue != rawValue) {

                if (!doubleWord)
                    value.text = (PLC.rD(registerAddress) / scaler / Math.pow(
                                10, decimalPoints)).toFixed(decimalPoints) + " " + units
                if (doubleWord)
                    value.text = (PLC.rDW(registerAddress) / scaler / Math.pow(
                                10, decimalPoints)).toFixed(decimalPoints) + " " + units



            }
            display.refresh()
        }
    }

    MouseArea {
        id: mouseMain
        anchors.fill: parent
        onClicked: {
            if (!Vars.editMode) {
                if (Vars.recipesRegisterMode && registerAddress > 0) {
                    if (doubleWord) {
                        PLC.addDWtoRecipe(registerAddress,label)
                        Vars.recipeStatusText = "Added DW" + registerAddress + " to recipe"
                    } else {
                        PLC.addDtoRecipe(registerAddress,label)
                        Vars.recipeStatusText = "Added D" + registerAddress + " to recipe"
                    }
                }

                display.clicked()
                if (allowInput && !Vars.recipesRegisterMode) {

                    Vars.currentScaler = scaler
                    Vars.keypadDecimalPlaces = decimalPoints
                    Vars.numpadAddress = registerAddress
                    Vars.numpadReset = true
                    if (!doubleWord)
                        Vars.numpadValue = PLC.rD(registerAddress) * scaler
                    if (doubleWord)
                        Vars.numpadValue = PLC.rDW(registerAddress) * scaler

                    Vars.numpadMaximum = maximumValue/Vars.currentScaler
                    Vars.numpadMinimum = minimumValue/Vars.currentScaler
                    Vars.numpadDoubleWord = doubleWord

                    Vars.numpadVisible = true



                }
            } else {
                Vars.currentRegisterID = display.plcID
                Vars.currentRegisterAddress = Parameters.getRegisterAddress(
                            display.plcID)
                Vars.currentRegisterType = Parameters.getRegisterType(
                            display.plcID)
                Vars.editPadVisible = true
            }
        }
    }

    MouseArea{
        enabled: Vars.positionConfiguration && changePositionEnable

        anchors.fill: mouseMain
        drag.target: parent
        drag.axis: Drag.XAndYAxis
    }

    function makeImperialConversion(){
        if(units==="mm"){
            units="inch"
            if(decimalPoints===0){
                decimalPoints=2
            scaler=0.254}
            else
            scaler=25.4

        }

        if(units==="m"){
            units="ft"
            scaler=0.3048
        }

        if(units==="m/min"){
            units="ft/min"
            scaler=0.3048
        }

        if(units.toLowerCase()==="bar"){
            units="psi"
            scaler=0.0689475729
        }
        if(units.toLowerCase()==="μm"){
            units="thou"
            scaler=25.4
        }
    }

    function makeMetricConversion(){
        if(units==="inch"){
            units="mm"
            if(decimalPoints===2)
                decimalPoints=0
            scaler=1.0
        }

        if(units==="ft"){
            units="m"
            scaler=1.0
        }

        if(units==="ft/min"){
            units="m/min"
            scaler=1.0
        }

        if(units.toLowerCase()==="psi"){
            units="bar"
            scaler=1.0
        }
        if(units.toLowerCase()==="thou"){
            units="μm"
            scaler=1.0
        }
    }
}
