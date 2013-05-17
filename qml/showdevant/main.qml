import QtQuick 2.0
import com.guid75 1.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import "notifications.js" as Notifications

Rectangle {
	id: root
	width: 1024
	height: 768

	ShowModel {
		id: showModel
		table: "show"
	}

	Component.onCompleted: {
		var compo, messageBox;
		//switch (databaseManager.openDB()) {
		switch (2) {
		case 0:
			epicFailMessage.active = true;
			epicFailMessage.focus = true;
			break;
		case 1:
			Notifications.warning("It seems than the local file database is impossible to open, so we fallback on the memory database", 4000);
			break;
		case 2:
			break;
		default:
			break;
		}
		topToolbarAnimation.start();
	}

	SplitView {
		anchors {
			top: parent.top
			bottom: parent.bottom
			left: parent.left
			right: parent.right
			topMargin: 50
		}
		orientation: Qt.Horizontal

		Rectangle {
			id: leftRectangle
			width: 200
			Layout.minimumWidth: 100
			Layout.maximumWidth: 300
			color: "#DDDDDD"
			ShowList {
				id: showList
				anchors.fill: parent
				anchors.topMargin: 10
				anchors.bottomMargin: 40
			}
		}
		Rectangle {
			color: "#FFEECC"
			Layout.minimumWidth: 0
			Layout.fillWidth: true
		}
	}

	TopToolbar {
		id: topToolbar
		y: -60
	}

	PathAnimation {
		id: topToolbarAnimation
		easing.type: Easing.InQuad
		path: Path {
			startY: -50
			PathCubic {
				x: 0
				y: 0
			}
		}
		target: topToolbar
		duration: 750
	}

	Loader {
		anchors.fill: parent
		id: epicFailMessage
		sourceComponent: MessageBox {
			message: "It seems we can\'t even use a memory database :-(<br>As a database is vital to this software, it can not continue anymore"
			buttons: ListModel {
				ListElement {
					text: "Quit"
				}
			}
			onQuit: { epicFailMessage.active = false; Qt.quit(); }
		}
		active: false
		asynchronous: true
	}
}

