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

#ifndef SQLTABLEMODEL_H
#define SQLTABLEMODEL_H

#include <QSqlTableModel>

class SqlTableModel : public QSqlTableModel
{
	Q_OBJECT
	Q_PROPERTY(int count READ getCount() NOTIFY countChanged())
	Q_PROPERTY(QString table READ tableName() WRITE setTable(tableName))
	Q_PROPERTY(Qt::SortOrder sortOrder READ getSortOrder() WRITE setSortOrder(order))
	Q_PROPERTY(QString sortField READ getSortField() WRITE setSortField(field))
	Q_PROPERTY(QString currentFilter READ filter() WRITE setFilter(filter))

	Q_PROPERTY(bool synchronized READ getSynchronized() NOTIFY synchronizedChanged())
	Q_PROPERTY(bool synchronizing READ getSynchronizing() NOTIFY synchronizingChanged())
public:
	SqlTableModel(QObject *parent = 0);
	Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole ) const;
	void generateRoleNames();
	virtual QHash<int, QByteArray> roleNames() const{return roles;}

	void setTable(const QString & tableName);

	int getCount() { this->count = this->rowCount(); return count; }

	Qt::SortOrder getSortOrder() const;
	void setSortOrder(Qt::SortOrder order);

	const QString &getSortField() const;
	void setSortField(const QString &field);

	bool getSynchronized() const { return _synchronized; }
	bool getSynchronizing() const { return _synchronizing; }
signals:
	void countChanged();

	void synchronizedChanged();
	void synchronizingChanged();

protected:
	void setSynchronized(bool value);
	void setSynchronizing(bool value);

private:
	bool _synchronized;
	bool _synchronizing;
	int count;
	QHash<int, QByteArray> roles;

	Qt::SortOrder _sortOrder;
	QString _sortField;
};

#endif // SQLTABLEMODEL_H
