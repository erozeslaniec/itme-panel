import QtQuick 2.9
import QtQuick.Controls 2.2
import "../Vars.js" as Vars
import "../widgets"

Rectangle {
    width: 1200
    height: 600
    id: keyboard
    border.width: 1
    border.color: "grey"
    color: "lightGrey"
    signal refresh()
    property bool upperCaseEnabled:false

    Text{
        anchors.left:parent.left
        anchors.top:parent.top
        anchors.margins: 5
        text:qsTr("Search")
    }

    MouseArea {
        anchors.fill: parent
    }
    property string keys: "x1234567890QWERTYUIOP<ASDFGHJKL].ZXCVBNM<>? "
    signal hideKeyboard
    signal letterClicked(string letter)

    onHideKeyboard: {
        visible = false
        Vars.numpadVisible = false
    }

    onRefresh:{
        textVal.text=Vars.currentRecipeText
    }

    Rectangle{
        height:50
        width:400
        anchors.top:parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 5
        color:"white"
        border.color: "black"
        border.width: 1
        Text{
            id:textVal
            anchors.centerIn: parent
            font.pointSize: 16
            color:"black"
        }

    }


    property real value: 0

    Rectangle {
        id: rectangle2
        anchors.centerIn: parent
        color: "#ffffff"
        visible: true



        Grid {

            id: grid1
            anchors.centerIn: parent
            rows: 4
            columns: 11
            spacing: 1

            Repeater {
                id: repeater1
                model: keys.length + 1
                StateButton {
                    width: 100

                    onClicked: {
                   switch(index){
                   case 0:
                       Vars.currentRecipeText+="0"
                       Vars.currentRecipeNum=0
                       Vars.recipesListOffset=0
                       break
                   case 21:
                        Vars.currentRecipeText=Vars.currentRecipeText.substring(0,Vars.currentRecipeText.length-1)
                       break;
                   case 42:
                       upperCaseEnabled=!upperCaseEnabled
                       break;



                   default:
                       if(!upperCaseEnabled)
                       Vars.currentRecipeText+=keys.charAt(index).toLowerCase()
                       else
                       Vars.currentRecipeText+=keys.charAt(index)

                       Vars.currentRecipeNum=0
                       Vars.recipesListOffset=0
                   }

                    }

                    height: 100
                    label: {
                        switch(index){
                        case 42:
                            return "aA"
                            break
                        default:
                        return keys.charAt(index)}

                    }

                }
            }
        }
    }


    Connections {
        target: PLC
        onValuesUpdated: refresh()
    }
}
