import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

ToolBar {
	id: toolBar
	signal currentIndexChanged(int current)
	signal closeMe()
	property int min: 0
	property int max: 0
	property int current: 0
	property string currentTemplate: "Foobar %1/%2"

	clip: true

	function __changeCurrent(val) {
		if (current === val)
			return;

		current = val;
		currentIndexChanged(val);
	}

	Component.onCompleted: {
		textBehavior.enabled = true;
	}

	RowLayout {
		anchors.fill: parent

		ToolButton {
			iconSource: "first.png"
			onClicked: {
				if (current === min)
					return;
				__changeCurrent(min);
			}
		}

		ToolButton {
			iconSource: "previous.png"
			onClicked: {
				if (current > min) {
					__changeCurrent(current - 1);
				}
			}
		}

		Item {
			Layout.fillWidth: true
			width: 1
			Layout.fillHeight: true

			Text {
				id: myText
				renderType: Text.NativeRendering
				clip: true
				Behavior on text {
					id: textBehavior
					enabled: toolBar.visible
					ParallelAnimation {
						SequentialAnimation {
							NumberAnimation { target: myText; property: "opacity"; to: 0; duration: 500 }
							PropertyAction {}
							NumberAnimation { target: myText; property: "opacity"; to: 1; duration: 1 }
						}
						SequentialAnimation {
							NumberAnimation { target: myAlternativeText; property: "opacity"; to: 1; duration: 500 }
							NumberAnimation { target: myAlternativeText; property: "opacity"; to: 0; duration: 1 }
						}
					}
				}
				anchors.fill: parent
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				text: currentTemplate.arg(current).arg(max)
				font.pointSize: 13
			}

			Text {
				id: myAlternativeText
				renderType: Text.NativeRendering
				anchors.fill: parent
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				property int current: 0
				clip: true
				text: currentTemplate.arg(current).arg(max)
				font.pointSize: 13
				opacity: 0
			}

			Binding {
				target: myAlternativeText
				property: "current"
				value: current
			}
		}

		ToolButton {
			iconSource: "next.png"
			onClicked: {
				if (current < max) {
					__changeCurrent(current + 1);
				}
			}
		}

		ToolButton {
			iconSource: "last.png"
			onClicked: {
				if (current === max)
					return;
				__changeCurrent(max);
			}
		}

		ToolButton {
			iconSource: "close.png"
			onClicked: {
				closeMe();
			}
		}
	}
}
