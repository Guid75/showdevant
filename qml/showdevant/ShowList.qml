import QtQuick 2.0

Rectangle {
	color: "#EEEEEE"
	property alias model : listView.model

	ListView {
		id: listView
		anchors.fill: parent
	}
}
