import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

ToolBar {
	signal seasonChanged(int season)
	signal closeMe()
	clip: true
	property int minSeason: 0
	property int maxSeason: 0
	property int currentSeason: 0

	function __changeSeason(season) {
		if (currentSeason === season)
			return;

		myAlternativeText.nextSeason = season;
		currentSeason = season;
		seasonChanged(currentSeason);
	}

	RowLayout {
		anchors.fill: parent

		ToolButton {
			iconSource: "first.png"
			onClicked: {
				if (currentSeason === minSeason)
					return;
				__changeSeason(minSeason);
			}
		}

		ToolButton {
			iconSource: "previous.png"
			onClicked: {
				if (currentSeason > minSeason) {
					__changeSeason(currentSeason - 1);
				}
			}
		}

		Item {
			Layout.fillWidth: true
			width: 1
			Layout.fillHeight: true

			Text {
				id: myText
				clip: true
				Behavior on text {
					ParallelAnimation {
						SequentialAnimation {
							NumberAnimation { target: myText; property: "opacity"; to: 0; duration: 500 }
							PropertyAction {}
							NumberAnimation { target: myText; property: "opacity"; to: 1; duration: 1 }
						}
						SequentialAnimation {
							NumberAnimation { target: myAlternativeText; property: "opacity"; to: 1; duration: 500 }
							NumberAnimation { target: myAlternativeText; property: "opacity"; to: 0; duration: 1 }
						}
					}
				}
				anchors.fill: parent
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				text: "Season %1/%2".arg(currentSeason).arg(maxSeason)
				font.pointSize: 14
			}

			Text {
				id: myAlternativeText
				anchors.fill: parent
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				property int nextSeason: 0
				clip: true
				text: "Season %1/%2".arg(nextSeason).arg(maxSeason)
				font.pointSize: 14
				opacity: 0
			}
		}

		ToolButton {
			iconSource: "next.png"
			onClicked: {
				if (currentSeason < maxSeason) {
					__changeSeason(currentSeason + 1);
				}
			}
		}

		ToolButton {
			iconSource: "last.png"
			onClicked: {
				if (currentSeason === maxSeason)
					return;
				__changeSeason(maxSeason);
			}
		}

		ToolButton {
			iconSource: "close.png"
			onClicked: {
				closeMe();
			}
		}
	}
}
