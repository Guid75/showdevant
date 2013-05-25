import QtQuick 2.0

Flickable {
	signal seasonSelected(int season)
	contentWidth: Math.max(width, 208)
	contentHeight: flowEpisode.childrenRect.height + 8
	clip: true
	Flow {
		id: flowEpisode
		anchors.fill: parent
		anchors.margins: 4
		spacing: 10
		add: Transition {
			NumberAnimation {
				easing.type: Easing.OutCirc
				properties: "x,y"
				duration: 500
			}
		}
		move: Transition {
			NumberAnimation {
				easing.type: Easing.OutCirc
				properties: "x,y"
				duration: 500
			}
		}
		Repeater {
			id: repeaterEpisode
			model: seasonModel
			SeasonItem {
				MouseArea {
					anchors.fill: parent
					onClicked: {
						episodeModel.show = show_id;
						episodeModel.season = number;
						seasonSelector.current = number;
						seasonSelector.min = 1;
						seasonSelector.max = seasonModel.count;
						seasonSelector.visible = true;
						seasonSelector.height = 40;
						mouse.accepted = false;
						playgroundLoader.source = "EpisodesFlickable.qml"
					}
				}
			}
		}
	}
}
