import QtQuick 2.0
import com.guid75 1.0

Item {
	Flickable {
		id: flickable
		anchors.fill: parent
		signal seasonSelected(int season)
		readonly property string widgetType : "seasons"

		contentWidth: Math.max(width, 208)
		contentHeight: flowEpisode.childrenRect.height + 8
		clip: true

		Component.onCompleted: {
			flowAnimation.start();
		}

		Connections {
			target: seasonListModel
			onModelReset: flowAnimation.start()
		}

		Flow {
			id: flowEpisode
			x: 4
			anchors {
				left: parent.left
				right: parent.right
			}
			anchors.margins: 4
			spacing: 10
			Repeater {
				id: repeaterEpisode
				model: seasonListModel
				SeasonItem {
					onItemClicked: {
						episodeListModel.show = show_id;
						episodeListModel.season = number;
						seasonSelector.current = number;
						seasonSelector.min = 1;
						seasonSelector.max = seasonListModel.count;
						seasonSelector.visible = true;
						seasonSelector.height = 40;
						playgroundLoader.setSource("EpisodesViewer.qml", {
													   currentSeason: number,
													   currentShow: show_id
												   });
					}
				}
			}
		}
		PropertyAnimation {
			id: flowAnimation
			target: flowEpisode
			property: 'y'
			from: parent ? parent.height : 0
			to: 4
			duration: 300
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
		active: seasonListModel.synchronizeState === CacheWatcher.Synchronizing
		asynchronous: true
	}
}
