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

#include "commandmanager.h"
#include "command.h"
#include "jsonparser.h"
#include "cache.h"

Cache *Cache::_instance = 0;

Cache &Cache::instance()
{
	if (!_instance)
		_instance = new Cache();
	return *_instance;
}

Cache::Cache() :
	QObject()
{
}

Cache::SynchronizeAction *Cache::getAction(Cache::SynchronizeAction::ActionType actionType, const QMap<QString, QVariant> &id)
{
	foreach (SynchronizeAction *action, currentActions) {
		if (action->actionType == actionType &&
				action->id == id) {
			return action;
		}
	}
	return 0;
}

void Cache::commandFinished(const QByteArray &response)
{
	Command *command = qobject_cast<Command*>(sender());
	TicketData ticketData = parsing[command];
	if (ticketData.showId.isNull())
		return;

	parsing.remove(command);

	// call the parse function
	QMetaObject::invokeMethod(this, ticketData.parseMethodName.toLocal8Bit(), Q_ARG(QString, ticketData.showId), Q_ARG(QByteArray, response));

	emit refreshDone(ticketData.showId);
}

void Cache::parseEpisode(const QString &showId, int season, const QJsonObject &root)
{
	bool ok;
	int episode = root.value("episode").toString().toInt(&ok);
	if (!ok)
		return;
	QString title = root.value("title").toString();
	QString number = root.value("number").toString();
	int global = root.value("global").toString().toInt();
	int date = root.value("date").toDouble();
	int comments = root.value("comments").toString().toInt();

	QSqlQuery query;
	query.prepare("INSERT INTO episode (show_id, season, episode, title, number, global, date, comments) "
				  "VALUES (:show_id, :season, :episode, :title, :number, :global, :date, :comments)");
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

void Cache::parseEpisodes(const QString &showId, int season, const QJsonObject &root)
{
	QJsonObject episodesJson = root.value("episodes").toObject();
	foreach (const QString &key, episodesJson.keys()) {
		QJsonObject episodeJson = episodesJson.value(key).toObject();
		if (episodeJson.isEmpty())
			continue;
		parseEpisode(showId, season, episodeJson);
	}
}

void Cache::parseSeasons(const QString &showId, const QByteArray &response)
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

		// numbers are under the form of strings from betaseries :/
		bool ok;
		int number = seasonJson.value("number").toString().toInt(&ok);
		if (!ok)
			continue;

		QSqlQuery query;
		query.prepare("INSERT INTO season (show_id, number) "
					  "VALUES (:show_id, :number)");
		query.bindValue(":show_id", showId);
		query.bindValue(":number", number);
		query.exec();
		parseEpisodes(showId, number, seasonJson);
	}

	// update the episodes last check date of the show
	QSqlQuery query;
	query.prepare("UPDATE show SET episodes_last_check_date=:epoch WHERE show_id=:showid");
	query.bindValue(":epoch", QDateTime::currentMSecsSinceEpoch() / 1000);
	query.bindValue(":showid", showId);
	query.exec();
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

	// update the season list
	query.prepare("DELETE FROM season WHERE show_id=:show_id");
	query.bindValue(":show_id", showId);
	query.exec();

	QJsonObject seasonsJson = showJson.value("seasons").toObject();
	foreach (const QString &key, seasonsJson.keys()) {
		QJsonObject seasonJson = seasonsJson.value(key).toObject();

		query.prepare("INSERT INTO season (show_id, number, episode_count) "
					  "VALUES (:show_id, :number, :episode_count)");
		query.bindValue(":show_id", showId);
		query.bindValue(":number", seasonJson.value("number").toString().toInt());
		query.bindValue(":episode_count", seasonJson.value("episodes").toDouble());
		query.exec();
	}

	QSqlDatabase::database().commit();
}

void Cache::showInfosCallback(const QString &showId, const QByteArray &response)
{
	// TODO: parseShowInfos must return a boolean for potential errors
	parseShowInfos(showId, response);
	emit showInfosSynchronized(showId);
}

int Cache::refreshOnExpired(const QString &showId, int season, int episode, bool description)
{
	QSqlQuery query;
	qint64 last_check_epoch = 0;
	qint64 expiration = 24 * 60 * 60 * 1000; // one day => TODO customizable
	// have we the season in database?
	// take the expiration date in account
	query.exec(QString("SELECT episodes_last_check_date FROM show WHERE show_id='%1'").arg(showId));
	if (query.next() && !query.value(0).isNull()) {
		last_check_epoch = query.value(0).toLongLong() * 1000;
	}
	if (QDateTime::currentDateTime().toMSecsSinceEpoch() - last_check_epoch > expiration) {
		// expired data, we need to launch the request
		Command *command = CommandManager::instance().showsEpisodes(showId);
		// TODO ticket can be invalid, manage it
		parsing.insert(command, TicketData(showId, "parseSeasons"));
		connect(command, &Command::finished, this, &Cache::commandFinished);
		return 1;
	} else
		return 0;
}

int Cache::synchronizeShowInfos(const QString &showId)
{
	QSqlQuery query;
	qint64 last_sync_epoch = 0;
	qint64 expiration = 24 * 60 * 60 * 1000; // one day => TODO customizable
	// have we the season in database?
	// take the expiration date in account
	query.exec(QString("SELECT last_sync FROM show WHERE show_id='%1'").arg(showId));
	if (query.next() && !query.value(0).isNull()) {
		last_sync_epoch = query.value(0).toLongLong() * 1000;
	}
	if (QDateTime::currentDateTime().toMSecsSinceEpoch() - last_sync_epoch <= expiration) {
		// data are already synchronized, we can use them
		return 0;
	}

	// is there a current action about it?
	QMap<QString,QVariant> id;
	id.insert("showId", showId);
	SynchronizeAction *action = getAction(SynchronizeAction::Action_ShowInfos, id);
	if (action) {
		// an action is already running, so just wait for the end of it
		return 1;
	}

	// expired data, we need to launch the request
	emit showInfosSynchronizing(showId);

	// store the synchronize action
	action = new SynchronizeAction;
	action->actionType = SynchronizeAction::Action_ShowInfos;
	action->id.insert("showId", showId);
	currentActions << action;

	Command *command = CommandManager::instance().showsDisplay(showId);
	parsing.insert(command, TicketData(showId, "showInfosCallback"));
	connect(command, &Command::finished, this, &Cache::commandFinished);
	return 1;
}

int Cache::synchronizeSeasonEpisodeList(const QString &showId, int season)
{
}
