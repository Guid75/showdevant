import QtQuick 2.0

Rectangle {
	anchors {
		left: parent.left
		right: parent.right
		top: parent.top
	}
	height: 50
	color: "#8888FF"

	Image {
		id: betaSeriesImage
		source: "betaseries.png"
		anchors {
			leftMargin: 10
			left: parent.left
			verticalCenter: parent.verticalCenter
		}
		fillMode: Image.PreserveAspectFit
	}

	MouseArea {
		anchors.fill: betaSeriesImage
	}

	Rectangle {
		id: searchRectangle
		color: "white"
		border.color: "#888888"
		border.width: 1
		antialiasing: true
		radius: 4
		width: 300
		height: 30
		clip: true
		anchors {
			leftMargin: 10
			left: betaSeriesImage.right
			verticalCenter: parent.verticalCenter
		}
		TextInput {
			id: searchInput
			clip: true
			anchors.fill: parent
			anchors.leftMargin: 8
			anchors.rightMargin: 8
			anchors.topMargin: 8
			selectByMouse: true
			focus: true
		}

		Text {
			text: "Enter a TV show name of a part of it"
			visible: searchInput.text == ""
			color: "#AAAAAA"
			anchors.fill: parent
			anchors.leftMargin: 8
			anchors.rightMargin: 8
			anchors.topMargin: 8
		}
	}

	Rectangle {
		color: "#4444FF"
		border.color: "#AAAAAA"
		border.width: 1
		antialiasing: true
		radius: 4
		width: 100
		height: 30
		clip: true
		anchors {
			leftMargin: 5
			left: searchRectangle.right
			verticalCenter: parent.verticalCenter
		}
		Text {
			color: "white"
			horizontalAlignment: Text.AlignHCenter
			text: "search"
			clip: true
			anchors.fill: parent
			anchors.leftMargin: 8
			anchors.rightMargin: 8
			anchors.topMargin: 8
		}
	}
}
