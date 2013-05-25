import QtQuick 2.0
import com.guid75 1.0
import "showmanager.js" as ShowManager

Rectangle {
	color: "#00000000"
	property alias model : listView.model
	signal showSelected(string showId, string title)

	Component {
		id: showDelegate
		Rectangle {
			width: parent.width
			height: 20
			color: itemMouseArea.containsMouse ? "white" : (listView.currentIndex == index ? "#FFEECC" : "#DDDDDD")
			clip: true
			Text {
				x: 8
				anchors.verticalCenter: parent.verticalCenter
				width: parent.width
				text: model.title
				elide: Text.ElideRight
			}
			MouseArea {
				cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
				id: itemMouseArea
				anchors.fill: parent
				hoverEnabled: true
				onClicked: {
					listView.currentIndex = index;
					showSelected(model.show_id, model.title);
				}
			}
		}
	}

	Component {
		id: showSectionDelegate
		Rectangle {
			gradient: Gradient {
				GradientStop { position: 0.0; color: "#AAAAAA" }
				GradientStop { position: 1.0; color: "#CCCCCC" }
			}
			border.color: "#888888"
			width: parent.width
			height: 20
			clip: true
			Text {
				x: 2
				anchors.verticalCenter: parent.verticalCenter
				horizontalAlignment: Text.AlignLeft
				width: parent.width
				font.capitalization: Font.Capitalize
				font.bold: true
				text: section
			}
		}
	}

	ListView {
		id: listView
		currentIndex: -1
		add: Transition {
			NumberAnimation { properties: "x,y"; duration: 200 }
		}
		anchors {
			fill: parent
			leftMargin: 4
			rightMargin: 4
		}
		section.property: "title"
		section.criteria: ViewSection.FirstCharacter
		section.delegate: showSectionDelegate
		delegate: showDelegate
	}
}
