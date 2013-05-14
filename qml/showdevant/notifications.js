var boxes = [];
var boxCompo;

function __createMessageBox(message, criticality) {
    var lowerBox;
    var y = 0;
    var box;
    var colors = [ "#fbfaae", "#aefafb", "#FFE0E0" ];

    criticality = criticality === undefined ? 0 : criticality;

    if (!boxCompo)
        boxCompo = Qt.createComponent("NotificationBox.qml");

    console.log(boxes.length);
    if (boxes.length > 0) {
        lowerBox = boxes[boxes.length - 1];
        y = lowerBox.y + lowerBox.height - 1;
        console.log(y);
    }

    box = boxCompo.createObject(root, {x: 200, y: y, message: message, color: colors[criticality]});

    if (!box) {
        // Error Handling
        console.log("Error creating object");
        return;
    }

    boxes.push(box);
}

function notify(message)
{
    __createMessageBox(message);
}

function warning(message)
{
    __createMessageBox(message, 1);
}

function error(message)
{
    __createMessageBox(message, 2);
}

