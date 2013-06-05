#ifndef SQLQUERYMODEL_H
#define SQLQUERYMODEL_H

#include <QSqlQueryModel>

#include "cachewatcher.h"

class SqlQueryModel : public QSqlQueryModel
{
	Q_OBJECT
	Q_PROPERTY(QString query READ getQuery WRITE setQuery)
	Q_PROPERTY(int count READ getCount() NOTIFY countChanged())

	Q_PROPERTY(CacheWatcher::SynchronizeState synchronizeState READ synchronizeState() NOTIFY synchronizeStateChanged())

public:
	explicit SqlQueryModel(QObject *parent = 0);

	const QString &getQuery() const { return _query; }
	Q_INVOKABLE void setQuery(const QString &query);
	Q_INVOKABLE void setQuery(const QSqlQuery &query);
	QVariant data(const QModelIndex &index, int role) const;
	virtual QHash<int, QByteArray> roleNames() const{return roles;}
	int getCount() { this->count = this->rowCount(); return count; }
	Q_INVOKABLE QVariant get(int row);

	CacheWatcher::SynchronizeState synchronizeState() const;

signals:
	void countChanged();
	void synchronizeStateChanged();

protected:
	CacheWatcher cacheWatcher;

	virtual void select() = 0;

private:
	int count;
	QString _query;
	QHash<int, QByteArray> roles;

	void generateRoleNames();

private slots:
	void slotSynchronizeStateChanged();
};

#endif // SQLQUERYMODEL_H
