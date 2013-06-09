import QtQuick 2.0

ModalBox {
	id: myself
	anchors.fill: parent

	property alias message: mainText.text
	property alias buttons: buttonsRepeater.model
	property int padding : 10;

	signal quit(string buttonLabel)

	Rectangle {
		anchors.centerIn: parent
		width: Math.max(mainText.contentWidth + 40, (invisibleMetrics.width + (2 * padding)) * buttonsRepeater.count)

		height: 30 + mainText.contentHeight + 40
		border.color: "#888888"
		color: "#EEEEEE"

		ListModel {
			id: buttonsModel
			ListElement {
				text: "OK"
				action: "OK"
			}
			ListElement {
				text: "Cancel"
				action: "CANCEL"
			}
		}

		Component {
			id: buttonComponent
			Rectangle {
				height: buttonRow.height
				width: buttonRow.width / buttonsRepeater.count
				color:  mouseAreaOk.containsMouse ? "#44000000" : "#00000000"
				Text {
					id: myText
					anchors.centerIn: parent
					text: model.text
					color: mouseAreaOk.containsMouse ? "white" : "black"
				}
				MouseArea {
					id: mouseAreaOk
					anchors.fill: parent
					hoverEnabled: true
					cursorShape: "PointingHandCursor"
					onClicked: {
						quit(model.action);
						//myself.destroy();
					}
				}
			}
		}

		Rectangle {
			id: buttonBar
			color: "#DDDDDD"
			anchors.leftMargin: 1
			anchors.rightMargin: 1
			anchors.bottomMargin: 1
			height: 30
			anchors {
				bottom: parent.bottom
				left: parent.left
				right: parent.right
			}

			Column {
				id: invisibleMetrics
				visible: false

				Repeater {
					model: buttonsModel;
					delegate: Text { text: model.text; }
				}
			}

			Row {
				id: buttonRow
				anchors.fill: parent
				Repeater {
					id: buttonsRepeater
					model: buttonsModel
					delegate: buttonComponent
				}
			}
		}

		Item {
			id: messageItem
			clip: true
			anchors {
				top: parent.top
				left: parent.left
				right: parent.right
				bottom: buttonBar.top
			}
			Text {
				id: mainText
				anchors.fill: parent
				anchors.leftMargin: 20
				anchors.rightMargin: 20
				anchors.topMargin: 20
				anchors.bottomMargin: 20
				wrapMode: Text.WordWrap
			}
		}
	}
}
