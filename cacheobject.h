#ifndef CACHEOBJECT_H
#define CACHEOBJECT_H

#include <QObject>

#include "cache.h"

class CacheObject : public QObject
{
	Q_OBJECT

	Q_PROPERTY(bool synchronized READ getSynchronized() NOTIFY synchronizedChanged())
	Q_PROPERTY(bool synchronizing READ getSynchronizing() NOTIFY synchronizingChanged())

public:
	explicit CacheObject(QObject *parent = 0);

	bool getSynchronized() const { return _synchronized; }
	bool getSynchronizing() const { return _synchronizing; }

signals:
	void synchronizedChanged();
	void synchronizingChanged();

protected:
	void setSynchronized(bool value);
	void setSynchronizing(bool value);

	// functions to overload in the inherited classes
	virtual bool handleSynchronizing(Cache::CacheDataType dataType, const QMap<QString,QVariant> &id);
	virtual bool handleSynchronized(Cache::CacheDataType dataType, const QMap<QString,QVariant> &id);

private:
	bool _synchronized;
	bool _synchronizing;

private slots:
	void synchronizing(Cache::CacheDataType dataType, const QMap<QString,QVariant> &id);
	void synchronized(Cache::CacheDataType dataType, const QMap<QString,QVariant> &id);
};

#endif // CACHEOBJECT_H
