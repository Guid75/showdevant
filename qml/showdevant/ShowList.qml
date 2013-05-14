import QtQuick 2.0

Rectangle {
    color: "#00000000"
	property alias model : listView.model

	ListView {
		id: listView
		anchors.fill: parent
	}
}
