import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.0
import com.guid75 1.0

Rectangle {
	ColumnLayout {
		anchors.fill: parent
		anchors.topMargin: 4
		spacing: 2
		RowLayout {
			Layout.fillWidth: true
			spacing: 2
			TextField {
				Layout.minimumWidth: 200
				placeholderText: "Enter a search term"
			}
			ComboBox {
				Layout.fillWidth: true
				model: 10
			}
			ComboBox {
				textRole: 'caption'
				model: ListModel {
					ListElement {
						caption: "EN/FR"
					}
					ListElement {
						caption: "FR"
					}
					ListElement {
						caption: "EN"
					}
				}
			}
		}
		Item {
			Layout.fillHeight: true
			Layout.fillWidth: true
			TableView {
				id: subtitlesTableView
				anchors.fill: parent
				model: subtitleListModel
				headerVisible: false
				TableViewColumn {
					role: "file"
					title: "File"
					elideMode: Qt.ElideRight
				}
//				itemDelegate: Item {
//					Text {
//						renderType: Text.NativeRendering
//						anchors.verticalCenter: parent.verticalCenter
//						color: subtitleItemMouseArea.containsMouse && !styleData.selected ? "blue" : styleData.textColor
//						font.underline: subtitleItemMouseArea.containsMouse
//						elide: styleData.elideMode
//						text: styleData.value
//						MouseArea {
//							cursorShape: Qt.PointingHandCursor
//							id: subtitleItemMouseArea
//							hoverEnabled: true
//							anchors.fill: parent
//						}
//					}
//				}
				itemDelegate: RowLayout {
					Text {
						renderType: Text.NativeRendering
						//anchors.verticalCenter: parent.verticalCenter
						color: subtitleItemMouseArea.containsMouse && !styleData.selected ? "blue" : styleData.textColor
						font.underline: subtitleItemMouseArea.containsMouse
						elide: styleData.elideMode
						text: styleData.value
						MouseArea {
							cursorShape: Qt.PointingHandCursor
							id: subtitleItemMouseArea
							hoverEnabled: true
							anchors.fill: parent
						}
					}
					Item {
						Layout.fillWidth: true
					}
				}
			}
			Loader {
				id: loadingWidget
				anchors.centerIn: parent
				sourceComponent: LoadingWidget {
					anchors.centerIn: parent
					color: "#888888"
					count: 16
					size: 8
					width: 100
					height: 100
				}
				active: subtitleListModel.synchronizeState === CacheWatcher.Synchronizing
				asynchronous: true
			}
		}
	}
}
