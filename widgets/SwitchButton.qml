import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.3

Switch {



    property int registerAddress: -1
    property bool latching
    property color colorOn:"green"




    checked: false
    Connections {
        target: PLC
        onValuesUpdated: {
            if (!pressed) {
                if (latching)
                    checked = PLC.rL(registerAddress)
                else
                    checked = PLC.rM(registerAddress)
            }
        }
    }

    onToggled: {
        if (latching)
            PLC.l(registerAddress, checked)
        else
            PLC.m(registerAddress, checked)
    }




}
