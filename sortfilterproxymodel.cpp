#include "sortfilterproxymodel.h"

SortFilterProxyModel::SortFilterProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent),
	_sortOrder(sortOrder())
{
}

SqlTableModel *SortFilterProxyModel::sourceModel() const
{
	return qobject_cast<SqlTableModel*>(QSortFilterProxyModel::sourceModel());
}

void SortFilterProxyModel::setSourceModel(SqlTableModel *sourceModel)
{
	QSortFilterProxyModel::setSourceModel(sourceModel);
	SqlTableModel *model = this->sourceModel();

	if (!_sortField.isEmpty()) {
		if (model)
			sort(model->fieldIndex(_sortField), _sortOrder);
	}
	if (!_filterField.isEmpty()) {
		if (model)
			setFilterKeyColumn(model->fieldIndex(_filterField));
		if (!_filter.isEmpty())
			setFilterFixedString(_filter);
	}
}

void SortFilterProxyModel::setSortField(const QString &field)
{
	if (_sortField == field)
		return;

	_sortField = field;
	SqlTableModel *model = sourceModel();
	if (model)
		sort(model->fieldIndex(field), _sortOrder);
}

void SortFilterProxyModel::setSortOrder(Qt::SortOrder order)
{
	if (_sortOrder == order)
		return;

	_sortOrder = order;
	SqlTableModel *model = sourceModel();
	if (model && !_sortField.isEmpty())
		sort(model->fieldIndex(_sortField), _sortOrder);
}

void SortFilterProxyModel::setFilterField(const QString &field)
{
	if (_filterField == field)
		return;

	_filterField = field;
	SqlTableModel *model = sourceModel();
	if (model)
		setFilterKeyColumn(model->fieldIndex(field));
}

void SortFilterProxyModel::setFilter(const QString &filter)
{
	if (_filter == filter)
		return;

	_filter = filter;
/*	SqlTableModel *model = sourceModel();
	if (model) {*/
		this->setFilterFixedString(filter);
	/*}*/
}
