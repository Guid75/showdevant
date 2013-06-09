import QtQuick 2.0

FocusScope {
	Component.onCompleted: {
		backAnimation.start();
	}

	Rectangle {
		id: back
		anchors.fill: parent
		opacity: 0
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

	NumberAnimation {
		id: backAnimation
		target: back
		property: "opacity"
		duration: 400
		easing.type: Easing.InOutQuad
		from: 0
		to: 0.40
	}
}
