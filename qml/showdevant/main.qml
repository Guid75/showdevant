import QtQuick 2.0
import com.guid75 1.0
import "notifications.js" as Notifications
import QtQuick.Controls 1.0;

Item {
	id: root

	// default size, but scalable by user
	width: 800
	height: 600

	Component.onCompleted: {
		var compo, messageBox;
		switch (0) {
		//switch (databaseManager.openDB()) {
		case 0:
			epicFailMessage.active = true;
			break;
			// return;
		case 1:
			Notifications.warning("It seems than the local file database is impossible to open, so we fallback on the memory database", 4000);
			break;
		case 2:
			break;
		default:
			break;
		}
		leftAnimation.start();
		topToolbarAnimation.start();
	}

	Rectangle {
		id: leftRectangle
		anchors {
			top: topToolbar.bottom
			bottom: parent.bottom
		}
		width: 200
		x: -200
		color: "#DDDDDD"
		ShowList {
			anchors.fill: parent
			anchors.topMargin: 10
			anchors.bottomMargin: 40
		}
	}

	PathAnimation {
		id: leftAnimation
		easing.type: Easing.InQuad
		path: Path {
			startX: -200
			PathCubic {
				x: 0
				y: 0
			}
		}
		target: leftRectangle
		duration: 750
	}

	TopToolbar {
		id: topToolbar
		y: -50
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
			onQuit: { Qt.quit(); }
		}
		active: false
		asynchronous: true
	}
}

