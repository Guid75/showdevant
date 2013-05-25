import QtQuick 2.0

Flickable {
	contentWidth: Math.max(width, 208)
	contentHeight: flow.childrenRect.height + 8
	clip: true
	Flow {
		id: flow
		anchors.fill: parent
		anchors.margins: 4
		spacing: 10
/*		add: Transition {
			NumberAnimation {
				from: 0.0
				to: 1.0
				easing.type: Easing.OutCirc
				properties: "opacity"
				duration: 500
			}
		}
		move: Transition {
			NumberAnimation {
				from: 0.0
				to: 1.0
				easing.type: Easing.OutCirc
				properties: "opacity"
				duration: 500
			}
		}*/

/*		add: Transition {
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
		}*/
		Repeater {
			id: repeaterEpisode
			model: episodeModel
			EpisodeItem {
				onItemClicked: {
					episodeSelector.current = episode;
					episodeSelector.min = 1;
					episodeSelector.max = episodeModel.count;
					episodeSelector.visible = true;
					episodeSelector.height = 40;
					mouse.accepted = false;
				}
			}
		}
	}
}
