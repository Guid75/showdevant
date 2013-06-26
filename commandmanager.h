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

#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QObject>
#include <QMap>
#include <QNetworkReply>
#include <QUrlQuery>

class QNetworkAccessManager;
class Command;

class CommandManager : public QObject
{
	Q_OBJECT
public:
	static CommandManager &instance();

	void setNetworkAccessManager(QNetworkAccessManager *nam);

	// after this function call, all future commands will be passed with this token as
	// an additional parameter which will be useful to get informations about the authenticated member.
	void recordAuthToken(const QString &token);

	// show commands
	Command *showsSearch(const QString &expression);
	Command *showsDisplay(const QString &showId);
	Command *showsEpisodes(const QString &showId, int season = -1, int episode = -1, bool summary = true, bool hide_notes = true);
	Command *showsAdd(const QString &showId);
	Command *showsRemove(const QString &showId);
	Command *showsArchive(const QString &showId);

	// subtitles commands
	Command *subtitlesShow(const QString &showId, int season = -1, int episode = -1, const QString &language = QString());
	Command *subtitlesShowByFile(const QString &showId, const QString &fileName, const QString &language = QString());

	// members commands
	Command *membersAuth(const QString &login, const QString &password);
	Command *membersInfos(const QString &login = "");
	Command *membersWatched(const QString &showId, int season, int episode, int note = -1);

private:
	static CommandManager *_instance;
	QNetworkAccessManager *nam;
	QMap<QNetworkReply*,Command*> commands;
	QString authToken;

	explicit CommandManager(QObject *parent = 0);

	Command *pushCommand(const QString &path, const QString &command, const QVariantMap &arguments = QVariantMap());
	Command *pushCommandAuth(const QString &path, const QString &command, const QVariantMap &arguments = QVariantMap());

	static QUrlQuery forgeQuery(const QString &path, const QString &postfix = "");

private slots:
	void httpFinished();
	void httpReadyRead();
	void httpError(QNetworkReply::NetworkError);
};

#endif // COMMANDMANAGER_H
