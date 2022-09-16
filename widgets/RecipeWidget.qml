import QtQuick 2.0
import "../Vars.js" as Vars

Rectangle {
    id: main
    width: 600
    height: 300
    property int entryCount: 4
    Column {
        id: column2
        anchors.left: column1.right
        anchors.top: column1.top
        spacing: 5
        StateButton {
            text: "↑"
            onClicked: {
                if (Vars.recipesListOffset > 0)
                    Vars.recipesListOffset -= 1
            }
        }
        StateButton {
            text: "↓"
            onClicked: {
                //Dopisac jakies ograniczenie
                if (Vars.recipesListOffset < PLC.getRecipeListSize(
                            Vars.currentRecipeText) - main.entryCount)
                    Vars.recipesListOffset += 1
            }
        }
    }
    Column {
        id: column1

        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 2
        Repeater {
            id: repeater1
            model: main.entryCount

            RecipeRow {
                onRefresh: {
                    text = PLC.getRecipeList(index + Vars.recipesListOffset,
                                             Vars.currentRecipeText)
                    if (index == Vars.currentRecipeNum)
                        state = "PRESSED"
                    else
                        state = "RELEASED"
                }

                onClicked: {
                    Vars.currentRecipeNum = index
                }
            }
        }
    }
}
