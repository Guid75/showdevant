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

#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QVariant>
#include <QDebug>
#include <QMetaType>
#include <QSqlError>
#include <QThread>

#include "commandmanager.h"
#include "command.h"
#include "cache.h"

//#define OFFLINE 1

Cache *Cache::_instance = 0;

Cache &Cache::instance()
{
	if (!_instance)
		_instance = new Cache();
	return *_instance;
}

void Cache::start()
{
	thread->start();
}

Cache::Cache() :
	QObject()
{
	qRegisterMetaType<CacheDataType>("CacheDataType");

	connect(&commandMapper, SIGNAL(mapped(QObject *)),
			this, SLOT(commandFinished(QObject *)));

	thread = new QThread;
	this->moveToThread(thread);
}

Cache::~Cache()
{
	thread->exit();
	delete thread;
}

Cache::SynchronizeAction *Cache::getAction(CacheDataType dataType, const QVariantMap &id) const
{
	foreach (SynchronizeAction *action, currentActions) {
		if (action->dataType == dataType &&
				action->id == id) {
			return action;
		}
	}
	return 0;
}

Cache::SynchronizeAction *Cache::getAction(Command *command) const
{
	foreach (SynchronizeAction *action, currentActions) {
		if (action->commands.indexOf(command) >= 0)
			return action;
	}
	return 0;
}

void Cache::commandFinished(QObject *commandObj)
{	
	Command *command = qobject_cast<Command*>(commandObj);
	Q_ASSERT(command != 0);
	SynchronizeAction *action = getAction(command);
	Q_ASSERT(action != 0); // an action MUST exists for the command

	if (command->httpError())
		emit synchronizeFailed(action->dataType, action->id);
	else
		QMetaObject::invokeMethod(this, action->callbackMethodName.toLocal8Bit(),
								  Qt::DirectConnection, Q_ARG(QVariantMap, action->id), Q_ARG(JsonParser, command->jsonParser()));

	currentActions.removeOne(action);
	command->deleteLater();
}

void Cache::parseEpisodes(const QString &showId, int season, const QJsonObject &root, bool detailMode)
{
	QJsonObject episodesJson = root.value("episodes").toObject();
	foreach (const QString &key, episodesJson.keys()) {
		QJsonObject episodeJson = episodesJson.value(key).toObject();
		if (episodeJson.isEmpty())
			continue;

		int episode = episodeJson.value("episode").toString().toInt();
		QString title = episodeJson.value("title").toString();
		QString description = episodeJson.value("description").toString();
		QString number = episodeJson.value("number").toString();
		int global = episodeJson.value("global").toString().toInt();
		int date = episodeJson.value("date").toDouble();
		bool seen = episodeJson.value("has_seen").toString() == "1"; // only valid when are logged
		int comments = episodeJson.value("comments").toString().toInt();

		QSqlQuery query;
		query.prepare("REPLACE INTO episode (show_id, season, episode, title, description, number, global, "
					  "date, seen, comments) "
					  "VALUES (:show_id, :season, :episode, :title, :description, :number, :global, "
					  ":date, :seen, :comments)");

		query.bindValue(":show_id", showId);
		query.bindValue(":season", season);
		query.bindValue(":episode", episode);
		query.bindValue(":title", title);
		query.bindValue(":description", description);
		query.bindValue(":number", number);
		query.bindValue(":global", global);
		query.bindValue(":date", date);
		query.bindValue(":seen", seen);
		query.bindValue(":comments", comments);
		query.exec();
	}
}

void Cache::tagSeen(const QString &showId, int maxSeason, int maxEpisode)
{
	if (!QSqlDatabase::database().transaction()) {
		qCritical("Error while beginning a transaction for SQL insertion");
		return;
	}

	QSqlQuery query;

	// command successful, we must tag all previous episodes
	query.prepare("UPDATE episode SET seen=1 WHERE show_id=:showid AND (season < :season OR (season = :season AND episode <= :episode))");
	query.bindValue(":showid", showId);
	query.bindValue(":season", maxSeason);
	query.bindValue(":episode", maxEpisode);
	query.exec();

	QSqlDatabase::database().commit();
}

