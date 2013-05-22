#include <QSqlQuery>

#include "episodemodel.h"

EpisodeModel::EpisodeModel(QObject *parent) :
	SqlQueryModel(parent),
	_season(1)
{
	setShow("a");
}

void EpisodeModel::setShow(const QString &show)
{
	if (_show == show)
		return;

	_show = show;
	refreshQuery();
}

void EpisodeModel::setSeason(int season)
{
	if (_season == season)
		return;

	_season = season;
	refreshQuery();
}

void EpisodeModel::refreshQuery()
{
	if (_show.isEmpty() || _season < 0)
		return;

	QSqlQuery query;
	query.prepare("select * from episode where show_id=:show_id AND season=:season");
	query.bindValue(":show_id", _show);
	query.bindValue(":season", _season);
	query.exec();
	setQuery(query);
}
