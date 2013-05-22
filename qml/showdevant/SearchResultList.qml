import QtQuick 2.0
import "showmanager.js" as ShowManager

Item {
	anchors.fill: parent
	property alias model : listView.model
	property string searchText : ""
	property alias listX : mainRectangle.x
	property alias listY : mainRectangle.y
	property alias listWidth : mainRectangle.width

	MouseArea {
		anchors.fill: parent
		propagateComposedEvents: true
		onPressed: {
			parent.destroy();
			mouse.accepted = false;
		}
	}

	ShadowRectangle {
		id: mainRectangle
		width: 100
		height: listView.contentHeight + resumeRectangle.height
		antialiasing: true
		color: "#FFFFFF"

		Component {
			id: searchDelegate
			Rectangle {
				id: itemRectangle
				state: "normal"
				width: parent.width
				color: mouseAreaItem.containsMouse || followMouseArea.containsMouse ? "#EEEEEE" : "#00000000"
				height: 20
				clip: true
				MouseArea {
					id: mouseAreaItem
					anchors.fill: parent
					hoverEnabled: true
					onClicked: {
					}
				}
				Text {
					id: itemText
					anchors.verticalCenter: parent.verticalCenter
					x: 8
					width: listView.width
					height: 10
					text: title
					font.bold: true
					elide: Text.ElideRight
				}
				Rectangle {
					id: followRectangle
					antialiasing: true
					anchors.right: parent.right;
					anchors.verticalCenter: parent.verticalCenter
					anchors.rightMargin: 4
					width: followText.contentWidth + 10
					height: 18
					radius: 4
					border.color: "#BBBBBB"
					color: followMouseArea.containsMouse ? "#00AA00" : "#AAFFAA"
					Text {
						id: followText
						anchors.verticalCenter: parent.verticalCenter
						anchors.horizontalCenter: parent.horizontalCenter
						font.pointSize: 8
						color: followMouseArea.containsMouse ? "#DDDDDD" : "#444444"
						text: "follow"
					}
					MouseArea {
						id: followMouseArea
						propagateComposedEvents: true
						anchors.fill: parent
						hoverEnabled: true
						cursorShape: "PointingHandCursor"
						onClicked: {
							showModel.addShow(model.showId, model.title);
							//ShowManager.addShow(model.showId, model.title);
							mouse.accepted = false;
						}
					}
				}
			}
		}

		Rectangle {
			id: resumeRectangle
			height: 20
			anchors.top: parent.top
			anchors.left: parent.left
			anchors.right: parent.right
			color: "#DDDDDD"
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
}
