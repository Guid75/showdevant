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

class Command;

class Cache : public QObject
{
	Q_OBJECT
public:
	static Cache &instance();

	/*! \brief If a show item is expired, reload it from the website
	 * \retval 0 if the show item is still fresh
	 * \retval 1 if the show is expired and a request ticket has been emitted
	 * \retval -1 if the request ticket could not be given by the request manager
	 */
	Q_INVOKABLE int refreshOnExpired(const QString &showid, int season = -1, int episode = -1, bool description = false);

	/*! \brief Synchronize show informations
	 */
	Q_INVOKABLE int synchronizeShowInfos(const QString &showId);

	Q_INVOKABLE int synchronizeSeasonEpisodeList(const QString &showId, int season);

signals:
	void showInfosSynchronizing(const QString &showId);
	void showInfosSynchronized(const QString &showId);
	void refreshDone(const QString &showId);

private:
	struct TicketData {
		TicketData() {}
		TicketData(const QString &_showId, const QString &_parseMethodName) :
			showId(_showId), parseMethodName(_parseMethodName) {}
		QString showId;
		QString parseMethodName;
	};

	struct SynchronizeAction {
		enum ActionType {
			Action_ShowInfos,
			Action_SeasonEpisodeList
		};

		ActionType actionType;
		// TODO make a generic list for the values below
		QMap<QString,QVariant> id;
	};

	static Cache *_instance;
	QMap<Command*,TicketData> parsing;
	QList<SynchronizeAction*> currentActions;

	explicit Cache();

	SynchronizeAction *getAction(SynchronizeAction::ActionType actionType,
								 const QMap<QString,QVariant> &id);

private slots:
	void commandFinished(const QByteArray &response);

	// parsing methods
	void parseEpisode(const QString &showId, int season, const QJsonObject &root);
	void parseEpisodes(const QString &showId, int season, const QJsonObject &root);
	void parseSeasons(const QString &showId, const QByteArray &response);
	void parseShowInfos(const QString &showId, const QByteArray &response);
	void showInfosCallback(const QString &showId, const QByteArray &response);
};

#endif // CACHE_H
