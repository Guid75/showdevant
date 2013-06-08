#include <QSqlQuery>

#include "episodelistmodel.h"

EpisodeListModel::EpisodeListModel(QObject *parent) :
	SqlQueryModel(parent),
	_season(-1),
	_episode(-1)
{
}

void EpisodeListModel::setShow(const QString &show)
{
	if (_show == show)
		return;

	_show = show;
	load();
	emit showChanged();
}

void EpisodeListModel::setSeason(int season)
{
	if (_season == season)
		return;

	_season = season;
	load();
	emit seasonChanged();
}

void EpisodeListModel::setEpisode(int episode)
{
	if (_episode == episode)
		return;

	_episode = episode;
	load();
	emit episodeChanged();
}

void EpisodeListModel::select()
{
	QSqlQuery query;
	QString prepareStr = "select * from episode where show_id=:show_id AND season=:season";
	if (_episode >= 0)
		prepareStr.append(" AND episode=:episode");
	query.prepare(prepareStr);
	query.bindValue(":show_id", _show);
	query.bindValue(":season", _season);
	if (_episode >= 0)
		query.bindValue(":episode", _episode);
	query.exec();
	setQuery(query);
}

void EpisodeListModel::load()
{
	if (_season < 0)
		return;

	cacheWatcher.clearWatching();
	QVariantMap id;
	id.insert("showId", _show);
	id.insert("season", _season);
	cacheWatcher.watchFor(Cache::Data_Episodes, id);
	if (_episode >= 0) {
		id.insert("episode", _episode);
		cacheWatcher.watchFor(Cache::Data_Episodes, id);
	}

	Cache::instance().synchronizeEpisodes(_show, _season, _episode, _episode >= 0);
	select();
}
