import QtQuick 2.0
import QtQuick.LocalStorage 2.0
import com.guid75 1.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import "commands.js" as Commands

Rectangle {
	id: root
	width: 1024
	height: 768

	property int morphAnimationTime : 500

	ShowModel {
		id: showModel
	}

	MyShowsModel {
		id: myShowsModel
	}

	SortFilterProxyModel {
		id: showProxyModel
		sourceModel: myShowsModel
		sortField: "title"
		sortOrder: Qt.AscendingOrder
		sortCaseSensitivity: Qt.CaseInsensitive
		filterField: "title"
		filterCaseSensitivity: Qt.CaseInsensitive
	}

	SeasonListModel {
		id: seasonListModel
	}

	Connections {
		target: authenticator
		onError: {
			wrongPasswordMessage.active = true;
		}
		onLogStateChanged: {
			if (authenticator.logState === Authenticator.Logged) {
				// TODO refresh the show model with all user shows
				cache.synchronizeMemberInfos();
			}
		}
	}

	Connections {
		target: cacheQmlProxy
		onShowAdded: {
			notifyZone.notify('"%1" as been <b>added</b> to your followed tv shows'.arg(title), 4000);
		}
		onShowRemoved: {
			notifyZone.notify('"%1" as been <b>removed</b> from your followed tv shows'.arg(title), 4000);
		}
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

		authenticator.autoLogin();
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
					seasonListModel.show = showId;
					playgroundLoader.source = "SeasonsViewer.qml";
					seasonSelector.visible = false;
					episodeSelector.visible = false;

					bannerImage.source = "http://api.betaseries.com/pictures/show/" + showId + ".jpg?key=9adb4ab628c6";
					bannerText.text = title;
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
				anchors {
					left: parent.left
					right: parent.right
					top: resumeRectangle.bottom
					margins: 4
				}
				visible: false
				onCurrentIndexChanged: {
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
					//playgroundLoader.item.season = episodeListModel.season
					playgroundLoader.item.season = seasonSelector.current;
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

			Connections {
				target: playgroundLoader.item
				onSeasonClicked: {
					seasonMorpher.x = x;
					seasonMorpher.y = y;
					seasonMorpher.width = w;
					seasonMorpher.height = h;
					seasonMorpher.current = season;
					seasonMorpher.min = 1;
					seasonMorpher.max = seasonListModel.count;
					seasonMorpher.visible = true;
					shadowSeasons.start();
					seasonMorpherAnimation.initXY();
					seasonMorpherAnimation.start();
				}
			}

			NumberAnimation {
				id: shadowSeasons
				target: playgroundLoader.item
				from: 1.0
				to: 0.0
				duration: morphAnimationTime
				easing.type: Easing.InOutQuad
				property: "opacity"
				onStopped: {
					seasonSelector.current = seasonMorpher.current;
					seasonSelector.min = 1;
					seasonSelector.max = seasonListModel.count;
					seasonSelector.visible = true;
					seasonMorpher.visible = false;
					playgroundLoader.setSource("EpisodesViewer.qml", {
												   currentSeason: seasonMorpher.current,
												   currentShow: seasonListModel.show
											   });
				}
			}

			ParallelAnimation {
				id: seasonMorpherAnimation
				function initXY() {
					seasonMorpherX.to = seasonSelector.mapToItem(null, 0, 0).x;
					seasonMorpherY.to = seasonSelector.mapToItem(null, 0, 0).y;
				}
				NumberAnimation {
					id: seasonMorpherX
					target: seasonMorpher
					property: "x"
					//to: 205 //seasonSelector.mapToItem(null, 0, 0).x
					easing.type: Easing.InOutQuad
					duration: morphAnimationTime
				}
				NumberAnimation {
					id: seasonMorpherY
					target: seasonMorpher
					property: "y"
//					to: 158 //seasonSelector.mapToItem(null, 0, 0).y
					easing.type: Easing.InOutQuad
					duration: morphAnimationTime
				}
				NumberAnimation {
					target: seasonMorpher
					property: "width"
					to: seasonSelector.width
					easing.type: Easing.InOutQuad
					duration: morphAnimationTime
				}
				NumberAnimation {
					target: seasonMorpher
					property: "height"
					to: seasonSelector.height
					easing.type: Easing.InOutQuad
					duration: morphAnimationTime
				}
/*				NumberAnimation {
					target: seasonMorpher
					property: "fontSize"
					from: 12
					to: 13
					easing.type: Easing.InOutQuad
					duration: morphAnimationTime
				}
				NumberAnimation {
					target: seasonMorpher
					property: "secondaryTextOpacity"
					from: 1
					to: 0
					easing.type: Easing.InOutQuad
					duration: morphAnimationTime
				}*/
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

	function __raiseLoginBox() {
		loginBox.active = true;
	}

	TopToolbar {
		id: topToolbar
		y: -60
		onAskForLogin: {
			__raiseLoginBox();
		}
		onAskForLogout: {
			authenticator.forgetMe();
		}
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

	RangeSelector {
		id: seasonMorpher
		currentTemplate: "Season %1/%2"
		visible: false
	}

/*	SeasonItem {
		id: seasonMorpher
		visible: false
	}*/

	BubbleMenu {
		id: bubbleMenu
		visible: false
		bubbleX: showList.width - 10
		model: ListModel {
			ListElement {
				text: "Remove"
				name: "remove"
			}
			ListElement {
				text: "Archive"
				name: "archive"
			}
			ListElement {
				text: "Refresh"
				name: "refresh"
			}
		}
		onItemClicked: {
			var showId;
			switch (name) {
			case "remove":
				showId = showProxyModel.get(showList.currentShowIndex).show_id;
				cache.removeShow(showId);
				break;
			default:
				break;
			}
		}
		onPressOutside: {
			// if we are clicking on another show dropdown, don't hide the bubble menu
			if (!showList.overDropDown(x, y)) {
				bubbleMenu.visible = false;
			}
		}
	}

	NotifyZone {
		id: notifyZone
		anchors {
			horizontalCenter: parent.horizontalCenter
			top: parent.top
			bottom: parent.bottom
		}
		width: parent.width
	}

	Loader {
		id: epicFailMessage
		anchors.fill: parent
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
	Loader {
		id: loginBox
		anchors.fill: parent
		sourceComponent: LoginBox {
			onCloseMe: {
				loginBox.active = false;
			}
		}
		active: false
		asynchronous: true
		onLoaded: {
			if (active) {
				item.initFocus();
			}
		}
	}
	Loader {
		id: wrongPasswordMessage
		anchors.fill: parent
		sourceComponent: MessageBox {
			message: {
				switch (authenticator.lastError()) {
				case Authenticator.BadPassword:
					return "Wrong password";
				case Authenticator.UnknownUser:
					return "Unknown user";
				case Authenticator.UnknownError:
					return "Unknown error";
				}
				return "";
			}
			buttons: ListModel {
				ListElement {
					text: "Ok"
				}
			}
			onQuit: { wrongPasswordMessage.active = false; }
		}
		active: false
		asynchronous: true
	}
}

