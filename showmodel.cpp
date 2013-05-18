#include <QSqlTableModel>
#include <QSqlField>
#include <QSqlQuery>

#include "showmodel.h"

ShowModel::ShowModel(QObject *parent) :
	SqlTableModel(parent)
{
}

bool ShowModel::addShow(const QString &showId, const QString &title)
{
	QSqlRecord record = QSqlDatabase::database().record("show");
	record.setValue("show_id", showId);
	record.setValue("title", title);
	bool v = insertRecord(-1, record);
	select();
	return v;
}
