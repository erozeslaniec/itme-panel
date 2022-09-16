import QtQuick 2.2

Grid {
    id:locker
    columns:3
    rows:3
    spacing:5

    property int code:2211
    property int currentCode:0

    signal unlocked()

    Repeater{
        model:9
        PatternLockButton{
            value:index+1
            onClicked:{
                timerCode.restart()
               locker.currentCode=locker.currentCode*10+index+1
        if(locker.code==locker.currentCode)locker.unlocked()

            }


        }


    }


    Timer{
        id:timerCode
        interval:3000
        running:true
        repeat:true
        onTriggered:currentCode=0
    }


}
