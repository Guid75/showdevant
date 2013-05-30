#include <QSqlTableModel>
#include <QSqlField>
#include <QSqlQuery>

#include "showmodel.h"

ShowModel::ShowModel(QObject *parent) :
	SqlTableModel(parent)
{
	setTable("show");
}

bool ShowModel::addShow(const QString &showId, const QString &title)
{
	QSqlQuery query;
	query.prepare("INSERT INTO show (show_id, title) "
				  "VALUES (:show_id, :title)");
	query.bindValue(":show_id", showId);
	query.bindValue(":title", title);
	bool v = query.exec();

	if (v)
		select();

	return v;

	// the following code does not work with an id primary key in the show table
	/*QSqlRecord record = QSqlDatabase::database().record("show");
	record.setValue("show_id", showId);
	record.setValue("title", title);
	bool v = insertRecord(-1, record);
	return v;*/
}
