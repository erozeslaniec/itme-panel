import QtQuick 2.0
import  "../icons"

Rectangle {
    width: 730
    height: 496
    color: "#c0c0c0"
    border.color: "#6e6c6c"
    border.width: 1

    Display{
        x: 76
        y: 135
        width: 360
        height: 120
        textSize: 26
        units: "m/min"
        label: "Prędkość wyciągania"

    }
    Display{
        x: 463
        y: 64
        width: 240
        height: 120
        units: "mm"
        label: "Odchyłka średnicy"

    }
    StateButton{
        x: 503
        y: 177
        width: 160
        height: 160
        fontSize: 10
        textSize: 17
        label: "Kontrola\nśrednicy:\nRĘCZNA"

    }
    Display{
        x: 8
        y: 343
        width: 240
        height: 120
        label: "Aktualna ilość prętów"

    }
    Display{
        x: 245
        y: 343
        width: 240
        height: 120
        label: "Zadana ilość prętów"

    }
    Display{
        x: 463
        y: 343
        width: 240
        height: 120
        label: "Długość prętów"
        units: "mm"

    }
}
