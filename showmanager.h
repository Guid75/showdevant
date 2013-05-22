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

#ifndef SHOWMANAGER_H
#define SHOWMANAGER_H

#include <QObject>
#include <QHash>

class ShowManager : public QObject
{
	Q_OBJECT
public:
	static ShowManager &instance();

	/*! \brief If a show item is expired, reload it from the website
	 * \retval 0 if the show item is still fresh
	 * \retval 1 if the show is expired and a request ticket has been emitted
	 * \retval -1 if the request ticket could not be given by the request manager
	 */
	Q_INVOKABLE int refreshOnExpired(const QString &showid);

signals:
	void refreshDone(const QString &showId);

private:
	struct TicketData {
		TicketData() {}
		TicketData(const QString &_showId, const QString &_parseMethodName) :
			showId(_showId), parseMethodName(_parseMethodName) {}
		QString showId;
		QString parseMethodName;
	};

	static ShowManager *_instance;
	QHash<int,TicketData> parsing;

	explicit ShowManager();

private slots:
	void commandFinished(int ticketId, const QByteArray &response);

	// parsing methods
	void parseEpisode(const QString &showId, int season, const QJsonObject &root);
	void parseEpisodes(const QString &showId, int season, const QJsonObject &root);
	void parseSeasons(const QString &showId, const QByteArray &response);
};

#endif // SHOWMANAGER_H
