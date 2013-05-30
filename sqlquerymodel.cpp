#include <QSqlRecord>
#include <QSqlField>

#include "sqlquerymodel.h"

SqlQueryModel::SqlQueryModel(QObject *parent) :
	QSqlQueryModel(parent),
	_synchronized(false),
	_synchronizing(false)
{

}

void SqlQueryModel::setQuery(const QString &query)
{
	_query = query;
	QSqlQueryModel::setQuery(query);
	generateRoleNames();
}

void SqlQueryModel::setQuery(const QSqlQuery &query)
{
	QSqlQueryModel::setQuery(query);
	generateRoleNames();
}

void SqlQueryModel::generateRoleNames()
{
	for( int i = 0; i < record().count(); i++)
		roles.insert(Qt::UserRole + i + 1, record().fieldName(i).toLocal8Bit());
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
	QMap<QString, QVariant> itemData;
	foreach (int role, roles.keys())
		itemData.insert(roles[role], data(createIndex(row, 0), role));

	return QVariant(itemData);
}


void SqlQueryModel::setSynchronized(bool value)
{
	if (_synchronized == value)
		return;

	_synchronized = value;
	emit synchronizedChanged();
}

void SqlQueryModel::setSynchronizing(bool value)
{
	if (_synchronizing == value)
		return;

	_synchronizing = value;
	emit synchronizingChanged();
}
