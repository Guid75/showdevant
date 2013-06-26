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

#ifndef SQLQUERYMODEL_H
#define SQLQUERYMODEL_H

#include <QSqlQueryModel>
#include <QSqlRecord>

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
	virtual QHash<int, QByteArray> roleNames() const{ return roles; }
	int getCount() { this->count = this->rowCount(); return count; }
	Q_INVOKABLE QVariant get(int row);

	CacheWatcher::SynchronizeState synchronizeState() const;

signals:
	void countChanged();
	void synchronizeStateChanged();

protected:
	CacheWatcher cacheWatcher;

	virtual void select() {}

private:
	int count;
	QString _query;
	QHash<int, QByteArray> roles;

	void generateRoleNames(const QSqlRecord &rec = QSqlRecord());

private slots:
	void slotSynchronizeStateChanged();
};

#endif // SQLQUERYMODEL_H
