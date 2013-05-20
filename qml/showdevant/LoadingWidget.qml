import QtQuick 2.0

Item {
	height: 10
	Column {
	Text {
		text: "Loading, please wait..."
	}

	Rectangle {
		id: myself
		radius: 4
		height: 10
		anchors.left: parent.left
		anchors.right: parent.right
		gradient: Gradient {
			GradientStop { position: 0.0; color: "black"}
			GradientStop { position: 1.0; color: "#888888" }
		}
		border.color: "#AA9999"

		Component.onCompleted: {
			cursorAnimation.start()
		}

		Rectangle {
			id: cursor
			anchors {
				topMargin: 1
				bottomMargin: 1
				top: parent.top
				bottom: parent.bottom
			}
			width: 10
			x: 1
			color: "red"
			BorderImage {
				anchors.fill: cursor
				anchors { leftMargin: -10; topMargin: -10; rightMargin: -10; bottomMargin: -10 }
				border { left: 10; top: 10; right: 10; bottom: 10 }
				source: "loading-glow.png"; smooth: true
			}
		}

		PathAnimation {
			id: cursorAnimation
			loops: Animation.Infinite
			target: cursor
			path: Path {
				startX: 1
				startY: 0
				PathQuad {
					x: myself.width - 11
					y: 0
				}
				PathQuad {
					x: 1
					y: 0
				}
			}
			duration: 2000
		}

	}
	}
}
