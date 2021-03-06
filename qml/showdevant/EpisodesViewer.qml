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
	property int maxSeason
	property int flowMargins : 6

	function getEpisodeItemCoordinates(episode) {
		var i, len, item, coord, modelData;
		for (i = 0, len = episodeModel.count; i < len; i ++) {
			modelData = episodeModel.get(i);
			if (modelData.episode === episode) {
				item = repeaterEpisodes.itemAt(i);
				coord = item.mapToItem(null, 0, 0);
				return {
					x: coord.x,
					y: coord.y,
					w: item.width,
					h: item.height
				}
			}
		}
		return null;
	}

	EpisodeListModel {
		id: episodeModel
	}

	Component.onCompleted: {
		// pre-fetch surrounding seasons
//		if (season > 1)
//			cache.synchronizeEpisodes(show, season - 1);
//		if (season < maxSeason)
//			cache.synchronizeEpisodes(show, season + 1);
	}

	Flickable {
		anchors.fill: parent
		contentWidth: Math.max(width, 200 + 2 * flowMargins)
		contentHeight: flowEpisodes.childrenRect.height + flowMargins * 2
		clip: true
		Flow {
			id: flowEpisodes
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
				id: repeaterEpisodes
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
		active: model && model.synchronizeState === CacheWatcher.Synchronizing
		asynchronous: true
	}
}

