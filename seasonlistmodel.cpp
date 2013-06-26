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

#include "cache.h"

#include "seasonlistmodel.h"

SeasonListModel::SeasonListModel(QObject *parent) :
	SqlQueryModel(parent)
{
}

void SeasonListModel::setShow(const QString &show)
{
	if (show == _show)
		return;

	_show = show;

	cacheWatcher.clearWatching();
	QVariantMap id;
	id.insert("showId", _show);
	cacheWatcher.watchFor(Cache::Data_ShowInfos, id);

	Cache::instance().synchronizeShowInfos(_show);
	select();

	emit showChanged();
}

void SeasonListModel::select()
{
	QSqlQuery query;
	query.prepare("SELECT * FROM season WHERE show_id=:show_id");
	query.bindValue(":show_id", _show);
	query.exec();
	setQuery(query);
}
