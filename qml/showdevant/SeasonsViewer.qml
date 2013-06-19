import QtQuick 2.0
import com.guid75 1.0

Item {
	signal seasonClicked(int season, int episodeCount); //, int x, int y, int w, int h)
	readonly property string widgetType : "seasons"
	property alias model : seasonModel
	property alias show : seasonModel.show
	property int flowMargins : 6

	function getSeasonItemCoordinates(season) {
		var i, len, item, coord;
		for (i = 0, len = repeaterSeasons.count; i < len; i++) {
			item = repeaterSeasons.itemAt(i);
			if (item.seasonNumber === season) {
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

	SeasonListModel {
		id: seasonModel
	}

	Flickable {
		id: flickable
		anchors.fill: parent
		signal seasonSelected(int season)

		contentWidth: Math.max(width, 200 + flowMargins * 2)
		contentHeight: flowSeasons.childrenRect.height + flowMargins * 2
		clip: true

		Flow {
			id: flowSeasons
			y: flowMargins
			anchors {
				left: parent.left
				right: parent.right
				leftMargin: flowMargins
				rightMargin: flowMargins
			}
			spacing: 10
			Repeater {
				id: repeaterSeasons
				model: seasonModel
				SeasonItem {
					id: seasonItem
					seasonNumber: number
					episodeCount: episode_count
					onItemClicked: {
						seasonClicked(number, episode_count);
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
			color: "#AAAAAA"
			count: 20
			size: 16
			width: 200
			height: 200
		}
		active: seasonModel.synchronizeState === CacheWatcher.Synchronizing
		asynchronous: true
	}
}
