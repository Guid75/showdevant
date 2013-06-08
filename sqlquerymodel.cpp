#include <QSqlRecord>
#include <QSqlField>
#include <QSqlQuery>

#include "sqlquerymodel.h"

SqlQueryModel::SqlQueryModel(QObject *parent) :
	QSqlQueryModel(parent)
{
	// to re-emit the signal
	connect(&cacheWatcher, &CacheWatcher::synchronizeStateChanged,
			this, &SqlQueryModel::synchronizeStateChanged);
	// to re-do the select
	connect(&cacheWatcher, &CacheWatcher::synchronizeStateChanged,
			this, &SqlQueryModel::slotSynchronizeStateChanged);
}

void SqlQueryModel::setQuery(const QString &query)
{
	_query = query;
	QSqlQueryModel::setQuery(query);
	generateRoleNames();
}

void SqlQueryModel::setQuery(const QSqlQuery &query)
{
	generateRoleNames(query.record());
	QSqlQueryModel::setQuery(query);
}

void SqlQueryModel::generateRoleNames(const QSqlRecord &rec)
{
	roles.clear();
	if (rec.isEmpty()) {
		for( int i = 0; i < record().count(); i++)
			roles.insert(Qt::UserRole + i + 1, record().fieldName(i).toLocal8Bit());
	} else {
		for( int i = 0; i < rec.count(); i++)
			roles.insert(Qt::UserRole + i + 1, rec.fieldName(i).toLocal8Bit());
	}
}

void SqlQueryModel::slotSynchronizeStateChanged()
{
	if (cacheWatcher.synchronizeState() == CacheWatcher::Synchronized)
		select();
}

QVariant SqlQueryModel::data(const QModelIndex &index, int role) const
{
	QVariant value = QSqlQueryModel::data(index, role);
	if(role < Qt::UserRole)
	{
		value = QSqlQueryModel::data(index, role);
	}
	else
	{
		int columnIdx = role - Qt::UserRole - 1;
		QModelIndex modelIndex = this->index(index.row(), columnIdx);
		value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
	}
	return value;
}

QVariant SqlQueryModel::get(int row)
{
	QVariantMap itemData;
	foreach (int role, roles.keys())
		itemData.insert(roles[role], data(createIndex(row, 0), role));

	return QVariant(itemData);
}

CacheWatcher::SynchronizeState SqlQueryModel::synchronizeState() const
{
	return cacheWatcher.synchronizeState();
}
