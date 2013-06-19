import QtQuick 2.0
import QtQuick.Controls 1.0

StackViewDelegate {
	function getTransition(properties)
	{
		if (properties.exitItem.widgetType === 'seasons' && properties.enterItem.widgetType === 'episodes')
			return seasonsToEpisodesTransition;
		if (properties.exitItem.widgetType === 'episodes' && properties.enterItem.widgetType === 'seasons')
			return seasonsToEpisodesTransition;
		if (properties.exitItem.widgetType === 'episodes' && properties.enterItem.widgetType === 'episodes') {
			if (properties.enterItem.model.season < properties.exitItem.model.season)
				return toRightTransition;
			return toLeftTransition;
		}

		return showToShowTransition;
	}

	function transitionFinished(properties)
	{
		properties.exitItem.x = 0
		properties.exitItem.y = 0

		properties.exitItem.opacity = 1
	}

	property Component showToSeasonTransition: StackViewTransition {
		PropertyAnimation {
			target: enterItem
			property: "y"
			from: target.height
			to: 0
			duration: 300
		}
		PropertyAnimation {
			target: exitItem
			property: "y"
			from: 0
			to: -target.height
			duration: 300
		}
	}
	property Component showToShowTransition: StackViewTransition {
		PropertyAnimation {
			target: enterItem
			property: "opacity"
			from: 0
			to: 1
		}
		PropertyAnimation {
			target: exitItem
			property: "opacity"
			from: 1
			to: 0
		}
	}
	property Component seasonsToEpisodesTransition: StackViewTransition {
		PropertyAnimation {
			target: enterItem
			property: "y"
			from: target.height
			to: 0
			duration: 300
		}
		PropertyAnimation {
			target: exitItem
			property: "y"
			from: 0
			to: target.height
			duration: 300
		}
	}
	property Component toLeftTransition: StackViewTransition {
		PropertyAnimation {
			target: enterItem
			property: "x"
			from: target.width
			to: 0
			duration: 300
		}
		PropertyAnimation {
			target: exitItem
			property: "x"
			from: 0
			to: -target.width
			duration: 300
		}
	}

	property Component toRightTransition: StackViewTransition {
		PropertyAnimation {
			target: enterItem
			property: "x"
			from: -target.width
			to: 0
			duration: 300
		}
		PropertyAnimation {
			target: exitItem
			property: "x"
			from: 0
			to: target.width
			duration: 300
		}
	}
}

