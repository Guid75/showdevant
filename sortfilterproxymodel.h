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
