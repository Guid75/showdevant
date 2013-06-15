#include "sortfilterproxymodel.h"

SortFilterProxyModel::SortFilterProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent),
	_sortOrder(sortOrder())
{
}

SqlQueryModel *SortFilterProxyModel::sourceModel() const
{
	return qobject_cast<SqlQueryModel*>(QSortFilterProxyModel::sourceModel());
}

void SortFilterProxyModel::setSourceModel(SqlQueryModel *sourceModel)
{
	QSortFilterProxyModel::setSourceModel(sourceModel);
	SqlQueryModel *model = this->sourceModel();

	if (!_sortField.isEmpty()) {
		if (model)
			sort(model->record().indexOf(_sortField), _sortOrder);
	//		sort(model->fieldIndex(_sortField), _sortOrder);
	}
	if (!_filterField.isEmpty()) {
		if (model)
//			setFilterKeyColumn(model->fieldIndex(_filterField));
			setFilterKeyColumn(model->record().indexOf(_filterField));
		if (!_filter.isEmpty())
			setFilterFixedString(_filter);
	}
}

void SortFilterProxyModel::setSortField(const QString &field)
{
	if (_sortField == field)
		return;

	_sortField = field;
	SqlQueryModel *model = sourceModel();
	if (model)
		sort(model->record().indexOf(field), _sortOrder);
}

void SortFilterProxyModel::setSortOrder(Qt::SortOrder order)
{
	if (_sortOrder == order)
		return;

	_sortOrder = order;
	SqlQueryModel *model = sourceModel();
	if (model && !_sortField.isEmpty())
		sort(model->record().indexOf(_sortField), _sortOrder);
}

void SortFilterProxyModel::setFilterField(const QString &field)
{
	if (_filterField == field)
		return;

	_filterField = field;
	SqlQueryModel *model = sourceModel();
	if (model)
		setFilterKeyColumn(model->record().indexOf(field));
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

QVariant SortFilterProxyModel::get(int row)
{
	QModelIndex sourceIdx = mapToSource(index(row, 0));
	return sourceModel()->get(sourceIdx.row());
}
