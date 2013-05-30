import QtQuick 2.0
import QtQuick.LocalStorage 2.0
import com.guid75 1.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import "notifications.js" as Notifications
//import "database.js" as Database

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
		filterField: "title"
		filterCaseSensitivity: Qt.CaseInsensitive
	}

	Connections {
		target: cache
//		onRefreshDone: {
//			loadingWidget.active = false;
//			__showReady(showId);
//		}
	}

	SeasonListModel {
		id: seasonListModel
	}

	EpisodeModel {
		id: episodeModel
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
			TextField {
				placeholderText: "Filter wildcard"
				id: showFilterField
				anchors.top: parent.top
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.topMargin: 10
				anchors.leftMargin: 2
				anchors.rightMargin: 2
				onTextChanged: {
					showProxyModel.filter = text;
				}
			}

			ShowList {
				id: showList
				model: showProxyModel
				clip: true
				anchors.top: showFilterField.bottom
				anchors.bottom: parent.bottom
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.topMargin: 6
				anchors.bottomMargin: 40
				onShowSelected: {
					bannerImage.source = "http://api.betaseries.com/pictures/show/" + showId + ".jpg?key=9adb4ab628c6";
					bannerText.text = title;

					seasonListModel.show = showId;
					playgroundLoader.source = "SeasonsViewer.qml";
					seasonSelector.visible = false;
					episodeSelector.visible = false;
				}
			}
		}
		Item {
			Layout.minimumWidth: 0
			Layout.fillWidth: true

			Rectangle {
				id: resumeRectangle
				anchors {
					left: parent.left
					right: parent.right
					top: parent.top
					margins: 4
				}
				height: 100
				color: "white"
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
			}

			RangeSelector {
				id: seasonSelector
				currentTemplate: "Season %1/%2"
				Behavior on height {
					NumberAnimation { from: 20; to: 40; duration: 500 }
				}
				anchors {
					left: parent.left
					right: parent.right
					top: resumeRectangle.bottom
					margins: 4
				}
				visible: false
				onCurrentIndexChanged: {
					episodeModel.season = current;
					episodeSelector.current = 1;
					if (playgroundLoader.item.widgetType === "episodes") {
						playgroundLoader.item.setCurrentSeason(current);
					} else if (playgroundLoader.item.widgetType === "episodedetail") {
						playgroundLoader.item.season = current;
						playgroundLoader.item.episode = 1;
					}
				}
				onCloseMe: {
					playgroundLoader.source = "SeasonsViewer.qml";
					seasonSelector.visible = false;
					episodeSelector.visible = false;
				}
			}

			RangeSelector {
				id: episodeSelector
				currentTemplate: "Episode %1/%2"
				Behavior on height {
					NumberAnimation { from: 20; to: 40; duration: 500 }
				}
				anchors {
					left: parent.left
					right: parent.right
					top: seasonSelector.bottom
					margins: 4
				}
				visible: false
				onCurrentIndexChanged: {
					// TODO refresh current episode details
					playgroundLoader.item.season = episodeModel.season
					playgroundLoader.item.episode = current;
				}
				onCloseMe: {
					playgroundLoader.setSource("EpisodesViewer.qml", {
												   currentSeason: seasonSelector.current,
												   currentShow: seasonListModel.show
											   });

					episodeSelector.visible = false;
				}
			}

			Loader {
				id: playgroundLoader
				anchors {
					top: episodeSelector.visible ? episodeSelector.bottom : (seasonSelector.visible ? seasonSelector.bottom : resumeRectangle.bottom)
					left: parent.left
					right: parent.right
					bottom: parent.bottom
					margins: 4
				}
			}

			Loader {
				id: loadingWidget
				anchors.centerIn: playgroundLoader
				sourceComponent: LoadingWidget {
					anchors.centerIn: parent
					color: "#888888"
					width: 200
				}
				active: false
				asynchronous: true
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

