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

#include <QSqlRecord>

#include "sqltablemodel.h"

SqlTableModel::SqlTableModel(QObject *parent) :
	QSqlTableModel(parent),
	_synchronized(false),
	_synchronizing(false),
	_sortOrder(Qt::AscendingOrder)
{
}

QVariant SqlTableModel::data(const QModelIndex &index, int role) const
{
	if (index.row() >= rowCount())
		return QString("");

	if (role < Qt::UserRole)
		return QSqlTableModel::data(index, role);

	return QSqlTableModel::data(this->index(index.row(), role - Qt::UserRole - 1), Qt::DisplayRole);
}

void SqlTableModel::generateRoleNames()
{
	roles.clear();
	int nbCols = this->columnCount();
	for (int i = 0; i < nbCols; i++) {
		roles[Qt::UserRole + i + 1] = QVariant(this->headerData(i, Qt::Horizontal).toString()).toByteArray();
	}
}

void SqlTableModel::setTable(const QString &tableName)
{
	QSqlTableModel::setTable(tableName);
	generateRoleNames();

	int col = fieldIndex(_sortField);
	if (col >= 0) {
		setSort(col, _sortOrder);
	}

	select();
}

Qt::SortOrder SqlTableModel::getSortOrder() const
{
	return _sortOrder;
}

void SqlTableModel::setSortOrder(Qt::SortOrder order)
{
	if (_sortOrder == order)
		return;

	_sortOrder = order;
}

const QString &SqlTableModel::getSortField() const
{
	return _sortField;
}

void SqlTableModel::setSortField(const QString &field)
{
	if (_sortField == field)
		return;

	_sortField = field;
}


void SqlTableModel::setSynchronized(bool value)
{
	if (_synchronized == value)
		return;

	_synchronized = value;
	emit synchronizedChanged();
}

void SqlTableModel::setSynchronizing(bool value)
{
	if (_synchronizing == value)
		return;

	_synchronizing = value;
	emit synchronizingChanged();
}
