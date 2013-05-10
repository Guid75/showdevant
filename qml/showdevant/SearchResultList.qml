import QtQuick 2.0

ShadowRectangle {
	property alias model : listView.model
	width: 100
	height: 200
	antialiasing: true
	color: "#FFFFFF"

	Component {
		id: searchDelegate
		Rectangle {
			id: itemRectangle
			state: "normal"
			width: parent.width
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
				x: 0
				anchors.verticalCenter: parent.verticalCenter
				//anchors.fill: parent
				width: listView.width
				height: 10
				/*anchors.verticalCenter: parent.verticalCenter
						anchors.horizontalCenter: parent.horizontalCenter*/
				text: display
				elide: Text.ElideRight
			}
		}
	}

	ListView {
		anchors.fill: parent
		id: listView
		delegate: searchDelegate
	}
}
