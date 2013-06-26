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

#include "myshowsmodel.h"

MyShowsModel::MyShowsModel(QObject *parent) :
	SqlQueryModel(parent)
{
	cacheWatcher.watchFor(Cache::Data_MemberInfos, QVariantMap());
	cacheWatcher.watchFor(Cache::Data_AddShow, QVariantMap());
	cacheWatcher.watchFor(Cache::Data_RemoveShow, QVariantMap());
	select();
}

void MyShowsModel::select()
{
	setQuery(QSqlQuery("SELECT * FROM myshows"));
}
