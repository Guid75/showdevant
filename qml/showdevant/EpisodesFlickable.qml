import QtQuick 2.0

Flickable {
	contentWidth: Math.max(width, 208)
	contentHeight: flow.childrenRect.height + 8
	clip: true
	property int currentSeason;

	Component.onCompleted: {
		creationAnimation.start();
		currentSeason = episodeModel.season;
	}

	Connections {
		target: episodeModel
		onModelReset: {
			if (episodeModel.season < currentSeason)
				toRightAnimation.start();
			else if (episodeModel.season > currentSeason)
				toLeftAnimation.start()
			currentSeason = episodeModel.season;
		}
	}

	Flow {
		id: flow
		x: 4
		y: 4
		width: parent.width
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
					playgroundLoader.source = "EpisodeDetail.qml"
				}
			}
		}
	}
	PropertyAnimation {
		id: creationAnimation
		target: flow
		property: 'y'
		from: parent.height
		to: 4
		duration: 300
	}
	PropertyAnimation {
		id: toRightAnimation
		target: flow
		property: 'x'
		from: -parent.width
		to: 4
		duration: 300
	}
	PropertyAnimation {
		id: toLeftAnimation
		target: flow
		property: 'x'
		from: parent.width
		to: 4
		duration: 300
	}
}
