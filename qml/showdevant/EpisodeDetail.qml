import QtQuick 2.0

Rectangle {
	property string showId
	property int season
	property int episode
	readonly property string widgetType : "episodedetail"

	Component.onCompleted: {
//		bannerImage.source = "http://api.betaseries.com/pictures/episode/%1.jpg?season=%2&episode=%3&key=9adb4ab628c6".arg(showId).arg(season).arg(episode);
	}

	color: "white"
	Rectangle {
		id: resumeRectangle
		anchors {
			left: parent.left
			top: parent.top
			margins: 4
		}
		width: bannerImage.paintedWidth
		height: bannerImage.paintedHeight
		color: "#DDDDDD"
		Image {
			id: bannerImage
			source: showId && season && episode ? "http://api.betaseries.com/pictures/episode/%1.jpg?season=%2&episode=%3&key=9adb4ab628c6".arg(showId).arg(season).arg(episode) : ""
//			anchors.fill: parent
			fillMode: Image.PreserveAspectCrop
		}
/*		Rectangle {
			id: backgroundItem
			anchors {
				left: parent.left
				right: parent.right
				bottom: parent.bottom
			}
			color: "black"
			opacity: bannerText.text == "" ? 0.0 : 0.7
			height: 30
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
				color: "white"
			}
		}*/
	}

}
