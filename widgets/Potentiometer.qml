import QtQuick 2.2
import QtQuick.Controls 2.2

Dial {
    id: dial
    property int registerAddress: -1
    signal refresh
    property bool zoomWhenChanging: false
    property real zoomValue: 1.2
    property real nominalWidth: 100
    property real nominalHeight: 100
    to: 220

    Connections {
        target: PLC
        onValuesUpdated: {
            dial.refresh()
        }
    }

    onRefresh: {
        if (!pressed) {
            if (dial.value !== PLC.rDW(registerAddress)) {
                dial.value = PLC.rDW(registerAddress)
                if (zoomWhenChanging)
                    width = nominalWidth * zoomValue
                height = nominalHeight * zoomValue
                timer1.restart()
            }
        }
    }

    onMoved: {
        if (pressed) {
            PLC.d(registerAddress, dial.value)
            if (zoomWhenChanging) {
                width = nominalWidth * zoomValue
                height = nominalHeight * zoomValue
                timer1.restart()
            }
        }
    }

    Behavior on width {
        NumberAnimation {
            duration: 500
        }
    }

    Behavior on height {
        NumberAnimation {
            duration: 300
        }
    }

    Timer {
        id: timer1
        repeat: false
        interval: 1500
        onTriggered: {
            width = nominalWidth
            height = nominalHeight
        }
    }
}
