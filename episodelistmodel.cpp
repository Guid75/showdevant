//  Copyright 2013 Guillaume Denry (guillaume.denry@gmail.com)
//  This file is part of ShowDevant.
//
//  ShowDevant is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ShowDevant is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with ShowDevant.  If not, see <http://www.gnu.org/licenses/>.

#include <QSqlQuery>

#include "episodelistmodel.h"

EpisodeListModel::EpisodeListModel(QObject *parent) :
	SqlQueryModel(parent),
	_season(-1)
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

void EpisodeListModel::select()
{
	QSqlQuery query;
	query.prepare("SELECT * FROM episode WHERE show_id=:show_id AND season=:season");
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
	QVariantMap id;
	id.insert("showId", _show);
	id.insert("season", _season);
	cacheWatcher.watchFor(Cache::Data_Episodes, id);

	Cache::instance().synchronizeEpisodes(_show, _season);
	select();
}
