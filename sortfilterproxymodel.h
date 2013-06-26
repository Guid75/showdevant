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

#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

#include "sqlquerymodel.h"

class SortFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
	Q_PROPERTY(int count READ getCount() NOTIFY countChanged())
	Q_PROPERTY(QString sortField READ getSortField() WRITE setSortField(field))
	Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder() WRITE setSortOrder(order))
	Q_PROPERTY(SqlQueryModel *sourceModel READ sourceModel() WRITE setSourceModel(model))
	Q_PROPERTY(QString filterField READ getFilterField() WRITE setFilterField(field))
	Q_PROPERTY(QString filter READ getFilter() WRITE setFilter(filter))

public:
	explicit SortFilterProxyModel(QObject *parent = 0);

	int getCount() { this->count = this->rowCount(); return count; }

	SqlQueryModel *sourceModel() const;
	void setSourceModel(SqlQueryModel *sourceModel);

	const QString &getSortField() const { return _sortField; }
	void setSortField(const QString &field);
	void setSortOrder(Qt::SortOrder order);

	const QString &getFilterField() const { return _filterField; }
	void setFilterField(const QString &field);
	const QString &getFilter() const { return _filter; }
	void setFilter(const QString &filter);

	Q_INVOKABLE QVariant get(int row);

signals:
	void countChanged();

private:
	int count;
	QString _sortField;
	Qt::SortOrder _sortOrder;
	QString _filterField;
	QString _filter;
};

#endif // SORTFILTERPROXYMODEL_H
