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

	/*! \brief If a show item is expired, reload it from the website
	 * \retval 0 if the show item is still fresh
	 * \retval 1 if the show is expired and a request ticket has been emitted
	 * \retval -1 if the request ticket could not be given by the request manager
	 */
//	Q_INVOKABLE int refreshOnExpired(const QString &showid, int season = -1, int episode = -1, bool description = false);

	Q_INVOKABLE int synchronizeShowInfos(const QString &showId);

	Q_INVOKABLE int synchronizeSeasonEpisodeList(const QString &showId, int season);

signals:
	void synchronizing(CacheDataType dataType, QMap<QString,QVariant> id);
	void synchronized(CacheDataType dataType, QMap<QString,QVariant> id);
/*	void showInfosSynchronizing(const QString &showId);
	void showInfosSynchronized(const QString &showId);

	void episodeListSynchronizing(const QString &showId, int season);
	void episodeListSynchronized(const QString &showId, int season);*/

//	void refreshDone(const QString &showId);

private:
/*	struct TicketData {
		TicketData() {}
		TicketData(const QString &_showId, const QString &_parseMethodName) :
			showId(_showId), parseMethodName(_parseMethodName) {}
		QString showId;
		QString parseMethodName;
	};*/
	typedef QMap<QString,QVariant> ActionId;
	struct SynchronizeAction {
		enum ActionType {
			Action_ShowInfos,
			Action_SeasonEpisodeList
		};

		ActionType actionType;
		// TODO make a generic list for the values below
		ActionId id;
		QString parseMethodName;
		QList<Command*> commands; // all commands belonging to the action
	};

	static Cache *_instance;
	QList<SynchronizeAction*> currentActions;

	explicit Cache();

	SynchronizeAction *getAction(SynchronizeAction::ActionType actionType,
								 const ActionId &id) const;

	SynchronizeAction *getAction(Command *command) const;

private slots:
	void commandFinished(const QByteArray &response);

	// parsing methods
	void parseEpisode(const QString &showId, int season, const QJsonObject &root);
	void parseEpisodes(const QString &showId, int season, const QJsonObject &root);
	void parseSeasons(const QString &showId, const QByteArray &response);
	void parseShowInfos(const QString &showId, const QByteArray &response);
	void showInfosCallback(const ActionId &id, const QByteArray &response);
	void episodeListCallback(const ActionId &id, const QByteArray &response);
};

#endif // CACHE_H
