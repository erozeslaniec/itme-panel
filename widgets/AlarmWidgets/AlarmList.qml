import QtQuick 2.9

Rectangle {
    width: 1800
    height: 200

    Rectangle {
        width: parent.width
        height: parent.height

        Component {
            id: alarmDelegate

            Rectangle {
                height: 70
                width: parent.width

                color: {if(index%2)"lightGrey";else "grey"}


                    Text {
                        id: descriptionText
                        text: description
                        x:30
                        anchors.verticalCenter: parent.verticalCenter
                        color: {
                            if (active)
                                "red"
                            else
                                "black"
                        }
                    }

                    Text {
                        id: occurrenceText
                        x:600
                        anchors.verticalCenter: parent.verticalCenter
                        text: "<b>" + qsTr("Occurred:") + "</b>" + occurrence
                    }

                    Text {
                        id: restoreText
                        x:1200
                        anchors.verticalCenter: parent.verticalCenter
                        text: "<b>" + qsTr("Restored:") + "</b>" + restore
                    }

            }
        }
    }

    ListView {
        id: listView
        anchors.fill: parent
        model: alarmListModel

        delegate: alarmDelegate

        //        highlight: Rectangle {
        //            color: "lightsteelblue"
        //            radius: 5
        //        }
        // focus: true
    }

    Connections {
        target: PLC
        onAlarmListUpdated: {
            listView.model = 0
            listView.model = alarmListModel
        }
    }
}
