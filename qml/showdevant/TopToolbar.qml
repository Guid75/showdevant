import QtQuick 2.0
import com.guid75 1.0
import "commands.js" as Commands

ShadowRectangle {
	signal askForLogin()
	signal askForLogout()
	id: topToolbar
	ListModel {
		id: searchModel
	}

	Timer {
		id: searchTimer
		interval: 500;
		running: false;
		repeat: false
		onTriggered: {
			Commands.showsSearch({ expression: searchInput.text },
								 function(error, json) {
									 if (error)
										 return;
									 var compo = Qt.createComponent("SearchResultList.qml");

									 // fill the model
									 searchModel.clear();
									 for (var showName in json.shows) {
										 if (!json.shows.hasOwnProperty(showName)) {
											 continue;
										 }
										 var show = json.shows[showName];
										 searchModel.append({
																showId: show["url"],
																title: show["title"]
															})
									 }

									 if (!searchList) {
										 var obj = searchRectangle.mapToItem(null, 0, 0);
										 searchList = compo.createObject(root, {
																			 searchText: searchInput.text,
																			 listX: obj.x + 1,
																			 listY: obj.y + searchRectangle.height,
																			 listWidth: searchRectangle.width - 2,
																			 //model: showSearchModel
																			 model: searchModel
																		 });
									 }
								 });
		}
	}

	function hideSearchResults() {
		if (searchList)
			searchList.destroy();
		searchList = null;
	}

	Shortcut {
		key: "Escape"
		onActivated: {
			hideSearchResults();
		}
	}

	property Item searchList: null

	anchors {
		left: parent.left
		right: parent.right
	}
	height: 50
	gradient: Gradient {
		GradientStop { position: 0.0; color: "white" }
		GradientStop { position: 1.0; color: "#AAAAAA" }
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
			hoverEnabled: true
			anchors.fill: betaSeriesImage
			cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
			onClicked: {
			}
		}
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
			renderType: Text.NativeRendering
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

				if (text != "") {
					if (searchList)
						searchList.searchText = text;
					searchTimer.start();
				} else {
					hideSearchResults();
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
				renderType: Text.NativeRendering
			}
		}
		MouseArea {
			id: searchMouseArea
			anchors.fill: parent
			cursorShape: containsMouse ? Qt.IBeamCursor : Qt.ArrowCursor
			onPressed: {
				searchInput.focus = true;
				mouse.accepted = false;
			}
			hoverEnabled: true
		}
	}

	//	Rectangle {
	//		id: searchButton
	//		color: "#FFFF88"
	//		border.color: "#888888"
	//		border.width: 1
	//		antialiasing: true
	//		smooth: true
	//		radius: 4
	//		width: 70
	//		height: 30
	//		clip: true
	//		anchors {
	//			leftMargin: 5
	//			left: searchRectangle.right
	//			verticalCenter: parent.verticalCenter
	//		}
	//		Image {
	//			source: "search.png"
	//			smooth: true
	//			anchors {
	//				horizontalCenter: parent.horizontalCenter
	//				verticalCenter: parent.verticalCenter
	//			}
	//			width: 16
	//			height: 16
	//		}

	//		/*		Text {
	//			id: searchButtonText
	//			color: "white"
	//			horizontalAlignment: Text.AlignHCenter
	//			text: "<b>search</b>"
	//			clip: true
	//			anchors.fill: parent
	//			anchors.leftMargin: 8
	//			anchors.rightMargin: 8
	//			anchors.topMargin: 8
	//		}*/
	//		MouseArea {
	//			anchors.fill: parent
	//			onClicked: searchButton.focus = true
	//		}
	//	}

	Component {
		id: loggedComponent
		Column {
			anchors {
				right: parent.right
				verticalCenter: parent.verticalCenter
			}
			Text {
				renderType: Text.NativeRendering
				text: 'Welcome, %1!'.arg(authenticator.user)
			}
			Text {
				width: parent.width
				renderType: Text.NativeRendering
				text: 'Log out'
				font.underline: true
				color: "blue"
				horizontalAlignment: Text.AlignRight
				MouseArea {
					anchors.fill: parent
					hoverEnabled: true
					cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
					onClicked: {
						askForLogout();
					}
				}
			}
		}
	}

	Component {
		id: notLoggedComponent
		Text {
			anchors {
				right: parent.right
				verticalCenter: parent.verticalCenter
			}
			renderType: Text.NativeRendering
			text: 'not logged'
			font.underline: true
			color: "blue"
			MouseArea {
				anchors.fill: parent
				hoverEnabled: true
				cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
				onClicked: {
					askForLogin();
				}
			}
		}
	}

	Component {
		id: loadingComponent
		LoadingWidget {
			anchors.centerIn: parent
			color: "#888888"
			count: 12
			width: topToolbar.height - 14
		}
	}

	Loader {
		id: rightLoader
		active: true
		asynchronous: true
		anchors {
			right: parent.right
			verticalCenter: parent.verticalCenter
			rightMargin: 6
		}

		sourceComponent: authenticator.logState === Authenticator.Logging ? loadingComponent : notLoggedComponent
	}

	Connections {
		target: authenticator
		onLogStateChanged: {
			switch (authenticator.logState) {
			case Authenticator.NotLogged:
				rightLoader.sourceComponent = notLoggedComponent;
				break;
			case Authenticator.Logging:
				rightLoader.sourceComponent = loadingComponent;
				break;
			case Authenticator.Logged:
				rightLoader.sourceComponent = loggedComponent;
				break;
			default:
				break;
			}
		}
	}
}
