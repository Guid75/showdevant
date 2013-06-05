#include "cachewatcher.h"

CacheWatcher::CacheWatcher(QObject *parent) :
    QObject(parent)
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

bool CacheWatcher::watchable(Cache::CacheDataType dataType, const QMap<QString, QVariant> &id)
{
	return watching.values(dataType).indexOf(id) >= 0;
}

void CacheWatcher::synchronizing(Cache::CacheDataType dataType, const QMap<QString, QVariant> &id)
{
	if (watchable(dataType, id))
		setSynchronizeState(Synchronizing);
}

void CacheWatcher::synchronized(Cache::CacheDataType dataType, const QMap<QString, QVariant> &id)
{
	if (watchable(dataType, id))
		setSynchronizeState(Synchronized);
}

void CacheWatcher::synchronizeFailed(Cache::CacheDataType dataType, const QMap<QString, QVariant> &id)
{
	if (watchable(dataType, id))
		setSynchronizeState(SynchronizeError);
}

void CacheWatcher::clearWatching()
{
	watching.clear();
}

void CacheWatcher::watchFor(Cache::CacheDataType dataType, const QMap<QString, QVariant> &id)
{
	watching.insert(dataType, id);
}
