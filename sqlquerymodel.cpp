#include <QSqlRecord>
#include <QSqlField>

#include "sqlquerymodel.h"

SqlQueryModel::SqlQueryModel(QObject *parent) :
	QSqlQueryModel(parent)
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
	for( int i = 0; i < record().count(); i++) {
		roles.insert(Qt::UserRole + i + 1, record().fieldName(i).toLocal8Bit());
	}
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
