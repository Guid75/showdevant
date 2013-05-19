import QtQuick 2.0

ShadowRectangle {
	signal itemClicked()
	height: 50
	width: 200
	gradient: Gradient {
		GradientStop { position: 0.0; color: mouseAreaEpisode.containsMouse ? "white" : "#BBBBBB"}
		GradientStop { position: 1.0; color: mouseAreaEpisode.containsMouse ? "white" : "#DDDDDD" }
	}
	color: mouseAreaEpisode.containsMouse ? "#EEEEEE" : "#CCCCCC"
	Column {
		anchors {
			top: parent.top
			left: parent.left
			topMargin: 4
			leftMargin: 4
		}
		Text {
			text: "Season <b>" + number + "</b>"
			font.pointSize: 12
		}
		Text {
			text: episode_count + " episodes"
			font.pointSize: 8
		}
	}
	MouseArea {
		id: mouseAreaEpisode
		anchors.fill: parent
		hoverEnabled: true
		cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
	}
}
