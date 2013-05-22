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

	if (!_sortField.isEmpty()) {
		SqlTableModel *model = this->sourceModel();
		if (model)
			sort(model->fieldIndex(_sortField), _sortOrder);
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
