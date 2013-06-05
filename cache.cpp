//  Copyright 2013 Guillaume Denry (guillaume.denry@gmail.com)
//  This file is part of BetaSeeker.
//
//  BetaSeeker is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  BetaSeeker is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with BetaSeeker.  If not, see <http://www.gnu.org/licenses/>.

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
#include "jsonparser.h"
#include "cache.h"

// alias only for invokeMethod
typedef QMap<QString,QVariant> ActionId;

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
	qRegisterMetaType<ActionId>("ActionId");
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

Cache::SynchronizeAction *Cache::getAction(CacheDataType dataType, const QMap<QString,QVariant> &id) const
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

	if (command->error())
		emit synchronizeFailed(action->dataType, action->id);
	else
		QMetaObject::invokeMethod(this, action->callbackMethodName.toLocal8Bit(), Qt::DirectConnection, Q_ARG(ActionId, action->id), Q_ARG(QByteArray, command->response()));
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
		QString number = episodeJson.value("number").toString();
		int global = episodeJson.value("global").toString().toInt();
		int date = episodeJson.value("date").toDouble();
		int comments = episodeJson.value("comments").toString().toInt();

		QSqlQuery query;
		query.prepare("REPLACE INTO episode (show_id, season, episode, title, number, global, "
					  "date, comments) "
					  "VALUES (:show_id, :season, :episode, :title, :number, :global, "
					  ":date, :comments)");

		query.bindValue(":show_id", showId);
		query.bindValue(":season", season);
		query.bindValue(":episode", episode);
		query.bindValue(":title", title);
		query.bindValue(":number", number);
		query.bindValue(":global", global);
		query.bindValue(":date", date);
		query.bindValue(":comments", comments);
		query.exec();
	}
}

void Cache::parseSeasons(const QString &showId, const QByteArray &response, bool allEpisodes)
{
	JsonParser parser(response);
	if (!parser.isValid()) {
		// TODO manage error
		return;
	}

	QJsonObject seasonsJson = parser.root().value("seasons").toObject();
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

void Cache::parseShowInfos(const QString &showId, const QByteArray &response)
{
	JsonParser parser(response);
	if (!parser.isValid()) {
		// TODO manage error
		return;
	}

	QJsonObject showJson = parser.root().value("show").toObject();
	if (!QSqlDatabase::database().transaction()) {
		qCritical("Error while beginning a transaction for SQL insertion");
		return;
	}

	// update the show details
	QSqlQuery query;
	query.prepare("UPDATE show SET title=:title,description=:description, "
				  "network=:network,duration=:duration,last_sync=:last_sync WHERE show_id=:show_id");
	query.bindValue(":title", showJson.value("title").toString());
	query.bindValue(":description", showJson.value("description").toString());
	query.bindValue(":network", showJson.value("network").toString());
	query.bindValue(":duration", showJson.value("duration").toString().toInt());
	query.bindValue(":last_sync", QDateTime::currentMSecsSinceEpoch() / 1000);
	query.bindValue(":show_id", showId);
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

void Cache::showInfosCallback(const QMap<QString,QVariant> &id, const QByteArray &response)
{
	QString showId = id["showId"].toString();
	// TODO: parseShowInfos must return a boolean for potential errors
	parseShowInfos(showId, response);
	emit synchronized(Data_ShowInfos, id);
}

void Cache::episodesCallback(const QMap<QString,QVariant> &id, const QByteArray &response)
{
	QString showId = id["showId"].toString();
	parseSeasons(showId, response, id["episode"].isNull());
	emit synchronized(Data_Episodes, id);
}

int Cache::synchronizeShowInfos(const QString &showId)
{
	qint64 last_sync_epoch = 0;
	qint64 expiration = 24 * 60 * 60 * 1000; // one day => TODO customizable
	QMap<QString,QVariant> id;
	id.insert("showId", showId);
	// have we the season in database?
	// take the expiration date in account
	QSqlQuery query;
	query.exec(QString("SELECT last_sync FROM show WHERE show_id='%1'").arg(showId));
	if (query.next() && !query.value(0).isNull()) {
		last_sync_epoch = query.value(0).toLongLong() * 1000;
	}
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

	QMap<QString,QVariant> id;
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
