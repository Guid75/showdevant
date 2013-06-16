import QtQuick 2.0

ShadowRectangle {
	property int seasonNumber : 0
	property int episodeCount : 0
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
			id: mainText
			text: "Season <b>" + seasonNumber + "</b>"
			renderType: Text.NativeRendering
			font.pointSize: 12
		}
		Text {
			id: secondaryText
			text: episodeCount + " episodes"
			renderType: Text.NativeRendering
			font.pointSize: 8
		}
	}
	MouseArea {
		id: mouseAreaEpisode
		anchors.fill: parent
		hoverEnabled: true
		cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
		onClicked: {
			itemClicked();
		}
	}
}
