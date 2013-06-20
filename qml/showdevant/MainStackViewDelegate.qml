import QtQuick 2.0
import QtQuick.Controls 1.0

StackViewDelegate {
	function __getTransitionName(exitType, enterType) {
		return exitType + '2' + enterType;
	}

	function getTransition(properties)
	{
		switch (__getTransitionName(properties.exitItem.widgetType, properties.enterItem.widgetType)) {
		case 'seasons2episodes':
		case 'episodes2seasons':
		case 'episodes2episodedetail':
		case 'episodedetail2episodes':
		case 'episodedetail2seasons':
			return seasonsToEpisodesTransition;
		case 'episodes2episodes':
			if (properties.enterItem.model.season < properties.exitItem.model.season)
				return toRightTransition;
			return toLeftTransition;
		case 'episodedetail2episodedetail':
			if (properties.enterItem.episode < properties.exitItem.episode)
				return toRightTransition;
			return toLeftTransition;
		default: break;
		}
		return showToShowTransition;
	}

	function transitionFinished(properties)
	{
		properties.exitItem.x = 0
		properties.exitItem.y = 0

		properties.exitItem.opacity = 1

		switch (__getTransitionName(properties.exitItem.widgetType, properties.enterItem.widgetType)) {
		case 'seasons2episodes':
			seasonSelector.opacity = 1;
			break;
		case 'episodes2seasons':
			seasonSelector.visible = false;
			break;
		case 'episodes2episodedetail':
			episodeSelector.opacity = 1;
			break;
		case 'episodedetail2episodes':
			episodeSelector.visible = false;
			break;
		case 'episodedetail2seasons':
			seasonSelector.visible = false;
			episodeSelector.visible = false;
			break;
		default: break;
		}
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

