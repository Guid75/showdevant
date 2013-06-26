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

#ifndef CACHEWATCHER_H
#define CACHEWATCHER_H

#include <QObject>
#include <QMultiMap>

#include "cache.h"

class CacheWatcher : public QObject
{
	Q_OBJECT

	Q_ENUMS(SynchronizeState)
	Q_PROPERTY(SynchronizeState synchronizeState READ synchronizeState() NOTIFY synchronizeStateChanged())

public:
	enum SynchronizeState {
		Unsynchronized,
		Synchronizing,
		Synchronized,
		SynchronizeError
	};

	explicit CacheWatcher(QObject *parent = 0);

	SynchronizeState synchronizeState() const { return _synchronizeState; }
signals:
	void synchronizeStateChanged();

public slots:
	void watchFor(Cache::CacheDataType dataType, const QVariantMap &id);
	void clearWatching();

protected:
	void setSynchronizeState(SynchronizeState state);

	// returns true if the synchronize event matching the watching
	bool watchable(Cache::CacheDataType dataType, const QVariantMap &id);

private:
	SynchronizeState _synchronizeState;
	QMultiMap<Cache::CacheDataType,QVariantMap> watching;

private slots:
	void synchronizing(Cache::CacheDataType dataType, const QVariantMap &id);
	void synchronized(Cache::CacheDataType dataType, const QVariantMap &id);
	void synchronizeFailed(Cache::CacheDataType dataType, const QVariantMap &id);
};

#endif // CACHEWATCHER_H
