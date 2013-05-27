import QtQuick 2.0
import com.guid75 1.0

Item {
	clip: true
	property int currentSeason
	property int futureSeason
	readonly property string widgetType : "episodes"

	Component.onCompleted: {
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

	function setCurrentSeason(season) {
		if (currentSeason === season)
			return;
		futureSeason = season;

		// is animation is running, we stop here
		if (toLeftAnimation.running || toRightAnimation.running)
			return;

		__launchAnimation(futureSeason);
	}

	EpisodeModel {
		id: previousModel
		show: "dexter"
		season: 1
	}
	EpisodeModel {
		id: currentModel
		show: "dexter"
		season: 2
	}
	EpisodeModel {
		id: nextModel
		show: "dexter"
		season: 3
	}

	Flickable {
		id: previousFlickable
		width: parent.width
		height: parent.height
		x: - parent.width
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
//				model: episodeModel
				EpisodeItem {
					onItemClicked: {
	/*					episodeSelector.current = episode;
						episodeSelector.min = 1;
						episodeSelector.max = episodeModel.count;
						episodeSelector.visible = true;
						episodeSelector.height = 40;
						playgroundLoader.setSource("EpisodeDetail.qml", {
													   showId: episodeModel.show,
													   season: episodeModel.season,
													   episode: episode
												   });*/
					}
				}
			}
		}
	}

	Flickable {
		id: currentFlickable
		width: parent.width
		height: parent.height
		anchors.left: previousFlickable.right
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
//				model: episodeModel
				EpisodeItem {
					onItemClicked: {
	/*					episodeSelector.current = episode;
						episodeSelector.min = 1;
						episodeSelector.max = episodeModel.count;
						episodeSelector.visible = true;
						episodeSelector.height = 40;
						playgroundLoader.setSource("EpisodeDetail.qml", {
													   showId: episodeModel.show,
													   season: episodeModel.season,
													   episode: episode
												   });*/
					}
				}
			}
		}
	}

	Flickable {
		id: nextFlickable
		width: parent.width
		height: parent.height
		anchors.left: currentFlickable.right
		contentWidth: Math.max(width, 208)
		contentHeight: nextFlow.childrenRect.height + 8
		clip: true
		Flow {
			id: nextFlow
			anchors.fill: parent
			anchors.margins: 4
			spacing: 10
			Repeater {
//				model: episodeModel
				model: nextModel
				EpisodeItem {
					onItemClicked: {
	/*					episodeSelector.current = episode;
						episodeSelector.min = 1;
						episodeSelector.max = episodeModel.count;
						episodeSelector.visible = true;
						episodeSelector.height = 40;
						playgroundLoader.setSource("EpisodeDetail.qml", {
													   showId: episodeModel.show,
													   season: episodeModel.season,
													   episode: episode
												   });*/
					}
				}
			}
		}
	}

	PropertyAnimation {
		id: constructionAnimation
		target: currentFlow
		property: 'y'
		from: parent ? parent.height : 0
		to: 4
		duration: 600
	}

	SequentialAnimation {
		id: toLeftAnimation
		PropertyAnimation {
			target: previousFlickable
			property: 'x'
			from: - parent.width
			to: 0
			duration: 400
		}
		onStarted: {
			previousModel.season = currentSeason;
		}
		onStopped: {
			currentModel.season = currentSeason;
			previousFlickable.x = - parent.width
			if (futureSeason !== currentSeason) {
				futureSeason = currentSeason;
				__launchAnimation(futureSeason);
			}
		}
	}

	SequentialAnimation {
		id: toRightAnimation
		PropertyAnimation {
			target: previousFlickable
			property: 'x'
			from: - parent.width
			to: - 2 * parent.width
			duration: 400
		}
		onStarted: {
			previousModel.season = currentSeason;
		}
		onStopped: {
			currentModel.season = currentSeason;
			previousFlickable.x = - parent.width
			if (futureSeason !== currentSeason) {
				futureSeason = currentSeason;
				__launchAnimation(futureSeason);
			}
		}
	}
}

