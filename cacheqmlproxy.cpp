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