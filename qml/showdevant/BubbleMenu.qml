import QtQuick 2.0

Item {
	id: bubbleMenu
	signal itemClicked(string name);
	property alias model : listView.model

	property alias bubbleX : bubbleItem.x
	property alias bubbleY : bubbleItem.y
	property alias bubbleHeight : bubbleItem.height

	anchors.fill: parent

	MouseArea {
		anchors.fill: parent
		propagateComposedEvents: true
		onPressed: {
			parent.visible = false;
			mouse.accepted = false;
		}
	}

	Item {
		id: bubbleItem
		width: 70
		height: 48
		Behavior on y {
			NumberAnimation { duration: 100 }
		}

		Canvas {
			id: canvas
			property string strokeStyle: "#888888"
			property string fillStyle: "#EEEEEE"
			property double lineWidth: 1
			anchors.fill: parent
			anchors {
				top: parent.top
				bottom: parent.bottom
			}
			onPaint: {
				var ctx = canvas.getContext('2d');
				ctx.translate(0.5, 0.5); // prevent bluring effect
				ctx.save();
				ctx.clearRect(0, 0, canvas.width, canvas.height);
				ctx.strokeStyle = canvas.strokeStyle;
				ctx.fillStyle = canvas.fillStyle;
				ctx.lineWidth = canvas.lineWidth;
				ctx.globalAlpha = canvas.alpha;

				var middleV = canvas.height / 2;
				ctx.beginPath();
				ctx.moveTo(0, middleV);
				ctx.lineTo(10, middleV - 5);
				ctx.lineTo(10, 0);
				ctx.lineTo(canvas.width - 1, 0);
				ctx.lineTo(canvas.width - 1, canvas.height - 1);
				ctx.lineTo(10, canvas.height - 1);
				ctx.lineTo(10, middleV + 5);
				ctx.lineTo(0, middleV);
				ctx.closePath();
				ctx.fill();
				ctx.stroke();

				//			ctx.globalAlpha = canvas.alpha;
				//			ctx.strokeStyle = canvas.strokeStyle;
				//			ctx.fillStyle = canvas.fillStyle;
				//			ctx.lineWidth = canvas.lineWidth;
				//			ctx.scale(canvas.scaleX, canvas.scaleY);
				//			ctx.rotate(canvas.rotate);
				//			// ![0]
				//			ctx.beginPath();
				//			ctx.moveTo(75,25);
				//			ctx.quadraticCurveTo(25,25,25,62.5);
				//			ctx.quadraticCurveTo(25,100,50,100);
				//			ctx.quadraticCurveTo(50,120,30,125);
				//			ctx.quadraticCurveTo(60,120,65,100);
				//			ctx.quadraticCurveTo(125,100,125,62.5);
				//			ctx.quadraticCurveTo(125,25,75,25);
				//			ctx.closePath();
				//			// ![0]
				//			if (canvas.fill)
				//				ctx.fill();
				//			if (canvas.stroke)
				//				ctx.stroke();

				//			// ![1]
				//			ctx.fillStyle="green";
				//			ctx.font = "Bold 15px";

				//			ctx.fillText("QML酷毙了", 30, 60);
				// ![1]
				ctx.restore();
			}
		}

		ListView {
			id: listView
			clip: true
			anchors {
				fill: parent
				leftMargin: 16
				topMargin: 4
				bottomMargin: 4
				rightMargin: 4
			}
			delegate: Rectangle {
				color: itemMouseArea.containsMouse ? "white" : "#00000000"
				clip: true
				width: parent.width
				height: itemText.contentHeight
				Text {
					id: itemText
					renderType: Text.NativeRendering
					text: model.text
				}
				MouseArea {
					id: itemMouseArea
					anchors.fill: parent
					hoverEnabled: true
					cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
					onClicked: {
						itemClicked(model.name);
						bubbleMenu.visible = false;
					}
				}
			}
		}
	}
}
