import QtQuick 2.0
import QtQuick.Controls 1.0

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

	// this MouseArea is used to detect all clicks on the item except the clickable sub-controls (like the seen checkbox)
	MouseArea {
		id: clickableArea
		anchors.fill: parent
		onClicked: {
			itemClicked();
		}
	}

	Row {
		visible: mouseArea.containsMouse
		anchors {
			right: parent.right
			top: parent.top
			topMargin: 4
		}

		Text {
			text: "seen"
		}

		CheckBox {
		}
	}

	// this MouseArea is used to change the cursor
	MouseArea {
		id: mouseArea
		anchors.fill: parent
		hoverEnabled: true
		propagateComposedEvents: true

		cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
		onClicked: {
			mouse.accepted = false;
		}
		onPressed: {
			mouse.accepted = false;
		}
		onReleased: {
			mouse.accepted = false;
		}
	}
}
