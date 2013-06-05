#include <QSqlQuery>

#include "episodelistmodel.h"

EpisodeListModel::EpisodeListModel(QObject *parent) :
	SqlQueryModel(parent),
	_season(-1),
	_episode(-1)
{
	_show = "a";
	select();

/*	connect(&Cache::instance(), &Cache::synchronizing,
			this, &EpisodeListModel::synchronizing);

	connect(&Cache::instance(), &Cache::synchronized,
			this, &EpisodeListModel::synchronized);*/
}

void EpisodeListModel::setShow(const QString &show)
{
	if (_show == show)
		return;

	_show = show;
	load();
}

void EpisodeListModel::setSeason(int season)
{
	if (_season == season)
		return;

	_season = season;
	load();
}

void EpisodeListModel::setEpisode(int episode)
{
	if (_episode == episode)
		return;

	_episode = episode;
	load();
}

void EpisodeListModel::select()
{
	QSqlQuery query;
	query.prepare("select * from episode where show_id=:show_id AND season=:season");
	query.bindValue(":show_id", _show);
	query.bindValue(":season", _season);
	query.exec();
	setQuery(query);
}

void EpisodeListModel::load()
{
	if (_season < 0)
		return;

	cacheWatcher.clearWatching();
	QMap<QString,QVariant> id;
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
