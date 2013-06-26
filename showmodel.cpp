//  Copyright 2013 Guillaume Denry (guillaume.denry@gmail.com)
//  This file is part of ShowDevant.
//
//  ShowDevant is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ShowDevant is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with ShowDevant.  If not, see <http://www.gnu.org/licenses/>.

#include <QSqlTableModel>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlError>

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
	else
		qDebug(qPrintable(query.lastError().text()));


	return v;

	// the following code does not work with an id primary key in the show table
	/*QSqlRecord record = QSqlDatabase::database().record("show");
	record.setValue("show_id", showId);
	record.setValue("title", title);
	bool v = insertRecord(-1, record);
	return v;*/
}
