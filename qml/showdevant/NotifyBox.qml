import QtQuick 2.0

Item {
	property alias message: mainText.text
	property alias duration: mainTimer.interval

	height: myself.height
	Rectangle {
		id: myself
		anchors.horizontalCenter: parent.horizontalCenter
		// get a good z
		width: Math.max(mainText.contentWidth + 20, 150)
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
				listModel.remove(index, 1);
			}
		}
	}
}
