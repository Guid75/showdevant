import QtQuick 2.0
import com.guid75 1.0

Rectangle {
	color: "#00000000"
	property alias model : listView.model

	SqlTableModel {
		id: showModel
		table: "show"
	}

	Component {
		id: showDelegate
		Item {
			width: parent.width
			height: 20
			clip: true
			Text {
				width: parent.width
				//horizontalAlignment: Text.AlignHCenter
				text: model.title
				elide: Text.ElideRight
			}
		}
	}

	ListView {
		id: listView
		anchors {
			fill: parent
			leftMargin: 4
			rightMargin: 4
		}
		model: showModel
		delegate: showDelegate
	}
}
