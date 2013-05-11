import QtQuick 2.0

ShadowRectangle {
	property alias model : listView.model
	property string searchText : ""
	width: 100
	antialiasing: true
	color: "#FFFFFF"

	Component {
		id: searchDelegate
		Rectangle {
			id: itemRectangle
			state: "normal"
			width: parent.width
			color: mouseAreaItem.containsMouse ? "#EEEEEE" : "#00000000"
			height: 20
			MouseArea {
				id: mouseAreaItem
				anchors.fill: parent
				onClicked: {
					listView.currentIndex = index
				}
				hoverEnabled: true
			}
			Text {
				id: itemText
				anchors.verticalCenter: parent.verticalCenter
				x: 8
				width: listView.width
				height: 10
				text: display
				font.bold: true
				elide: Text.ElideRight
			}
		}
	}

	height: listView.contentHeight + resumeRectangle.height
	Rectangle {
		id: resumeRectangle
		height: 20
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		color: "#BBBBBB"
		Text {
			anchors.verticalCenter: parent.verticalCenter
			color: "#222222"
			id: resumeText
			x: 8
			text: 'TV shows relating to "' + searchText + '"'
		}
	}

	ListView {
		anchors.top: resumeRectangle.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		id: listView
		delegate: searchDelegate
	}
}
