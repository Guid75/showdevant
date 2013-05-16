import QtQuick 2.0

Rectangle {
    property alias message: mainText.text
	property alias duration: mainTimer.interval

	id: myself
	anchors.horizontalCenter: parent.horizontalCenter
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

	Timer {
		id: mainTimer
		interval: duration
		running: true
		repeat: false
		onTriggered: {
			myself.destroy();
		}
	}
}
