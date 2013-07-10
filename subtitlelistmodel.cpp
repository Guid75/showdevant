#include <QSqlQuery>

#include "subtitlelistmodel.h"

SubtitleListModel::SubtitleListModel(QObject *parent) :
	SqlQueryModel(parent)
{
}

void SubtitleListModel::setShow(const QString &show)
{
	if (_show == show)
		return;

	_show = show;
	load();
	emit showChanged();
}

void SubtitleListModel::setSeason(int season)
{
	if (_season == season)
		return;

	_season = season;
	load();
	emit seasonChanged();
}

void SubtitleListModel::setEpisode(int episode)
{
	if (_episode == episode)
		return;

	_episode = episode;
	load();
	emit episodeChanged();
}


void SubtitleListModel::load()
{
	if (_show.isEmpty() || _season < 0 || _episode < 0)
		return;

	cacheWatcher.clearWatching();
	QVariantMap id;
	id.insert("showId", _show);
	id.insert("season", _season);
	id.insert("episode", _episode);
	cacheWatcher.watchFor(Cache::Data_Subtitles, id);

	Cache::instance().synchronizeSubtitles(_show, _season, _episode);
	select();
}


void SubtitleListModel::select()
{
	QSqlQuery query;
	query.prepare("SELECT * FROM subtitle WHERE show_id=:show_id AND season=:season AND episode=:episode");
	query.bindValue(":show_id", _show);
	query.bindValue(":season", _season);
	query.bindValue(":episode", _episode);
	query.exec();
	setQuery(query);
}
