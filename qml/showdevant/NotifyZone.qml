import QtQuick 2.0

Item {
	id: notifyZone
	ListModel {
		id: listModel
	}

	function notify(message, duration)
	{
		listModel.append({
							 message: message,
							 type: 'notification',
							 duration: duration
						 });
	}

	function warning(message, duration)
	{
		listModel.append({
							 message: message,
							 type: 'warning',
							 duration: duration
						 });
	}

	function error(message, duration)
	{
		listModel.append({
							 message: message,
							 type: 'error',
							 duration: duration
						 });
	}

	Column {
		anchors.fill: parent
		add: Transition {
			NumberAnimation {
				properties: "y"
				duration: 500
			}
		}
		Repeater {
			model: listModel
			delegate: NotifyBox {
				width: notifyZone.width
				message: model.message
				duration: model.duration
			}
		}
	}
}
