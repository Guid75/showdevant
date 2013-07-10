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

#include <QDir>
#include <QSqlQuery>

#include "databasemanager.h"

DatabaseManager *DatabaseManager::_instance = 0;

DatabaseManager::DatabaseManager(QObject *parent) :
	QObject(parent)
{
}

bool DatabaseManager::createTable(QSqlQuery &query, const QString &table, const QString &request)
{
	bool b = query.exec(QString("CREATE TABLE %1 ").arg(table) + request);
	if (!b) {
		qCritical("Fail to create the \"%s\" table: %s", qPrintable(table), qPrintable(query.lastError().text()));
		return false;
	}
	return true;
}

int DatabaseManager::openDB()
{
	// Find QSLite driver
	db = QSqlDatabase::addDatabase("QSQLITE");

	QDir dir = QDir::home();
	if (dir.mkpath(".showdevant") && dir.cd(".showdevant")) {
		db.setDatabaseName(QDir::toNativeSeparators(dir.filePath("shows.sqlite")));
	}

	int ret = 2;
	// let try to open the persistent database
	if (!db.open()) {
		ret = 1;
		_openDBLastError = 1;
		// now let try with a memory database
		db.setDatabaseName(":memory:");
		if (!db.open()) {
			// even the memory fallback failed? Ouch!
			_openDBLastError = 0;
			return 0;
		}
	}
	_openDBLastError = 2;
	db.setConnectOptions("foreign_keys = ON");

	// create tables?
	// TODO: do not create if they exist and if they embed the needed fields
	// TODO: use ids for all tables
	QSqlQuery query;

	// my shows
	if (!createTable(query, "myshows",
					 "(show_id text, title text, archive integer, "
					 "UNIQUE (show_id) ON CONFLICT REPLACE)"))
		return ret;

	// shows
	if (!createTable(query, "show",
					 "(show_id text primary key, last_sync integer, title text, "
					 "description text, network text, duration integer, "
					 "UNIQUE (show_id) ON CONFLICT REPLACE)"))
		return ret;

	// seasons
	if (!createTable(query, "season",
					 "(show_id text, number integer, episode_count integer, last_sync_episode_list integer, "
					 "PRIMARY KEY (show_id, number), "
					 "FOREIGN KEY (show_id) REFERENCES show(show_id) ON DELETE CASCADE)"))
		return ret;

	// episodes
	if (!createTable(query, "episode",
					 "(show_id text, season integer, episode integer, title text, description text, "
					 "number text, global integer, date integer, seen integer, "
					 "comments integer, last_sync_detail integer, subtitles_last_check_date integer, "
					 "PRIMARY KEY (show_id, season, episode), "
					 "FOREIGN KEY (show_id, season) REFERENCES season(show_id, number) ON DELETE CASCADE)"))
		return ret;

	// subtitles
	if (!createTable(query, "subtitle",
					 "(id integer primary key, show_id text, season integer, episode integer, "
					 "language text, source text, file text, "
					 "url text, quality integer, "
					 "UNIQUE (show_id, season, episode, url) ON CONFLICT REPLACE)"))
		return ret;

	// subtitles content
	query.exec("CREATE TABLE subtitle_content "
			   "(subtitle_id integer, file text, "
			   "FOREIGN KEY (subtitle_id) REFERENCES subtitle(id) ON DELETE CASCADE, "
			   "UNIQUE (subtitle_id, file) ON CONFLICT REPLACE)");

	emit opened();

	return ret;
}

QSqlError DatabaseManager::lastError()
{
	// If opening database has failed user can ask
	// error description by QSqlError::text()
	return db.lastError();
}

DatabaseManager &DatabaseManager::instance()
{
	if (!_instance)
		_instance = new DatabaseManager();
	return *_instance;
}
