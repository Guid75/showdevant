import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.0
import com.guid75 1.0

Rectangle {
	id: episodeDetail
	property alias show: episodeDetailModel.show
	property alias season: episodeDetailModel.season
	property alias episode: episodeDetailModel.episode
	readonly property string widgetType : "episodedetail"

	EpisodeModel {
		id: episodeDetailModel
	}

	SubtitleListModel {
		id: subtitleListModel
		show: episodeDetail.show
		season: episodeDetail.season
		episode: episodeDetail.episode
	}

	Component.onCompleted: {
		//		bannerImage.source = "http://api.betaseries.com/pictures/episode/%1.jpg?season=%2&episode=%3&key=9adb4ab628c6".arg(show).arg(season).arg(episode);
	}

	//color: "#DDDDDD"

	EpisodeResume {
		id: episodeResume
		anchors {
			top: parent.top
			left: parent.left
			right: parent.right
		}
	}

	TabView {
		id: frame
		anchors {
			top: episodeResume.bottom
			left: parent.left
			right: parent.right
			bottom: parent.bottom
		}

		Tab {
			title: "Subtitles"
			SubtitlesArea {
			}
		}
		Tab {
			title: "Comment (%1)".arg(episodeDetailModel.get(0).comments)
			Item {
				Button {
					anchors.centerIn: parent
					text: "click me"
				}
			}
		}

		style: TabViewStyle {
			frameOverlap: 1
			tab: Rectangle {
				color: "#CCCCCC"
				implicitWidth: Math.max(text.width + 12, 80)
				implicitHeight: 25
				Rectangle {
					anchors {
						fill: parent
						topMargin: 1
						leftMargin: 1
						rightMargin: 1
						bottomMargin: styleData.selected ? 0 : 1
					}
					color: styleData.selected ? "#DDDDDD" :"#EEEEEE"
					implicitWidth: Math.max(text.width + 12, 80)
					implicitHeight: 25
					Text {
						id: shadowText
						color: "white"
						x: text.x + 1
						y: text.y + 1
						renderType: Text.NativeRendering
						text: styleData.title
					}
					Text {
						id: text
						renderType: Text.NativeRendering
						anchors.centerIn: parent
						text: styleData.title
					}
				}
			}
			frame: Rectangle {
				border.color: "#CCCCCC"
				color: "#DDDDDD"
			}
		}
	}
	//	Button {
	//		anchors {
	//			top: parent.top;
	//			left: parent.left
	//			right: parent.right
	//		}
	//		height: 20
	//		text: "click me!"
	//		onClicked: {
	//			console.log("model.show: " + episodeDetailModel.show);
	//			console.log("model.season: " + episodeDetailModel.season);
	//			console.log("model.episode: " + episodeDetailModel.episode);

	//			console.log("Model count: " + episodeDetailModel.count);
	//		}
	//	}
}
