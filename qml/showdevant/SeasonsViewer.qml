import QtQuick 2.0

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
						episodeModel.show = show_id;
						episodeModel.season = number;
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
			color: "#666666"
			size: 8
			width: 200
			height: 200
		}
		active: seasonListModel.synchronizing
		asynchronous: true
	}
}
