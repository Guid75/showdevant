import QtQuick 2.0
import com.guid75 1.0

ShadowRectangle {
	id: topToolbar
	Timer {
		id: searchTimer
		interval: 500;
		running: false;
		repeat: false
		onTriggered: topToolbar.searchTicket = commandManager.showsSearch(searchInput.text)
	}
	ShowSearchModel {
		id: showSearchModel
	}

	property int searchTicket: -1
	property Item searchList: null

	Connections {
		target: commandManager
		onCommandFinished: {
			if (ticketId !== searchTicket) {
				return;
			}

			var compo = Qt.createComponent("SearchResultList.qml");
			// TODO error management
			if (searchList)
				searchList.destroy();
			searchList = compo.createObject(root, {
											  x: searchRectangle.x +1,
											  y: searchRectangle.y + searchRectangle.height,
											  width: searchRectangle.width - 2,
												model: showSearchModel
										  });
		}
	}

	anchors {
		left: parent.left
		right: parent.right
		top: parent.top
	}
	height: 50
	gradient: Gradient {
		GradientStop { position: 0.0; color: "lightsteelblue" }
		GradientStop { position: 1.0; color: "#7777FF" }
	}

	Image {
		id: betaSeriesImage
		source: "betaseries.png"
		anchors {
			leftMargin: 10
			left: parent.left
			verticalCenter: parent.verticalCenter
		}
		fillMode: Image.PreserveAspectFit
		MouseArea {
			anchors.fill: betaSeriesImage
			cursorShape: "PointingHandCursor"
		}
	}

	MouseArea {
		anchors.fill: betaSeriesImage
	}

	Rectangle {
		id: searchRectangle
		color: "white"
		border.color: "#888888"
		border.width: 1
		antialiasing: true
		width: 300
		height: 30
		clip: true
		anchors {
			leftMargin: 10
			left: betaSeriesImage.right
			verticalCenter: parent.verticalCenter
		}
		TextInput {
			id: searchInput
			focus: true
			clip: true
			activeFocusOnPress: true
			anchors.fill: parent
			anchors.leftMargin: 8
			anchors.rightMargin: 8
			anchors.topMargin: 8
			selectByMouse: true
			onTextChanged: {
				// TODO relaunch the search timer
				if (searchTimer.running)
					searchTimer.stop();

				if (text != "")
					searchTimer.start();
				else {
					if (searchList)
						searchList.destroy();
					searchList = null;
				}
			}
			onFocusChanged:
			{
				searchMouseArea.enabled = !searchInput.focus
			}
			Text {
				text: "Enter a TV show name of a part of it"
				visible: searchInput.text == ""  && !searchInput.focus
				color: "#AAAAAA"
				anchors.fill: parent
			}
		}
		MouseArea {
			id: searchMouseArea
			anchors.fill: parent
			cursorShape: "IBeamCursor"
			onPressed: {
				searchInput.focus = true
			}
			hoverEnabled: true
		}
	}

	Rectangle {
		id: searchButton
		color: "#FFFF88"
		border.color: "#888888"
		border.width: 1
		antialiasing: true
		smooth: true
		radius: 4
		width: 70
		height: 30
		clip: true
		anchors {
			leftMargin: 5
			left: searchRectangle.right
			verticalCenter: parent.verticalCenter
		}
		Image {
			source: "search.png"
			smooth: true
			anchors {
				horizontalCenter: parent.horizontalCenter
				verticalCenter: parent.verticalCenter
			}
			width: 16
			height: 16
		}

/*		Text {
			id: searchButtonText
			color: "white"
			horizontalAlignment: Text.AlignHCenter
			text: "<b>search</b>"
			clip: true
			anchors.fill: parent
			anchors.leftMargin: 8
			anchors.rightMargin: 8
			anchors.topMargin: 8
		}*/
		MouseArea {
			anchors.fill: parent
			onClicked: searchButton.focus = true
		}
	}
}
