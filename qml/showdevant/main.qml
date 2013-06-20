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
	property int morphInitX
	property int morphInitY
	property int morphInitW
	property int morphInitH

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

	////////////////////////// SEASONS

	// -1 means that no seasons viewer has been showed yet
	property int currentSeasonsViewerIndex : -1

	function __toggleCurrentSeasonsViewer() {
		currentSeasonsViewerIndex = (currentSeasonsViewerIndex + 1) % 2;
	}

	function __getNextSeasonsViewerCompo() {
		if (currentSeasonsViewerIndex === 0)
			return altSeasonsViewerCompo;
		return seasonsViewerCompo;
	}

	function __getCurrentSeasonsViewer() {
		return stackView.find(function(item, index) {
			return item.widgetType === 'seasons';
		}, true);
	}

	Component {
		id: seasonsViewerCompo
		SeasonsViewer {
			onSeasonClicked: __seasonClicked(season, episodeCount)
		}
	}

	Component {
		id: altSeasonsViewerCompo
		SeasonsViewer {
			onSeasonClicked: __seasonClicked(season, episodeCount)
		}
	}

	////////////////////////// EPISODES

	property int currentEpisodesViewerIndex : -1

	function __toggleCurrentEpisodesViewer() {
		currentEpisodesViewerIndex = (currentEpisodesViewerIndex + 1) % 2;
	}

	function __getNextEpisodesViewerCompo() {
		if (currentEpisodesViewerIndex === 0)
			return altEpisodesViewerCompo;
		return episodesViewerCompo;
	}

	function __getCurrentEpisodesViewer() {
		return stackView.find(function(item, index) {
			return item.widgetType === 'episodes';
		}, true);
	}

	Component {
		id: episodesViewerCompo
		EpisodesViewer {
			onEpisodeClicked: __episodeClicked(episode)
		}
	}

	Component {
		id: altEpisodesViewerCompo
		EpisodesViewer {
			onEpisodeClicked: __episodeClicked(episode)
		}
	}

	////////////////////////// EPISODE DETAILS

	property int currentEpisodeDetailIndex : -1

	function __toggleCurrentEpisodeDetail() {
		currentEpisodeDetailIndex = (currentEpisodeDetailIndex + 1) % 2;
	}

	function __getNextEpisodeDetailCompo() {
		if (currentEpisodeDetailIndex === 0)
			return altEpisodeDetailCompo;
		return episodeDetailCompo;
	}

	function __getCurrentEpisodeDetail() {
		return stackView.find(function(item, index) {
			return item.widgetType === 'episodedetail';
		}, true);
	}

	Component {
		id: episodeDetailCompo
		EpisodeDetail {
		}
	}

	Component {
		id: altEpisodeDetailCompo
		EpisodeDetail {
		}
	}

	/////////////////////////////////////////

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
		showList.showSelected("lost", "Lost");
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

	function __seasonClicked(season, episodeCount) {
		var seasonsViewer = __getCurrentSeasonsViewer();

		stackView.push({
						   item: episodesViewerCompo,
						   properties: {
							   show: seasonsViewer.show,
							   season: season
						   }
					   });

		__precacheEpisodes(seasonsViewer.show, season, seasonsViewer.model.count);

		currentEpisodesViewerIndex = 0;

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
		var episodesViewer = __getCurrentEpisodesViewer();

		stackView.push({
						   item: episodeDetailCompo,
						   properties: {
							   show: episodesViewer.show,
							   season: episodesViewer.season,
							   episode: episode
						   }
					   });

		currentEpisodeDetailIndex = 0;

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

	function __precacheEpisodes(show, season, seasonMax) {
		if (season > 1)
			cache.synchronizeEpisodes(show, season - 1);
		if (season < seasonMax)
			cache.synchronizeEpisodes(show, season + 1);
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
				onShowSelected: {
					var seasonsViewer = stackView.get(0, null);
					if (seasonsViewer && seasonsViewer.model.show === showId)
						return;

					// pop all components until only one remains
					stackView.pop(null);

					seasonSelector.visible = false;
					episodeSelector.visible = false;

					var compoToPush = __getNextSeasonsViewerCompo();

					if (!stackView.currentItem || stackView.currentItem.widgetType === 'seasons') {
						var item = stackView.push({
													  item: compoToPush,
													  replace: true,
													  properties: {
														  show: showId
													  }
												  });
						__toggleCurrentSeasonsViewer();
					}

					bannerImage.source = "http://api.betaseries.com/pictures/show/" + showId + ".jpg?key=9adb4ab628c6";
					bannerText.text = title;
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
				onCurrentIndexChanged: {
					episodeSelector.current = 1;

					var episodesViewer = __getCurrentEpisodesViewer();

					// preload surrounding episodes
					__precacheEpisodes(episodesViewer.show, current, max);

					if (stackView.currentItem.widgetType === 'episodes') {
						var compoToPush = __getNextEpisodesViewerCompo();
						var item = stackView.push({
													  item: compoToPush,
													  replace: true,
													  properties: {
														  show: episodesViewer.show,
														  season: current
													  }
												  });
						__toggleCurrentEpisodesViewer();
					}
				}
				onCloseMe: {
					var seasonsViewer = __getCurrentSeasonsViewer();
					var noMorph;

					currentEpisodesViewerIndex = 0;

					if (stackView.currentItem.widgetType === 'episodedetail')
						noMorph = true;

					stackView.pop(null);

					if (noMorph)
						return;

					selectorMorpher.current = current;
					selectorMorpher.visible = true;
					selectorMorpher.initReturnStart(seasonSelector);
					seasonSelector.opacity = 0;

					var coord = seasonsViewer.getSeasonItemCoordinates(current);
					selectorMorpherAnimation.initTarget(coord.x, coord.y - seasonSelector.height, coord.w, coord.h);
					selectorMorpherAnimation.start();
				}
			}

			RangeSelector {
				id: episodeSelector
				template: "Episode %1/%2"
				Layout.fillWidth: true
				visible: false
				onCurrentIndexChanged: {
					var episodeDetail = __getCurrentEpisodeDetail();

					if (stackView.currentItem.widgetType === 'episodedetail') {
						var compoToPush = __getNextEpisodeDetailCompo();
						var item = stackView.push({
													  item: compoToPush,
													  replace: true,
													  properties: {
														  show: episodeDetail.show,
														  season: episodeDetail.season,
														  episode: current
													  }
												  });
						__toggleCurrentEpisodeDetail();
					}

				}
				onCloseMe: {					
					var episodesViewer = __getCurrentEpisodesViewer();

					currentEpisodeDetailIndex = 0;

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
				//anchors.centerIn: playgroundLoader
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

