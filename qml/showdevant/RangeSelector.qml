import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

ToolBar {
	id: toolBar
	signal closeMe()
	property int min: 0
	property int max: 0
	property int current: 0
	property string template: "Foobar %1/%2"
	clip: true

	// force the signal of the current changed to be sent again
	function refreshCurrent() {
		currentChanged(current);
	}

	RowLayout {
		anchors {
			left: parent.left
			right: parent.right
		}

		ToolButton {
			iconSource: "first.png"
			onClicked: {
				current = min;
			}
		}

		ToolButton {
			iconSource: "previous.png"
			onClicked: {
				if (current > min) {
					current -= 1;
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
						onStopped: {
							console.log("stop");
						}

						onStarted: {
							console.log("start");
						}

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
				text: template.arg(current).arg(max)
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
				text: template.arg(current).arg(max)
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
					current += 1;
				}
			}
		}

		ToolButton {
			iconSource: "last.png"
			onClicked: {
				current = max;
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
