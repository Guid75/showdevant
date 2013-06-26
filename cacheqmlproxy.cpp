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

#include "cache.h"

#include "cacheqmlproxy.h"

CacheQmlProxy::CacheQmlProxy(QObject *parent) :
    QObject(parent)
{
	connect(&Cache::instance(), &Cache::showAdded,
			this, &CacheQmlProxy::showAdded);
	connect(&Cache::instance(), &Cache::showRemoved,
			this, &CacheQmlProxy::showRemoved);
}
