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

	property int morphAnimationTime : 300

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

	Component {
		id: seasonsViewerCompo
		SeasonsViewer {
			onSeasonClicked: __seasonClicked(season)
		}
	}

	Component {
		id: episodesViewerCompo
		EpisodesViewer {
			onEpisodeClicked: __episodeClicked(episode)
		}
	}

	Component {
		id: episodeDetailCompo
		EpisodeDetail {
		}
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

	function __seasonClicked(season) {
		var seasonsViewer = stackView.findWidget("seasons");

		stackView.push({
						   item: episodesViewerCompo,
						   properties: {
							   show: seasonsViewer.show,
							   season: season,
							   maxSeason: seasonsViewer.model.count
						   }
					   });

		//__precacheEpisodes(seasonsViewer.show, season, seasonsViewer.model.count);

		// prepare the season morpher

		var coord = seasonsViewer.getSeasonItemCoordinates(season);

		selectorMorpher.template = 'Season %1/%2'
		selectorMorpher.x = coord.x;
		selectorMorpher.y = coord.y;
		selectorMorpher.width = coord.w;
		selectorMorpher.height = coord.h;
		selectorMorpher.current = season;
		selectorMorpher.min = 1;
		selectorMorpher.max = seasonsViewer.model.count;
		selectorMorpher.visible = true;
		seasonSelector.opacity = 0;
		seasonSelector.visible = true;
		seasonSelector.current = season;
		seasonSelector.min = 1;
		seasonSelector.max = seasonsViewer.model.count;
		var ar = stackView.mapToItem(null, 0, 0);
		selectorMorpherAnimation.initTarget(ar.x, ar.y, seasonSelector.width, seasonSelector.height);
		selectorMorpherAnimation.start();
	}

	function __episodeClicked(episode) {
		var episodesViewer = stackView.findWidget("episodes");

		stackView.push({
						   item: episodeDetailCompo,
						   properties: {
							   show: episodesViewer.show,
							   season: episodesViewer.season,
							   episode: episode
						   }
					   });

		var coord = episodesViewer.getEpisodeItemCoordinates(episode);

		selectorMorpher.template = 'Episode %1/%2'
		selectorMorpher.x = coord.x;
		selectorMorpher.y = coord.y;
		selectorMorpher.width = coord.w;
		selectorMorpher.height = coord.h;
		selectorMorpher.current = episode;
		selectorMorpher.min = 1;
		selectorMorpher.max = episodesViewer.model.count;
		selectorMorpher.visible = true;
		episodeSelector.opacity = 0;
		episodeSelector.visible = true;
		episodeSelector.current = episode;
		episodeSelector.min = 1;
		episodeSelector.max = episodesViewer.model.count;
		var ar = stackView.mapToItem(null, 0, 0);
		selectorMorpherAnimation.initTarget(ar.x, ar.y, episodeSelector.width, episodeSelector.height);
		selectorMorpherAnimation.start();
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
			z: 1
			width: 200
			Layout.minimumWidth: 100
			color: "#DDDDDD"

			ColumnLayout {
				spacing: 2
				anchors {
					fill: parent
					topMargin: 6 // top toolbar shadow needs a large top margin
					leftMargin: 4
					rightMargin: 4
					bottomMargin: 2
				}
				TextField {
					Layout.fillWidth: true
					id: showFilterField
					placeholderText: "Filter wildcard"
					onTextChanged: {
						showProxyModel.filter = text;
					}
				}

				ShowList {
					id: showList
					model: showProxyModel
					clip: true
					Layout.fillWidth: true
					Layout.fillHeight: true
					onShowSelected: {
						var seasonsViewer = stackView.findWidget('seasons');
						if (seasonsViewer && seasonsViewer.show === showId)
							return;

						// pop all components until only one remains
						stackView.pop(null);

						seasonSelector.visible = false;
						episodeSelector.visible = false;

						stackView.push({
										   item: seasonsViewerCompo,
										   replace: true,
										   properties: {
											   show: showId
										   }
									   });

						bannerImage.source = "http://api.betaseries.com/pictures/show/" + showId + ".jpg?key=9adb4ab628c6";
						bannerText.text = title;
					}
				}
			}
		}
		ColumnLayout {
			Layout.minimumWidth: 0
			Layout.fillWidth: true
			spacing: 0

			Rectangle {
				id: resumeRectangle
				Layout.fillWidth: true
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
				template: "Season %1/%2"
				Layout.fillWidth: true
				visible: false
				onCurrentChanged: {
					var episodesViewer = stackView.findWidget("episodes");

					if (!episodesViewer) {
						// not yet initialized
						return;
					}

					// preload surrounding episodes
					//__precacheEpisodes(episodesViewer.show, current, max);

					if (stackView.currentItem === episodesViewer) {
						// replaces an episodes viewer by another episodes viewer
						stackView.push({
										   item: episodesViewerCompo,
										   replace: true,
										   properties: {
											   show: episodesViewer.show,
											   season: current,
											   maxSeason: max
										   }
									   });
					} else {
						// the episodes item is not visible but we update its model
						episodesViewer.season = current;

						var episodeDetail = stackView.findWidget("episodedetail");
						episodeDetail.season = current;

						episodeSelector.current = 1;
						episodeSelector.refreshCurrent();
					}
				}
				onCloseMe: {
					var seasonsViewer = stackView.findWidget("seasons");

					if (stackView.currentItem !== stackView.findWidget("episodes")) {
						stackView.pop(null);
						return;
					}

					selectorMorpher.current = current;
					selectorMorpher.visible = true;
					selectorMorpher.initReturnStart(seasonSelector);
					seasonSelector.opacity = 0;

					var coord = seasonsViewer.getSeasonItemCoordinates(current);
					selectorMorpherAnimation.initTarget(coord.x, coord.y - seasonSelector.height, coord.w, coord.h);
					selectorMorpherAnimation.start();
					stackView.pop(null);
				}
			}

			RangeSelector {
				id: episodeSelector
				template: "Episode %1/%2"
				Layout.fillWidth: true
				visible: false
				onCurrentChanged: {
					var episodeDetail = stackView.findWidget("episodedetail");

					if (stackView.currentItem === episodeDetail) {
						stackView.push({
										   item: episodeDetailCompo,
										   replace: true,
										   properties: {
											   show: episodeDetail.show,
											   season: episodeDetail.season,
											   episode: current
										   }
									   });
					}

				}
				onCloseMe: {
					var episodesViewer = stackView.findWidget("episodes");

					selectorMorpher.current = current;
					selectorMorpher.visible = true;
					selectorMorpher.initReturnStart(episodeSelector);
					episodeSelector.opacity = 0;

					var coord = episodesViewer.getEpisodeItemCoordinates(current);
					selectorMorpherAnimation.initTarget(coord.x, coord.y - episodeSelector.height, coord.w, coord.h);
					selectorMorpherAnimation.start();
					stackView.pop();
				}
			}

			StackView {
				id: stackView
				clip: true
				Layout.fillWidth: true
				Layout.fillHeight: true
				delegate: MainStackViewDelegate {}
				initialItem: Item { // a blank item, used to display the very first transition
					property string widgetType : "seasons"
				}
				function findWidget(widgetType) {
					return find(function(item, index) {
						return item.widgetType === widgetType;
					}, true);
				}
			}

			ParallelAnimation {
				id: selectorMorpherAnimation
				function initTarget(x, y, w, h) {
					selectorMorpherX.to = x;
					selectorMorpherY.to = y;
					selectorMorpherW.to = w;
					selectorMorpherH.to = h;
				}
				onStopped: {
					selectorMorpher.visible = false;
				}
				NumberAnimation {
					id: selectorMorpherX
					target: selectorMorpher
					property: "x"
					easing.type: Easing.InOutQuad
					duration: morphAnimationTime
				}
				NumberAnimation {
					id: selectorMorpherY
					target: selectorMorpher
					property: "y"
					easing.type: Easing.InOutQuad
					duration: morphAnimationTime
				}
				NumberAnimation {
					id: selectorMorpherW
					target: selectorMorpher
					property: "width"
					easing.type: Easing.InOutQuad
					duration: morphAnimationTime
				}
				NumberAnimation {
					id: selectorMorpherH
					target: selectorMorpher
					property: "height"
					easing.type: Easing.InOutQuad
					duration: morphAnimationTime
				}
			}
			Loader {
				id: loadingWidget
				anchors.centerIn: stackView
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
		id: selectorMorpher

		function initReturnStart(selector) {
			var ar = selector.mapToItem(null, 0, 0);
			x = ar.x;
			y = ar.y;
			width = selector.width;
			height = selector.height;
			template = selector.template;
		}

		visible: false
	}

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

