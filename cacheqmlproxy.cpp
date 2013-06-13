#include "cache.h"

#include "cacheqmlproxy.h"

CacheQmlProxy::CacheQmlProxy(QObject *parent) :
    QObject(parent)
{
	connect(&Cache::instance(), &Cache::showAdded,
			this, &CacheQmlProxy::showAdded);
}
