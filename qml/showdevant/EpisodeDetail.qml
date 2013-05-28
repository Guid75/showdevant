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
		Tab { title: "Episode resume" }
		Tab { title: "Subtitles" }

		style: TabViewStyle {
			frameOverlap: 1
			tab: Rectangle {
				color: tab.selected ? "#DDDDDD" :"#EEEEEE"
				border.color:  tab.selected ? "#DDDDDD" : "#CCCCCC"
				implicitWidth: Math.max(text.width + 4, 80)
				implicitHeight: 25
				Text {
					id: text
					renderType: Text.NativeRendering
					anchors.centerIn: parent
					text: tab.title
				}
			}

			frame: Rectangle { color: "#DDDDDD" }
		}
	}

	/*	Rectangle {
		id: resumeRectangle
		anchors {
			left: parent.left
			top: parent.top
			margins: 4
		}
		width: episodeImage.paintedWidth
		height: episodeImage.paintedHeight
		color: "#DDDDDD"
		Image {
			id: episodeImage
			width: 250
			source: showId && season && episode ? "http://api.betaseries.com/pictures/episode/%1.jpg?season=%2&episode=%3&key=9adb4ab628c6".arg(showId).arg(season).arg(episode) : ""
			fillMode: Image.PreserveAspectFit
		}
		Rectangle {
			id: backgroundItem
			anchors {
				left: episodeImage.left
				right: episodeImage.right
				bottom: episodeImage.bottom
			}
			height: 30
			color: "black"
			opacity: bannerText.text == "" ? 0.0 : 0.7
		}
		Item {
			anchors.fill: backgroundItem
			Text {
				id: bannerText
				anchors.fill: parent
				anchors.leftMargin: 6
				verticalAlignment: Text.AlignVCenter
				//font.bold: true
				font.pointSize: 12
				text: '"Dexter"'
				color: "white"
			}
		}
	}*/
}
