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

#include "cachewatcher.h"

CacheWatcher::CacheWatcher(QObject *parent) :
	QObject(parent),
	_synchronizeState(Unsynchronized)
{

	connect(&Cache::instance(), &Cache::synchronizing,
			this, &CacheWatcher::synchronizing);

	connect(&Cache::instance(), &Cache::synchronized,
			this, &CacheWatcher::synchronized);

	connect(&Cache::instance(), &Cache::synchronizeFailed,
			this, &CacheWatcher::synchronizeFailed);
}

void CacheWatcher::setSynchronizeState(CacheWatcher::SynchronizeState state)
{
	if (_synchronizeState == state)
		return;

	_synchronizeState = state;
	emit synchronizeStateChanged();
}

bool CacheWatcher::watchable(Cache::CacheDataType dataType, const QVariantMap &id)
{
	foreach (const QVariantMap &watchingId, watching.values(dataType)) {
		bool ok = true;
		// all watching keys values must be included into the challenged id
		foreach (const QString &key, watchingId.keys()) {
			if (watchingId[key] != id[key]) {
				ok = false;
				break;
			}
		}
		if (ok)
			return true;
	}

	return false;
}

void CacheWatcher::synchronizing(Cache::CacheDataType dataType, const QVariantMap &id)
{
	if (watchable(dataType, id))
		setSynchronizeState(Synchronizing);
}

void CacheWatcher::synchronized(Cache::CacheDataType dataType, const QVariantMap &id)
{
	if (watchable(dataType, id))
		setSynchronizeState(Synchronized);
}

void CacheWatcher::synchronizeFailed(Cache::CacheDataType dataType, const QVariantMap &id)
{
	if (watchable(dataType, id))
		setSynchronizeState(SynchronizeError);
}

void CacheWatcher::clearWatching()
{
	watching.clear();
}

void CacheWatcher::watchFor(Cache::CacheDataType dataType, const QVariantMap &id)
{
	watching.insert(dataType, id);
}
