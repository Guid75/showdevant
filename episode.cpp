#include "episode.h"

Episode::Episode(QObject *parent) :
	CacheObject(parent),
	_showId(""),
	_season(-1),
	_episode(-1)
{
}

void Episode::setShowId(const QString &showId)
{
	if (showId == _showId)
		return;

	qDebug("setShowId()");

	_showId = showId;

	synchronizeData();
}

void Episode::setSeason(int season)
{
	if (_season == season)
		return;

	qDebug("setSeason()");
	_season = season;

	synchronizeData();
}

void Episode::setEpisode(int episode)
{
	if (_episode == episode)
		return;

	qDebug("setEpisode()");
	_episode = episode;

	synchronizeData();
}

bool Episode::handleSynchronizing(Cache::CacheDataType dataType, const QMap<QString, QVariant> &id)
{
	return checkSynchronizeEvent(dataType, id);
}

bool Episode::handleSynchronized(Cache::CacheDataType dataType, const QMap<QString, QVariant> &id)
{
	if (!checkSynchronizeEvent(dataType, id))
		return false;

	// refresh episode data

	return true;
}

void Episode::synchronizeData()
{
	if (_showId.isEmpty() || _season == -1 || _episode == -1)
		return;


}

bool Episode::checkSynchronizeEvent(Cache::CacheDataType dataType, const QMap<QString, QVariant> &id) const
{
	if (dataType != Cache::Data_Episodes)
		return false;

	if (id["showId"].toString() != _showId ||
		id["season"].toInt() != _season)
		return false;

	// if episode is undefined in id, it means ALL episodes are being synchronized so we take that in account
	return id["episode"].isNull() || id["episode"].toInt() == _episode;
}
