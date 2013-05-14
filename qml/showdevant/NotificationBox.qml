import QtQuick 2.0

Rectangle {
    property alias message: mainText.text

    x: 0
    y: 0
    // get a good z
    width: Math.max(mainText.contentWidth + 20, 100)
    height: 32
    radius: 2
    antialiasing: true
    smooth: true

    color: "#fbfaae"
    border.color: "#AAAAAA"

    Text {
        id: mainText
        color: "#444444"
        anchors.centerIn: parent
    }
}
