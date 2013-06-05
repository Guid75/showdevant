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
		Synchronizing,
		Synchronized,
		SynchronizeError
	};

	explicit CacheWatcher(QObject *parent = 0);

	SynchronizeState synchronizeState() const { return _synchronizeState; }
signals:
	void synchronizeStateChanged();

public slots:
	void watchFor(Cache::CacheDataType dataType, const QMap<QString,QVariant> &id);
	void clearWatching();

protected:
	void setSynchronizeState(SynchronizeState state);

	// returns true if the synchronize event matching the watching
	bool watchable(Cache::CacheDataType dataType, const QMap<QString,QVariant> &id);

private:
	SynchronizeState _synchronizeState;
	QMultiMap<Cache::CacheDataType,QMap<QString,QVariant> > watching;

private slots:
	void synchronizing(Cache::CacheDataType dataType, const QMap<QString,QVariant> &id);
	void synchronized(Cache::CacheDataType dataType, const QMap<QString,QVariant> &id);
	void synchronizeFailed(Cache::CacheDataType dataType, const QMap<QString,QVariant> &id);
};

#endif // CACHEWATCHER_H
