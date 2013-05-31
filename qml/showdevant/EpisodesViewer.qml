import QtQuick 2.0
import com.guid75 1.0

Item {
	clip: true
	property int currentSeason
	property string currentShow
	property int futureSeason
	readonly property string widgetType : "episodes"

	Component.onCompleted: {
		currentModel.season = currentSeason;
		// pre-caching the previous and next season
		cache.synchronizeSeasonEpisodeList(currentShow, currentSeason - 1);
		cache.synchronizeSeasonEpisodeList(currentShow, currentSeason + 1);
		constructionAnimation.start();
	}

	function __launchAnimation(season) {
		var toLeft = currentSeason > season;
		currentSeason = season;
		if (toLeft)
			toLeftAnimation.start();
		else
			toRightAnimation.start();
	}

	function __itemClicked(episode) {
		episodeSelector.current = episode;
		episodeSelector.min = 1;
		episodeSelector.max = currentModel.count;
		episodeSelector.visible = true;
		episodeSelector.height = 40;
		playgroundLoader.setSource("EpisodeDetail.qml", {
									   showId: currentModel.show,
									   season: currentModel.season,
									   episode: episode
								   });
	}

	function setCurrentSeason(season) {
		if (currentSeason === season)
			return;
		futureSeason = season;

		// is animation is running, we stop here
		if (toLeftAnimation.running || toRightAnimation.running)
			return;

		// pre-caching the previous and next season
		cache.synchronizeSeasonEpisodeList(currentShow, season - 1);
		cache.synchronizeSeasonEpisodeList(currentShow, season + 1);

		__launchAnimation(futureSeason);
	}

	EpisodeListModel {
		id: previousModel
	}
	Binding {
		target: previousModel
		property: "show"
		value: currentShow
	}
	EpisodeListModel {
		id: currentModel
	}
	Binding {
		target: currentModel
		property: "show"
		value: currentShow
	}
	EpisodeListModel {
		id: nextModel
	}
	Binding {
		target: nextModel
		property: "show"
		value: currentShow
	}

	Item {
		id: previousItem
		width: parent.width
		height: parent.height
		x: - parent.width
		Flickable {
			anchors.fill: parent
			contentWidth: Math.max(width, 208)
			contentHeight: previousFlow.childrenRect.height + 8
			clip: true
			Flow {
				id: previousFlow
				anchors.fill: parent
				anchors.margins: 4
				spacing: 10
				Repeater {
					model: previousModel
					EpisodeItem {
						onItemClicked: __itemClicked(episode)
					}
				}
			}
		}
		Loader {
			id: previousLoadingWidget
			anchors.centerIn: parent
			sourceComponent: LoadingWidget {
				anchors.centerIn: parent
				color: "#AAAAAA"
				count: 20
				size: 16
				width: 200
				height: 200
			}
			active: previousModel.synchronizing
			asynchronous: true
		}
	}

	Item {
		id: currentItem
		width: parent.width
		height: parent.height
		anchors.left: previousItem.right
		Flickable {
			anchors.fill: parent
			contentWidth: Math.max(width, 208)
			contentHeight: currentFlow.childrenRect.height + 8
			clip: true
			Flow {
				id: currentFlow
				anchors {
					left: parent.left
					right: parent.right
				}
				height: parent.height
				anchors.margins: 4
				spacing: 10
				Repeater {
					model: currentModel
					EpisodeItem {
						onItemClicked: __itemClicked(episode)
					}
				}
			}
		}
		Loader {
			id: currentLoadingWidget
			anchors.centerIn: parent
			sourceComponent: LoadingWidget {
				anchors.centerIn: parent
				color: "#AAAAAA"
				count: 20
				size: 16
				width: 200
				height: 200
			}
			active: currentModel.synchronizing
			asynchronous: true
		}
	}

	Item {
		width: parent.width
		height: parent.height
		anchors.left: currentItem.right
		Flickable {
			anchors.fill: parent
			contentWidth: Math.max(width, 208)
			contentHeight: nextFlow.childrenRect.height + 8
			clip: true
			Flow {
				id: nextFlow
				anchors.fill: parent
				anchors.margins: 4
				spacing: 10
				Repeater {
					model: nextModel
					EpisodeItem {
						onItemClicked: __itemClicked(episode)
					}
				}
			}
		}
		Loader {
			id: nextLoadingWidget
			anchors.centerIn: parent
			sourceComponent: LoadingWidget {
				anchors.centerIn: parent
				color: "#AAAAAA"
				count: 20
				size: 16
				width: 200
				height: 200
			}
			active: nextModel.synchronizing
			asynchronous: true
		}
	}

	PropertyAnimation {
		id: constructionAnimation
		target: currentFlow
		property: 'y'
		from: parent ? parent.height : 0
		to: 4
		duration: 300
	}

	PropertyAnimation {
		id: toLeftAnimation
		target: previousItem
		property: 'x'
		from: parent ? - parent.width : 0
		to: 0
		duration: 400
		onStarted: {
			previousModel.season = currentSeason;
		}
		onStopped: {
			currentModel.season = currentSeason;
			if (futureSeason !== currentSeason) {
				__launchAnimation(futureSeason);
			}
		}
	}

	PropertyAnimation {
		id: toRightAnimation
		target: previousItem
		property: 'x'
		from: parent ? - parent.width : 0
		to: parent ? - 2 * parent.width : 0
		duration: 400
		onStarted: {
			nextModel.season = currentSeason;
		}
		onStopped: {
			currentModel.season = currentSeason;
			if (futureSeason !== currentSeason)
				__launchAnimation(futureSeason);
		}
	}
}

