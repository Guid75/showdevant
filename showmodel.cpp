#include <QSqlTableModel>
#include <QSqlField>
#include <QSqlQuery>

#include "showmodel.h"

ShowModel::ShowModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int ShowModel::rowCount(const QModelIndex &parent) const
{
	if (!parent.isValid())
		return shows.count();
	return 0;
}

void ShowModel::setTable(const QString &tableName)
{
	if (_tableName == tableName)
		return;

	_tableName = tableName;

	generateRoles();
	select();
}

bool sqlRecordLessThan(const QSqlRecord &rec1, const QSqlRecord &rec2)
{
	return rec1.value("title").toString().toUpper() < rec2.value("title").toString().toUpper();
}

void ShowModel::select()
{
	shows.clear();
	QSqlQuery query("SELECT * from show");
	while (query.next())
		shows << query.record();

	qSort(shows.begin(), shows.end(), sqlRecordLessThan);
}

QVariant ShowModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role <= Qt::UserRole)
		return QVariant();

	const QSqlRecord &record = shows[index.row()];
	return record.value(role - Qt::UserRole - 1);
}

bool ShowModel::addShow(const QString &showId, const QString &title)
{
	// insert into the database and into the internal list

	QSqlQuery query;
	query.prepare("INSERT INTO show (show_id, title) "
				  "VALUES (:show_id, :title)");
	query.bindValue(":show_id", showId);
	query.bindValue(":title", title);
	if (!query.exec()) {
		qCritical("Insertion failed");
		return false;
	}

	// content?
	int showRowId = query.lastInsertId().toInt();
	query.prepare("SELECT * FROM show where rowid=:row_id");
	query.bindValue(":row_id", showRowId);
	query.exec();

	if (!query.next())
		return false;

	int insertionIndex = getInsertionIndex(query.record());

	beginInsertRows(QModelIndex(), insertionIndex, insertionIndex);
	shows.insert(insertionIndex, query.record());
	endInsertRows();
	return true;
}

void ShowModel::generateRoles()
{
	if (_tableName.isEmpty())
		return;

	QSqlTableModel tableModel;
	tableModel.setTable(_tableName);

	roles.clear();

	QSqlRecord rec = tableModel.record();

	for (int col = 0; col < tableModel.columnCount(); col++) {
		roles.insert(Qt::UserRole + col + 1, rec.fieldName(col).toLocal8Bit());
	}
}

int ShowModel::getInsertionIndex(const QSqlRecord &record) const
{
	QString recTitle = record.value("title").toString();
	for (int i = 0; i < shows.count(); i++) {
		QString title = shows[i].value("title").toString();
		if (recTitle.toUpper() < title.toUpper())
			return i;
	}

	return shows.count();
}
