import QtQuick 2.0
import com.guid75 1.0

Item {
	id: root

	// default size, but scalable by user
	width: 800
	height: 600

	Component.onCompleted: {
		if (databaseManager.openDB() === 2) {
			console.log("database opened");
		}
	}

	Rectangle {
		id: leftRectangle
		anchors {
			top: topToolbar.bottom
			bottom: parent.bottom
		}
		width: 200
		color: "#DDDDDD"
		Component.onCompleted: {
			leftAnimation.start();
		}
		ShowList {
			anchors.fill: parent
			anchors.topMargin: 10
			anchors.bottomMargin: 40
		}
	}

	PathAnimation {
		id: leftAnimation
		easing.type: Easing.InQuad
		path: Path {
			startX: -200
			PathCubic {
				x: 0
				y: 0
			}
		}
		target: leftRectangle
		duration: 750
	}

	TopToolbar {
		id: topToolbar
		Component.onCompleted: {
			topToolbarAnimation.start();
		}
	}

	PathAnimation {
		id: topToolbarAnimation
		easing.type: Easing.InQuad
		path: Path {
			startY: -50
			PathCubic {
				x: 0
				y: 0
			}
		}
		target: topToolbar
		duration: 750
	}

//	Rectangle {
//		id: leftPanel

//		anchors {
//			top: parent.top
//			left: parent.left
//			bottom: parent.bottom
//		}
//		width: 180 //root.width * 0.3
//		gradient: Gradient {
//			GradientStop { position: 0.0; color: "lightsteelblue" }
//			GradientStop { position: 1.0; color: "blue" }
//		}

//		ListModel {
//			id: pouetModel
//			ListElement {
//				name: 'House of Cards'
//			}
//			ListElement {
//				name: 'Californication'
//			}
//			ListElement {
//				name: 'Game of Thrones'
//			}
//			ListElement {
//				name: 'Dexter'
//			}
//			ListElement {
//				name: 'Lost'
//			}
//			ListElement {
//				name: "It's Always Sunny in Philadelphia"
//			}
//			ListElement {
//				name: 'The Good Wife'
//			}
//			ListElement {
//				name: 'Nurse Jackie'
//			}
//			ListElement {
//				name: "Grey's Anatomy"
//			}
//			ListElement {
//				name: 'Six Feet Under'
//			}
//			ListElement {
//				name: 'Sons of Anarchy'
//			}
//			ListElement {
//				name: 'American Horror Story'
//			}
//			ListElement {
//				name: 'Dead Like Me'
//			}
//			ListElement {
//				name: 'True Blood'
//			}
//			ListElement {
//				name: 'Supernatural'
//			}
//			ListElement {
//				name: 'How I Met Your Mother'
//			}
//			ListElement {
//				name: 'pouet2'
//			}
//			ListElement {
//				name: 'pouet2'
//			}
//		}

//		Component {
//			id: pouetDelegate
//			Rectangle {
//				id: itemRectangle
//				state: "normal"
//				width: parent.width
//				height: 20
//				MouseArea {
//					id: mouseAreaItem
//					anchors.fill: parent
//					onClicked: {
//						pouetList.currentIndex = index
//					}
//					hoverEnabled: true
//				}
//				Text {
//					id: itemTextShadow
//					color: "white"
//					x: itemText.x + 1
//					y: itemText.y + 1
//					width: pouetList.width
///*					anchors.fill: parent
//					width: pouetList.width*/
//					text: name
//					elide: Text.ElideRight
//				}
//				Text {
//					id: itemText
//					x: 0
//					anchors.verticalCenter: parent.verticalCenter
//					//anchors.fill: parent
//					width: pouetList.width
//					/*anchors.verticalCenter: parent.verticalCenter
//					anchors.horizontalCenter: parent.horizontalCenter*/
//					text: name
//					elide: Text.ElideRight
//				}
//				states: [
//					State {
//						name: "normal"
//						when: (pouetList.currentIndex != index)
//						PropertyChanges {
//							target: itemRectangle
//							color: mouseAreaItem.containsMouse ? "red" : "#00000000"
//						}
//						PropertyChanges {
//							target: itemText
//							color: mouseAreaItem.containsMouse ? "white" : "black"
//						}
//						PropertyChanges {
//							target: itemTextShadow
//							color: mouseAreaItem.containsMouse ? "black" : "white"
//						}
//					},
//					State {
//						name: "selected"
//						when: (pouetList.currentIndex == index)
//						PropertyChanges {
//							target: itemRectangle
//							color: mouseAreaItem.containsMouse ? "red" : "blue"
//						}
//						PropertyChanges {
//							target: itemText
//							color: "white"
//						}
//						PropertyChanges {
//							target: itemTextShadow
//							color: "black"
//						}
//					}
//				]
//			}
//		}

//		ListView {
//			clip: true
//			id: pouetList
//			anchors.fill: parent
//			anchors.bottomMargin: 20
//			model: pouetModel
//			delegate: pouetDelegate
//		}

//		Text {
//			id: bottomText
//			text: pouetList.currentIndex
//			anchors.bottom: parent.bottom
//			anchors.top: pouetList.bottom
//			height: 20
//		}
//	}

//	Rectangle {
//		id: topPanel

//		anchors {
//			top: parent.top
//			left: leftPanel.right
//			right: parent.right
//		}
//		radius: 10
//		border.width: 2
//		antialiasing: true
//		height: 25 //root.height * 0.2
//		color: "#E0E0E0"
//		clip: true
//		TextInput {
//			clip: true
//			focus:true
//			selectByMouse: true
//			anchors.fill: parent
//			anchors.leftMargin: 10
//			anchors.rightMargin: 10
//			anchors.topMargin: 5
//		}
//	}

//	Rectangle {
//		id: contentArea

//		anchors {
//			top: topPanel.bottom
//			left: leftPanel.right
//			right: parent.right
//			bottom: root.bottom
//		}
//		color: "white"

//		clip: true
//		Row {
//			spacing: 2
//			anchors.centerIn: parent
//			Rectangle { color: "red"; width: 50; height: 50 }
//			Rectangle { color: "green"; width: 20; height: 50 }
//			Rectangle { color: "blue"; width: 50; height: 20 }
//		}
//	}
}

