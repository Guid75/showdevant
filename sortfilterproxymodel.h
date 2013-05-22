#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

#include "sqltablemodel.h"

class SortFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
	Q_PROPERTY(int count READ getCount() NOTIFY countChanged())
	Q_PROPERTY(QString sortField READ getSortField() WRITE setSortField(field))
	Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder() WRITE setSortOrder(order))
	Q_PROPERTY(SqlTableModel *sourceModel READ sourceModel() WRITE setSourceModel(model))
	Q_PROPERTY(Qt::CaseSensitivity sortCaseSensitivity READ sortCaseSensitivity() WRITE setSortCaseSensitivity(sensitivity))

public:
	explicit SortFilterProxyModel(QObject *parent = 0);

	int getCount() { this->count = this->rowCount(); return count; }

	SqlTableModel *sourceModel() const;
	void setSourceModel(SqlTableModel *sourceModel);

	const QString &getSortField() const { return _sortField; }
	void setSortField(const QString &field);

	void setSortOrder(Qt::SortOrder order);

signals:
	void countChanged();

private:
	int count;
	QString _sortField;
	Qt::SortOrder _sortOrder;
};

#endif // SORTFILTERPROXYMODEL_H
