#ifndef CACHEQMLPROXY_H
#define CACHEQMLPROXY_H

#include <QObject>

class CacheQmlProxy : public QObject
{
	Q_OBJECT
public:
	explicit CacheQmlProxy(QObject *parent = 0);


signals:
	void showAdded(const QString &title);

public slots:
	
};

#endif // CACHEQMLPROXY_H
