#include <QSqlQuery>

#include "episodemodel.h"

EpisodeModel::EpisodeModel(QObject *parent) :
	SqlQueryModel(parent),
	_season(-1),
	_episode(-1)
{
}

void EpisodeModel::select()
{
	QSqlQuery query;
	query.prepare("SELECT * FROM episode WHERE show_id=:show_id AND season=:season AND episode=:episode");
	query.bindValue(":show_id", _show);
	query.bindValue(":season", _season);
	query.bindValue(":episode", _episode);
	query.exec();
	setQuery(query);
}

void EpisodeModel::load()
{
	if (_show.isEmpty() || _season < 0 || _episode < 0)
		return;

	cacheWatcher.clearWatching();
	QVariantMap id;
	id.insert("showId", _show);
	id.insert("season", _season);
	cacheWatcher.watchFor(Cache::Data_Episodes, id);
	id.insert("episode", _episode);
	cacheWatcher.watchFor(Cache::Data_Episodes, id);

	Cache::instance().synchronizeEpisodes(_show, _season, _episode, true);
	select();
}

void EpisodeModel::setShow(const QString &show)
{
	if (_show == show)
		return;

	_show = show;
	load();
	emit showChanged();
}

void EpisodeModel::setSeason(int season)
{
	if (_season == season)
		return;

	_season = season;
	load();
	emit seasonChanged();
}

void EpisodeModel::setEpisode(int episode)
{
	if (_episode == episode)
		return;

	_episode = episode;
	load();
	emit episodeChanged();
}
