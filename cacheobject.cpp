#include "cacheobject.h"

CacheObject::CacheObject(QObject *parent) :
    QObject(parent)
{

	connect(&Cache::instance(), &Cache::synchronizing,
			this, &CacheObject::synchronizing);

	connect(&Cache::instance(), &Cache::synchronized,
			this, &CacheObject::synchronized);
}

void CacheObject::setSynchronized(bool value)
{
	if (_synchronized == value)
		return;

	_synchronized = value;
	emit synchronizedChanged();
}

void CacheObject::setSynchronizing(bool value)
{
	if (_synchronizing == value)
		return;

	_synchronizing = value;
	emit synchronizingChanged();
}

bool CacheObject::handleSynchronizing(Cache::CacheDataType dataType, const QMap<QString, QVariant> &id)
{
	return false;
}

bool CacheObject::handleSynchronized(Cache::CacheDataType dataType, const QMap<QString, QVariant> &id)
{
	return false;
}

void CacheObject::synchronizing(Cache::CacheDataType dataType, const QMap<QString, QVariant> &id)
{
	if (!handleSynchronizing(dataType, id))
		return;

	setSynchronizing(true);
	setSynchronized(false);
}

void CacheObject::synchronized(Cache::CacheDataType dataType, const QMap<QString, QVariant> &id)
{
	if (!handleSynchronized(dataType, id))
		return;

	setSynchronizing(false);
	setSynchronized(true);
}

