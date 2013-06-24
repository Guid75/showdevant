import QtQuick 2.0
import QtQuick.Controls 1.0
import com.guid75 1.0

ShadowRectangle {
	id: fatherRectangle
	signal itemClicked()
	height: 55
	width: 200

	function __getPadded(num, pad) {
		var i;
		var str = '' + num;

		if (str.length >= pad)
			return str;

		for (i = 0; i < pad; i++) {
			str = '0' + str;
			if (str.length >= pad)
				return str;
		}
		return str;
	}

	gradient: Gradient {
		GradientStop { position: 0.0; color: mouseArea.containsMouse ? "white" : "#BBBBBB"}
		GradientStop { position: 1.0; color: mouseArea.containsMouse ? "white" : "#DDDDDD" }
	}
	color: mouseArea.containsMouse ? "#EEEEEE" : "#CCCCCC"
	Column {
		anchors {
			fill: parent
			margins: 4
		}
		Text {
			color: (new Date(1000 * date)) < new Date() ? "black" : "#888888"
			text: "Episode S%1E<b>%2</b>".arg(__getPadded(season, 2)).arg(__getPadded(episode, 2))
			renderType: Text.NativeRendering
			font.pointSize: 12
		}
		Text {
			color: (new Date(1000 * date)) < new Date() ? "black" : "#888888"
			text: '"%1"'.arg(title)
			renderType: Text.NativeRendering
			width: fatherRectangle.width
			font.pointSize: 8
			elide: Text.ElideMiddle
		}
		Text {
			font.pointSize: 8
			horizontalAlignment: Text.AlignRight
		}
	}

	Rectangle {
		gradient: Gradient {
			GradientStop { position: 0.0; color: "#888888" }
			GradientStop { position: 1.0; color: "#AAAAAA" }
		}
//		color: "blue"
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		height: dateText.contentHeight
		width: dateText.contentWidth + 4

		Text {
			id: dateText
			color: "white"
			renderType: Text.NativeRendering
			font.pointSize: 8
			text: '%1'.arg(Qt.formatDate(new Date(date * 1000)))
			anchors.centerIn: parent
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
		spacing: 2
		anchors {
			right: parent.right
			top: parent.top
			topMargin: 4
		}

		Text {
			text: "seen"
			renderType: Text.NativeRendering
		}

		CheckBox {
			checked: seen
			onClicked: {
				cache.watchShow(show, season, episode);
			}
		}
	}

	Image {
		visible: authenticator.isLogged() && !mouseArea.containsMouse && seen
		source: "check.png"
		anchors {
			right: parent.right
			top: parent.top
			topMargin: 4
			rightMargin: 4
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
