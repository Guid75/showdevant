import QtQuick 2.0

ShadowRectangle {
	id: fatherRectangle
	signal itemClicked()
	height: 50
	width: 200
	gradient: Gradient {
		GradientStop { position: 0.0; color: mouseArea.containsMouse ? "white" : "#BBBBBB"}
		GradientStop { position: 1.0; color: mouseArea.containsMouse ? "white" : "#DDDDDD" }
	}
	color: mouseArea.containsMouse ? "#EEEEEE" : "#CCCCCC"
	Column {
		anchors {
			top: parent.top
			left: parent.left
			topMargin: 4
			leftMargin: 4
		}
		Text {
			text: "Episode <b>" + episode + "</b>"
			font.pointSize: 12
		}
		Text {
			text: '"%1"'.arg(title)
			width: fatherRectangle.width
			font.pointSize: 8
			elide: Text.ElideMiddle
		}
	}
	MouseArea {
		id: mouseArea
		anchors.fill: parent
		hoverEnabled: true
		cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
	}
}
