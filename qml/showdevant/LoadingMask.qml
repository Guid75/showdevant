import QtQuick 2.0

Item {
	property alias count : loadingWidget.count
	property alias size : loadingWidget.size
	property alias color : loadingWidget.color
	property int speed : loadingWidget.speed

	Rectangle {
		id: back
		anchors.fill: parent
		opacity: 0.2
		color: "black"

		MouseArea {
			hoverEnabled: true;
			anchors.fill: parent;
			onEntered:  { }
			onExited:   { }
			onReleased: { }
			onPressed:  { }
			onClicked:  { }
			onWheel:    { }
		}
	}

	LoadingWidget {
		id: loadingWidget
		anchors.centerIn: parent
	}
}
