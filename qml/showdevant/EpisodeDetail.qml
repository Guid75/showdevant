import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Rectangle {
	id: episodeDetail
	property string showId
	property int season
	property int episode
	readonly property string widgetType : "episodedetail"

	Component.onCompleted: {
		//		bannerImage.source = "http://api.betaseries.com/pictures/episode/%1.jpg?season=%2&episode=%3&key=9adb4ab628c6".arg(showId).arg(season).arg(episode);
	}

	//color: "#DDDDDD"

	TabView {
		id: frame
		anchors.fill: parent
		//		anchors.margins: 4
		Tab {
			title: "Episode resume"
			Item {
				EpisodeResume {

				}
			}
		}
		Tab {
			title: "Subtitles"
		}

		style: TabViewStyle {
			frameOverlap: 1
			tab: Rectangle {
				color: "#CCCCCC"
				implicitWidth: Math.max(text.width + 4, 80)
				implicitHeight: 25
				Rectangle {
					anchors {
						fill: parent
						topMargin: 1
						leftMargin: 1
						rightMargin: 1
						bottomMargin: tab.selected ? 0 : 1
					}
					color: tab.selected ? "#DDDDDD" :"#EEEEEE"
					implicitWidth: Math.max(text.width + 4, 80)
					implicitHeight: 25
					Text {
						id: shadowText
						color: "white"
						x: text.x + 1
						y: text.y + 1
						renderType: Text.NativeRendering
						text: tab.title
					}
					Text {
						id: text
						renderType: Text.NativeRendering
						anchors.centerIn: parent
						text: tab.title
					}
				}
			}
			frame: Rectangle {
				border.color: "#CCCCCC"
				color: "#DDDDDD"
			}
		}
	}
}
