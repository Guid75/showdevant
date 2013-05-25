import QtQuick 2.0

Flickable {
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
					propagateComposedEvents: true
					onClicked: {
						// TODO animation to bring the season above
						seasonsFlickable.visible = false;
						episodesFlickable.visible = true;
						episodeModel.show = show_id;
						episodeModel.season = number;
						seasonSelector.currentSeason = number;
						seasonSelector.minSeason = 1;
						seasonSelector.maxSeason = seasonModel.count;
						seasonSelector.visible = true;
						seasonSelector.height = 40;
						mouse.accepted = false;
					}
				}
			}
		}
	}
}
