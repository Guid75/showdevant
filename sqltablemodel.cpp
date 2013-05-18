#include <QSqlRecord>

#include "sqltablemodel.h"

SqlTableModel::SqlTableModel(QObject *parent) :
	QSqlTableModel(parent),
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
