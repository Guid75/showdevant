import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Item {
	id: scrollTextItem
	property alias text : text.text
	clip: true
	ScrollView {
		id: scrollView
		anchors.fill: parent
		Text {
			id: text
			width: scrollTextItem.width - 9
			clip: true
			wrapMode: Text.WordWrap
			renderType: Text.NativeRendering
			MouseArea {
				id: textMouseArea
				anchors.fill: parent
				hoverEnabled: true
			}
		}
		style: ScrollViewStyle {
			handle: Rectangle {
				opacity: textMouseArea.containsMouse || styleData.hovered ? 1 : 0.2
				Behavior on opacity {
					NumberAnimation { duration: 400; easing.type: Easing.InOutQuad }
				}
				radius: 3
				implicitHeight: 7
				implicitWidth: 7
				color: "#888888"
			}
			scrollBarBackground: Rectangle {
				opacity: 0
				implicitHeight: 7
				implicitWidth: 7
			}
			handleOverlap: 5
			corner: Rectangle {
				color: "green"
				width: 7
				height: 7
			}
			incrementControl: Rectangle {
				color: "red"
				opacity: 0
				width: 7
				height: 7
			}
			decrementControl: Rectangle {
				color: "blue"
				opacity: 0
				width: 7
				height: 7
			}
		}
	}
}