void Cache::parseSeasons(const QString &showId, const JsonParser &json, bool allEpisodes)
{
	QJsonObject seasonsJson = json.root().value("seasons").toObject();
	if (!QSqlDatabase::database().transaction()) {
		qCritical("Error while beginning a transaction for SQL insertion");
		return;
	}

	foreach (const QString &key, seasonsJson.keys()) {
		QJsonObject seasonJson = seasonsJson.value(key).toObject();
		if (seasonJson.isEmpty())
			continue;

		int number = seasonJson.value("number").toString().toInt();

		parseEpisodes(showId, number, seasonJson, !allEpisodes);

		if (allEpisodes) {
			// if we refresh all episodes, we can record the current date
			QSqlQuery query;
			query.prepare("UPDATE season SET last_sync_episode_list=:epoch WHERE show_id=:showid AND number=:number");
			query.bindValue(":epoch", QDateTime::currentMSecsSinceEpoch() / 1000);
			query.bindValue(":showid", showId);
			query.bindValue(":number", number);
			query.exec();
		}
	}

	// update the episodes last check date of the show
	QSqlDatabase::database().commit();
}

void Cache::parseShowInfos(const QString &showId, const JsonParser &json)
{
	QJsonObject showJson = json.root().value("show").toObject();
	if (!QSqlDatabase::database().transaction()) {
		qCritical("Error while beginning a transaction for SQL insertion");
		return;
	}

	// update the show details
	QSqlQuery query;
	query.prepare("INSERT OR REPLACE INTO show (show_id, title, description, network, duration, last_sync) "
				  "VALUES (:show_id, :title, :description, :network, :duration, :last_sync)");
	query.bindValue(":show_id", showId);
	query.bindValue(":title", showJson.value("title").toString());
	query.bindValue(":description", showJson.value("description").toString());
	query.bindValue(":network", showJson.value("network").toString());
	query.bindValue(":duration", showJson.value("duration").toString().toInt());
	query.bindValue(":last_sync", QDateTime::currentMSecsSinceEpoch() / 1000);
	query.exec();

	// init the "to delete" list
	QList<int> toDelete;
	query.prepare("SELECT number FROM season WHERE show_id=:show_id");
	query.bindValue(":show_id", showId);
	query.exec();
	while (query.next())
		toDelete << query.value("number").toInt();

	// make the replace/insertion operations and reduct the "to delete" list
	QJsonObject seasonsJson = showJson.value("seasons").toObject();
	foreach (const QString &key, seasonsJson.keys()) {
		QJsonObject seasonJson = seasonsJson.value(key).toObject();
		int number = seasonJson.value("number").toString().toInt();
		toDelete.removeOne(number);

		query.prepare("REPLACE INTO season (show_id, number, episode_count) "
					  "VALUES (:show_id, :number, :episode_count)");
		query.bindValue(":show_id", showId);
		query.bindValue(":number", number);
		query.bindValue(":episode_count", seasonJson.value("episodes").toDouble());
		query.exec();
	}

	// delete all seasons that have not been found in the JSON return
	foreach (int season, toDelete) {
		query.prepare("DELETE FROM season WHERE show_id=:show_id AND season=:season");
		query.bindValue(":show_id", showId);
		query.bindValue(":season", season);
		query.exec();
	}

	QSqlDatabase::database().commit();
}

