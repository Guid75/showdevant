var boxes = [];
var boxCompo;

function __notifyBoxTimerTriggered() {
	console.log("notify triggered");
}

function __createMessageBox(message, criticality, duration) {
	var lowerBox;
	var y = 0;
	var box;
	var colors = [ "#fbfaae", "#ffe0b8", "#FFE0E0" ];

	criticality = criticality === undefined ? 0 : criticality;
	duration = duration === undefined ? 2000 : duration;

	if (!boxCompo)
		boxCompo = Qt.createComponent("NotificationBox.qml");

	if (boxes.length > 0) {
		lowerBox = boxes[boxes.length - 1];
		y = lowerBox.y + lowerBox.height - 1;
	}

	box = boxCompo.createObject(root, {y: y, message: message, duration: duration, color: colors[criticality]});

	if (!box) {
		// Error Handling
		console.log("Error creating object");
		return;
	}

	boxes.push(box);
}

function notify(message, duration)
{
	__createMessageBox(message, 0, duration);
}

function warning(message, duration)
{
	__createMessageBox(message, 1, duration);
}

function error(message, duration)
{
	__createMessageBox(message, 2, duration);
}

