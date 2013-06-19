import QtQuick 2.0
import com.guid75 1.0

Item {
	id: episodesViewer
	clip: true
	signal episodeClicked(int episode)

	readonly property string widgetType : "episodes"
	property alias model : episodeModel
	property alias show : episodeModel.show
	property alias season : episodeModel.season
	property int flowMargins : 6

	EpisodeListModel {
		id: episodeModel
	}

	function __itemClicked(episode) {
		//			episodeSelector.current = episode;
		//			episodeSelector.min = 1;
		//			episodeSelector.max = currentModel.count;
		//			episodeSelector.visible = true;
		//			episodeSelector.height = 40;
		//			playgroundLoader.setSource("EpisodeDetail.qml", {
		//										   showId: currentModel.show,
		//										   season: currentModel.season,
		//										   episode: episode
		//									   });
	}

	Component.onCompleted: {
	}

	Flickable {
		anchors.fill: parent
		contentWidth: Math.max(width, 200 + 2 * flowMargins)
		contentHeight: currentFlow.childrenRect.height + flowMargins * 2
		clip: true
		Flow {
			id: currentFlow
			y: flowMargins
			anchors {
				left: parent.left
				right: parent.right
				leftMargin: flowMargins
				rightMargin: flowMargins
			}
			height: parent.height
			spacing: 10
			Repeater {
				id: repeater
				model: episodeModel
				EpisodeItem {
					onItemClicked: {
						episodesViewer.episodeClicked(episode);
					}
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
		active: model ? model.synchronizeState === CacheWatcher.Synchronizing : false
		asynchronous: true
	}
}

