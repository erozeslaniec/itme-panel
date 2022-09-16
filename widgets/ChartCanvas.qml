import QtQuick 2.12
import QtQuick.Controls 1.1
import QtCharts 2.3
import QtGraphicalEffects 1.0

Rectangle {
    id: canvRect
    width: 550
    height: 496
    color: "#00000000"
    border.color: "#6e6c6c"
    border.width: 1



    ChartView{
        id: frictionChart
        backgroundColor: "#00000000"
        plotAreaColor: "#00000000"
        legend.visible: false
        anchors.fill: parent
        antialiasing: true
        //signal refresh

        MouseArea{
            anchors.fill: parent
            visible: true
            onPressed: {
                pionowa.removePoints(0, 2)
                pionowa.append(mouseX-canvRect.width/2, -200)
                pionowa.append(mouseX-canvRect.width/2, 200)
            }
        }


        ValueAxis{
            id: axisX
            min: -200
            max: 200
        }
        ValueAxis{
            id: axisY
            min:-200
            max: 200
        }



        LineSeries{
            id: pionowa
            XYPoint{x:-50; y:-200}
            XYPoint{x:-50; y:200}
            color: "#5d5d5d"
            width: 5
            axisX: axisX
            axisY: axisY
        }
        LineSeries{
            id: pozioma
            XYPoint{x:-200; y:50}
            XYPoint{x:200; y:50}
            color: "#5d5d5d"
            width: 5
            axisX: axisX
            axisY: axisY
        }


//        Connections {
//            target: PLC
//            onValuesUpdated: {
//                for (var i =0; i<23; i++){
//                    actualValues[i]=PLC.rD(firstTableRegister+i)
//                    lineseriesFrictionTable.replace(i, previousValues[i], i, actualValues[i])
//                    previousValues[i] = actualValues[i]
//                }
//                frictionChart.refresh()
//            }
//        }
//        LineSeries{
//            id: lineseriesFrictionTable
//            name: "Torque Values"
//            axisX: axisX
//            axisY: axisY
//            Component.onCompleted: {
//                for (var i =0; i<23; i++){
//                    previousValues[i] = PLC.rD(firstTableRegister+i)
//                    lineseriesFrictionTable.append(i, previousValues[i])
//                }
//            }
//        }

    }

}