void Cache::parseMemberInfos(const JsonParser &json)
{
	if (!QSqlDatabase::database().transaction()) {
		qCritical("Error while beginning a transaction for SQL insertion");
		return;
	}

	QJsonObject memberJson = json.root().value("member").toObject();

	// TODO parse member inner infos

	// init the "to delete" list
	QStringList toDelete;
	QSqlQuery query;
	query.prepare("SELECT show_id FROM myshows");
	query.exec();
	while (query.next())
		toDelete << query.value("show_id").toString();

	// parse shows
	QJsonObject showsJson = memberJson.value("shows").toObject();
	for (QJsonObject::const_iterator iter = showsJson.constBegin(); iter != showsJson.constEnd(); iter++) {
		QJsonObject showJson = (*iter).toObject();

		query.prepare("REPLACE INTO myshows (show_id, title, archive) "
					  "VALUES (:show_id, :title, :archive)");
		query.bindValue(":show_id", showJson.value("url").toString());
		query.bindValue(":title", showJson.value("title").toString());
		query.bindValue(":archive", showJson.value("archive").toString().toInt());
		query.exec();

		toDelete.removeOne(showJson.value("url").toString());
	}

	// delete all shows that have not been found in the JSON return
	foreach (const QString &showId, toDelete) {
		query.prepare("DELETE FROM myshows WHERE show_id=:show_id");
		query.bindValue(":show_id", showId);
		query.exec();
	}

	QSqlDatabase::database().commit();
}

bool Cache::parseAddShow(const QString &showId, const QString &title, const JsonParser &json)
{
	if (json.code() == 0) {
		// TODO manage error code
		return false;
	}

	if (!QSqlDatabase::database().transaction()) {
		qCritical("Error while beginning a transaction for SQL insertion");
		return false;
	}

	QSqlQuery query;
	query.prepare("INSERT INTO myshows (show_id, title, archive) "
				  "VALUES (:show_id, :title, :archive)");
	query.bindValue(":show_id", showId);
	query.bindValue(":title", title);
	query.bindValue(":archive", 0);
	query.exec();

	QSqlDatabase::database().commit();
	return true;
}

bool Cache::parseRemoveShow(const QString &showId, const JsonParser &json)
{
	if (json.code() == 0) {
		// TODO manage error code
		return false;
	}

	if (!QSqlDatabase::database().transaction()) {
		qCritical("Error while beginning a transaction for SQL insertion");
		return false;
	}

	QSqlQuery query;

	query.prepare("DELETE FROM myshows WHERE show_id=:show_id");
	query.bindValue(":show_id", showId);
	query.exec();

	QSqlDatabase::database().commit();
	return true;
}

void Cache::showInfosCallback(const QVariantMap &id, const JsonParser &json)
{
	QString showId = id["showId"].toString();
	// TODO: parseShowInfos must return a boolean for potential errors
	parseShowInfos(showId, json);
	emit synchronized(Data_ShowInfos, id);
}

void Cache::episodesCallback(const QVariantMap &id, const JsonParser &json)
{
	qDebug("episodesCallback");
	QString showId = id["showId"].toString();
	parseSeasons(showId, json, id["episode"].isNull());
	emit synchronized(Data_Episodes, id);
}

void Cache::memberInfosCallback(const QVariantMap &id, const JsonParser &json)
{
	parseMemberInfos(json);
	emit synchronized(Data_MemberInfos, id);
}

void Cache::addShowCallback(const QVariantMap &id, const JsonParser &json)
{
	if (parseAddShow(id["showId"].toString(), id["title"].toString(), json))
		emit showAdded(id["title"].toString());
	emit synchronized(Data_AddShow, id);
}

void Cache::removeShowCallback(const QVariantMap &id, const JsonParser &json)
{
	// get the title into the database
	QSqlQuery query;
	query.prepare("SELECT title FROM myshows WHERE show_id=:show_id");
	query.bindValue(":show_id", id["showId"]);
	query.exec();
	QString title;
	if (query.next())
		title = query.value("title").toString();

	if (parseRemoveShow(id["showId"].toString(), json))
		emit showRemoved(title);
	emit synchronized(Data_RemoveShow, id);
}

