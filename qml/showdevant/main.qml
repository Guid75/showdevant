import QtQuick 2.0
import QtQuick.LocalStorage 2.0
import com.guid75 1.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import "notifications.js" as Notifications
import "database.js" as Database

Rectangle {
	id: root
	width: 1024
	height: 768

	ShowModel {
		id: showModel
	}

	SortFilterProxyModel {
		id: showProxyModel
		sourceModel: showModel
		sortField: "title"
		sortOrder: Qt.AscendingOrder
		sortCaseSensitivity: Qt.CaseInsensitive
	}

	Connections {
		target: showManager
		onRefreshDone: {
			loadingWidget.active = false;
			seasonModel.show = showId;
		}
	}

	/*SqlTableModel {
		id: seasonModel
		table: "season"
		sortOrder: Qt.DescendingOrder
		sortField: "number"
	}*/

	SeasonModel {
		id: seasonModel
	}

	Component.onCompleted: {
		switch (databaseManager.openDBLastError()) {
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
			color: "#DDDDDD"
			ShowList {
				id: showList
				model: showProxyModel
				anchors.fill: parent
				anchors.topMargin: 10
				anchors.bottomMargin: 40
			}
		}
		Rectangle {
			color: "#EEEEEE"
			Layout.minimumWidth: 0
			Layout.fillWidth: true

			Rectangle {
				id: resumeRectangle
				height: 100
				color: "#EEEEEE"
				Image {
					id: bannerImage
					anchors.fill: parent
					fillMode: Image.PreserveAspectCrop
				}
				Rectangle {
					id: backgroundItem
					anchors {
						left: parent.left
						right: parent.right
						bottom: parent.bottom
					}
					color: "black"
					opacity: bannerText.text == "" ? 0.0 : 0.7
					height: 30
				}
				Item {
					anchors.fill: backgroundItem
					Text {
						id: bannerText
						anchors.fill: parent
						anchors.leftMargin: 6
						verticalAlignment: Text.AlignVCenter
						//font.bold: true
						font.pointSize: 12
						color: "white"
					}
				}

				anchors {
					top: parent.top
					left: parent.left
					right: parent.right
					margins: 8
				}
			}

			Rectangle {
				color: "#EEEEEE"
				anchors {
					top: resumeRectangle.bottom
					left: parent.left
					right: parent.right
					bottom: parent.bottom
					margins: 4
				}
				Flickable {
					anchors.fill: parent
					contentWidth: Math.max(width, 208)
					contentHeight: flowEpisode.childrenRect.height + 8
					clip: true
					Flow {
						id: flowEpisode
						anchors.fill: parent
						anchors.margins: 4
						spacing: 10
						add: Transition {
							NumberAnimation {
								easing.type: Easing.OutCirc
								properties: "x,y"
								duration: 500
							}
						}
						move: Transition {
							NumberAnimation {
								easing.type: Easing.OutCirc
								properties: "x,y"
								duration: 500
							}
						}
						Repeater {
							id: repeaterEpisode
							model: seasonModel
							SeasonItem {
								MouseArea {
									anchors.fill: parent
									propagateComposedEvents: true
									onClicked: {
										// TODO animation to bring the season above
										mouse.accepted = false;
									}
								}
							}
						}
					}
				}
				Loader {
					id: loadingWidget
					anchors.centerIn: parent
					sourceComponent: LoadingWidget {
						anchors.centerIn: parent
						width: 200
					}
					active: false
					asynchronous: true
				}
			}
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

