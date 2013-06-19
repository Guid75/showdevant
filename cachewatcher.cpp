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