void Cache::archiveShowCallback(const QVariantMap &id, const JsonParser &json)
{
	// TODO
	emit synchronized(Data_ArchiveShow, id);
}

void Cache::watchShowCallback(const QVariantMap &id, const JsonParser &json)
{
	if (json.code() == 0) {
		// TODO treat errors
		emit synchronized(Data_WatchShow, id);
		return;
	}

	tagSeen(id["showId"].toString(), id["season"].toInt(), id["episode"].toInt());

	emit synchronized(Data_WatchShow, id);
}

int Cache::synchronizeShowInfos(const QString &showId)
{
	qint64 last_sync_epoch = 0;
	qint64 expiration = 24 * 60 * 60 * 1000; // one day => TODO customizable
	QVariantMap id;
	id.insert("showId", showId);
	// have we the season in database?
	// take the expiration date in account
	QSqlQuery query;
	query.exec(QString("SELECT last_sync FROM show WHERE show_id='%1'").arg(showId));
	if (query.next() && !query.value(0).isNull()) {
		last_sync_epoch = query.value(0).toLongLong() * 1000;
	}

#ifdef OFFLINE
	emit synchronized(Data_ShowInfos, id);
	return 0;
#endif

	if (QDateTime::currentDateTime().toMSecsSinceEpoch() - last_sync_epoch <= expiration) {
		// data are already synchronized, we can use them
		emit synchronized(Data_ShowInfos, id);
		return 0;
	}

	// expired data, we need to launch the request if not already done
	emit synchronizing(Data_ShowInfos, id);

	// is there a current action about it?
	SynchronizeAction *action = getAction(Data_ShowInfos, id);
	if (action) {
		// just wait for the end of it
		return 1;
	}

	// store the synchronize action
	action = new SynchronizeAction;
	action->dataType = Data_ShowInfos;
	action->id = id;
	action->callbackMethodName = "showInfosCallback";
	Command *command = CommandManager::instance().showsDisplay(showId);
	action->commands << command;
	currentActions << action;
	commandMapper.setMapping(command, command);
	connect(command, SIGNAL(finished()), &commandMapper, SLOT(map()));
	return 1;
}

int Cache::synchronizeEpisodes(const QString &showId, int season, int episode, bool fullInfo)
{
	qint64 last_sync_epoch = 0;
	qint64 expiration = 24 * 60 * 60 * 1000; // one day => TODO customizable

	QVariantMap id;
	id.insert("showId", showId);
	id.insert("season", season);
	if (episode >= 0)
		id.insert("episode", episode);

	// have we the season in database?
	// take the expiration date in account
	QSqlQuery query;

	if (episode == -1)
		query.prepare("SELECT last_sync_episode_list FROM season WHERE show_id=:show_id AND number=:season");
	else
		query.prepare("SELECT last_sync_detail FROM episode WHERE show_id=:show_id AND season=:season AND episode=:episode");

	query.bindValue(":show_id", showId);
	query.bindValue(":season", season);
	if (episode >= 0)
		query.bindValue(":episode", episode);
	query.exec();
	if (query.next() && !query.value(0).isNull())
		last_sync_epoch = query.value(0).toLongLong() * 1000;

#ifdef OFFLINE
	emit synchronized(Data_Episodes, id);
	return 0;
#endif

	if (QDateTime::currentDateTime().toMSecsSinceEpoch() - last_sync_epoch <= expiration) {
		// data are already synchronized, we can use them
		emit synchronized(Data_Episodes, id);
		return 0;
	}

	// expired data, we need to launch the request if not already done
	emit synchronizing(Data_Episodes, id);

	// is there a current action about it?
	SynchronizeAction *action = getAction(Data_Episodes, id);
	if (action) {
		// just wait for the end of it
		return 1;
	}

	// store the synchronize action
	action = new SynchronizeAction;
	action->dataType = Data_Episodes;
	action->id = id;
	action->callbackMethodName = "episodesCallback";
	Command *command = CommandManager::instance().showsEpisodes(showId, season, episode, !fullInfo, !fullInfo);
	action->commands << command;
	currentActions << action;
	commandMapper.setMapping(command, command);
	connect(command, SIGNAL(finished()), &commandMapper, SLOT(map()));
	return 1;
}

