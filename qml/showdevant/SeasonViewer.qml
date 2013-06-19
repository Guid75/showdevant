import QtQuick 2.0
import QtQuick.Controls 1.0
import com.guid75 1.0

Item {
	id: seasonViewer

	signal closeMe();

	property string show
	property int currentSeason
	property int seasonCount
	readonly property string widgetType : "season"

	Timer {
		id: launchTimer
		interval: 1
		onTriggered: {
			episodesStackView.push(episodesViewer);
		}
	}

	Component.onCompleted: {
		// push the first card
		episodeListModel.show = show;
		episodeListModel.season = currentSeason;
		episodesStackView.push({ item: blankPage, immediate: true });
		currentEpisodeListModel = episodeListModel;
		launchTimer.start();
	}

	EpisodeListModel {
		id: episodeListModel
	}

	EpisodeListModel {
		id: episodeListModelAlt
	}

	Component {
		id: blankPage
		Item {
			property string widgetType : "blank"
		}
	}

	Component {
		id: episodesViewer
		EpisodesViewer {
			model: episodeListModel
		}
	}

	Component {
		id: episodesViewerAlt
		EpisodesViewer {
			model: episodeListModelAlt
		}
	}

	property EpisodeListModel currentEpisodeListModel;

	function __toggleCurrentEpisodeListModel() {
		currentEpisodeListModel = __getNextEpisodeListModel();
	}

	function __getNextEpisodeListModel() {
		if (currentEpisodeListModel === episodeListModel)
			return episodeListModelAlt;
		return episodeListModel;
	}

	function __getNextEpisodesViewer() {
		if (currentEpisodeListModel === episodeListModel)
			return episodesViewerAlt;
		return episodesViewer;
	}

	function getSelectorCoordinates() {
		var ar = seasonSelector.mapToItem(null, 0, 0);
		return {
			x: ar.x,
			y: ar.y,
			w: seasonSelector.width,
			h: seasonSelector.height
		}
	}

	function showSelector() {
		seasonSelector.opacity = 1;
	}

	RangeSelector {
		id: seasonSelector
		opacity: 0
		template: "Season %1/%2"
		min: 1
		max: seasonCount
		current: currentSeason
		anchors {
			top: parent.top
			left: parent.left
			right: parent.right
		}
		onCurrentIndexChanged: {
			currentSeason = current;

			var model = __getNextEpisodeListModel();
			var viewer = __getNextEpisodesViewer();

			model.show = show;
			model.season = current;

			if (!episodesStackView.currentItem || episodesStackView.currentItem.widgetType === 'episodes') {
				episodesStackView.push({ item: viewer, replace: true });
				__toggleCurrentEpisodeListModel();
			}
		}
		onCloseMe: {
			seasonViewer.closeMe();
		}
	}

	StackView {
		id: episodesStackView
		clip: true
		anchors {
			top: seasonSelector.bottom
			left: parent.left
			right: parent.right
			bottom: parent.bottom
		}
		delegate: StackViewDelegate {
			function getTransition(properties)
			{
				if (properties.exitItem.widgetType === 'blank') {
					return firstTransition;
				}
				if (properties.enterItem.model.season < properties.exitItem.model.season)
					return toRightTransition;
				return toLeftTransition;
			}

			function transitionFinished(properties)
			{
				properties.exitItem.x = 0
				properties.exitItem.y = 0
			}

			property Component firstTransition: StackViewTransition {
				PropertyAnimation {
					target: enterItem
					property: "y"
					from: target.height
					to: 0
					duration: 300
				}
				PropertyAnimation {
					target: exitItem
					property: "y"
					from: 0
					to: -target.height
					duration: 500
				}
			}

			property Component toLeftTransition: StackViewTransition {
				PropertyAnimation {
					target: enterItem
					property: "x"
					from: target.width
					to: 0
					duration: 300
				}
				PropertyAnimation {
					target: exitItem
					property: "x"
					from: 0
					to: -target.width
					duration: 300
				}
			}

			property Component toRightTransition: StackViewTransition {
				PropertyAnimation {
					target: enterItem
					property: "x"
					from: -target.width
					to: 0
					duration: 300
				}
				PropertyAnimation {
					target: exitItem
					property: "x"
					from: 0
					to: target.width
					duration: 300
				}
			}
		}
	}
}
