import QtQuick 2.0
import QtQuick.Controls 1.0
import com.guid75 1.0
import "showmanager.js" as ShowManager

Rectangle {
	id: showList
	color: "#00000000"
	property alias model : listView.model
	property alias currentShowIndex : listView.currentIndex
	signal showSelected(string showId, string title)

	function overDropDown(x, y) {
		var a = listView.mapFromItem(null, x, y)
		var item = listView.itemAt(a.x, a.y);

		if (!item)
			return false;

		a = item.mapFromItem(null, x, y);
		var image = item.childAt(a.x, a.y);
		if (!image)
			return false;

		return image && image.objectName === 'dropDownImage';
	}

	Component {
		id: showDelegate
		Rectangle {
			id: showItem
			width: parent.width
			height: 20
			color: (itemMouseArea.containsMouse || dropDownMouseArea.containsMouse) ? "white" : (ListView.isCurrentItem ? "#FFEECC" : "#DDDDDD")
			clip: true
			Text {
				x: 8
				anchors.verticalCenter: parent.verticalCenter
				width: parent.width
				text: model.title
				elide: Text.ElideRight
				renderType: Text.NativeRendering
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
			Image {
				id: dropDownImage
				objectName: "dropDownImage"
				visible: /*authenticator.isLogged() && */itemMouseArea.containsMouse// || dropDownMouseArea.containsMouse
				anchors {
					right: parent.right
					verticalCenter: parent.verticalCenter
					rightMargin: 2
				}
				source: "dropdown.png"
				MouseArea {
					id: dropDownMouseArea
					anchors.fill: parent
					//					hoverEnabled: true
					onClicked: {
						listView.currentIndex = index;
						var item = showItem;
						var ar = item.mapToItem(null, 0, 0);
						var bubbleCenter = ar.y + item.height / 2;
						bubbleMenu.bubbleY = bubbleCenter - bubbleMenu.bubbleHeight / 2;
						if (!bubbleMenu.visible) {
							bubbleMenu.visible = true;
						}

					}
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
				renderType: Text.NativeRendering
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
		}
		section.property: "title"
		section.criteria: ViewSection.FirstCharacter
		section.delegate: showSectionDelegate
		delegate: showDelegate
	}
	Loader {
		id: loadingMask
		anchors.fill: parent
		sourceComponent: LoadingMask {
		}
		active: myShowsModel.synchronizeState === CacheWatcher.Synchronizing
		asynchronous: true
	}
}