int Cache::synchronizeMemberInfos()
{
	QVariantMap id;

	// expired data, we need to launch the request if not already done
	emit synchronizing(Data_MemberInfos, id);

	// is there a current action about it?
	SynchronizeAction *action = getAction(Data_MemberInfos, id);
	if (action) {
		// just wait for the end of it
		return 1;
	}

	// store the synchronize action
	action = new SynchronizeAction;
	action->dataType = Data_MemberInfos;
	action->id = id;
	action->callbackMethodName = "memberInfosCallback";
	Command *command = CommandManager::instance().membersInfos();
	action->commands << command;
	currentActions << action;
	commandMapper.setMapping(command, command);
	connect(command, SIGNAL(finished()), &commandMapper, SLOT(map()));
	return 1;
}

int Cache::addShow(const QString &showId, const QString &title)
{
	QVariantMap id;
	id.insert("showId", showId);
	id.insert("title", title);

	// expired data, we need to launch the request if not already done
	emit synchronizing(Data_AddShow, id);

	// store the synchronize action
	SynchronizeAction *action = new SynchronizeAction;
	action->dataType = Data_AddShow;
	action->id = id;
	action->callbackMethodName = "addShowCallback";
	Command *command = CommandManager::instance().showsAdd(showId);
	action->commands << command;
	currentActions << action;
	commandMapper.setMapping(command, command);
	connect(command, SIGNAL(finished()), &commandMapper, SLOT(map()));
	return 1;
}

int Cache::removeShow(const QString &showId)
{
	QVariantMap id;
	id.insert("showId", showId);

	// expired data, we need to launch the request if not already done
	emit synchronizing(Data_RemoveShow, id);

	// store the synchronize action
	SynchronizeAction *action = new SynchronizeAction;
	action->dataType = Data_RemoveShow;
	action->id = id;
	action->callbackMethodName = "removeShowCallback";
	Command *command = CommandManager::instance().showsRemove(showId);
	action->commands << command;
	currentActions << action;
	commandMapper.setMapping(command, command);
	connect(command, SIGNAL(finished()), &commandMapper, SLOT(map()));
	return 1;
}

int Cache::archiveShow(const QString &showId)
{
	QVariantMap id;
	id.insert("showId", showId);

	// expired data, we need to launch the request if not already done
	emit synchronizing(Data_ArchiveShow, id);

	// store the synchronize action
	SynchronizeAction *action = new SynchronizeAction;
	action->dataType = Data_ArchiveShow;
	action->id = id;
	action->callbackMethodName = "archiveShowCallback";
	Command *command = CommandManager::instance().showsArchive(showId);
	action->commands << command;
	currentActions << action;
	commandMapper.setMapping(command, command);
	connect(command, SIGNAL(finished()), &commandMapper, SLOT(map()));
	return 1;
}

int Cache::watchShow(const QString &showId, int season, int episode)
{
	QVariantMap id;
	id.insert("showId", showId);
	id.insert("season", season);
	id.insert("episode", episode);

	// expired data, we need to launch the request if not already done
	emit synchronizing(Data_WatchShow, id);

	// store the synchronize action
	SynchronizeAction *action = new SynchronizeAction;
	action->dataType = Data_WatchShow;
	action->id = id;
	action->callbackMethodName = "watchShowCallback";
	Command *command = CommandManager::instance().membersWatched(showId, season, episode);
	action->commands << command;
	currentActions << action;
	commandMapper.setMapping(command, command);
	connect(command, SIGNAL(finished()), &commandMapper, SLOT(map()));
	return 1;
}
