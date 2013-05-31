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

#ifndef CACHE_H
#define CACHE_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QFuture>
#include <QFutureWatcher>

class Command;

class Cache : public QObject
{
	Q_OBJECT
public:
	enum CacheDataType {
		Data_ShowInfos,
		Data_EpisodeList
	};

	static Cache &instance();

	Q_INVOKABLE int synchronizeShowInfos(const QString &showId);

	Q_INVOKABLE int synchronizeSeasonEpisodeList(const QString &showId, int season);

signals:
	void synchronizing(CacheDataType dataType, QMap<QString,QVariant> id);
	void synchronized(CacheDataType dataType, QMap<QString,QVariant> id);

private:
	struct SynchronizeAction {
		enum ActionType {
			Action_ShowInfos,
			Action_SeasonEpisodeList
		};

		ActionType actionType;
		// TODO make a generic list for the values below
		QMap<QString,QVariant> id;
		QString parseMethodName;
		QList<Command*> commands; // all commands belonging to the action
		QFuture<void> future;
		QFutureWatcher<void> watcher;
	};

	static Cache *_instance;
	QList<SynchronizeAction*> currentActions;

	explicit Cache();

	SynchronizeAction *getAction(SynchronizeAction::ActionType actionType,
								 const QMap<QString,QVariant> &id) const;

	SynchronizeAction *getAction(Command *command) const;

	void launch_callback(Cache::SynchronizeAction *action, const QByteArray &response);

private slots:
	void commandFinished(const QByteArray &response);
	void futureFinished();

	// parsing methods
	void parseEpisode(const QString &showId, int season, const QJsonObject &root);
	void parseEpisodes(const QString &showId, int season, const QJsonObject &root);
	void parseSeasons(const QString &showId, const QByteArray &response);
	void parseShowInfos(const QString &showId, const QByteArray &response);
	void showInfosCallback(const QMap<QString,QVariant> &id, const QByteArray &response);
	void episodeListCallback(const QMap<QString,QVariant> &id, const QByteArray &response);
};

#endif // CACHE_H
