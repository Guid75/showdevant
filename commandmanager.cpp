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

#include <QNetworkAccessManager>
#include <QCryptographicHash>
#include <QDebug>

#include "command.h"

#include "commandmanager.h"

static const QString apiKey = "9adb4ab628c6";
static const QString websiteUrl = "http://api.betaseries.com";

CommandManager *CommandManager::_instance = 0;

CommandManager &CommandManager::instance()
{
	if (!_instance)
		_instance = new CommandManager;

	return *_instance;
}

void CommandManager::setNetworkAccessManager(QNetworkAccessManager *nam)
{
	if (this->nam == nam)
		return;

	this->nam = nam;

	// TODO : remove all current QNetworkReply
}

void CommandManager::recordAuthToken(const QString &token)
{
	authToken = token;
}

CommandManager::CommandManager(QObject *parent) :
	QObject(parent)
{
}

Command *CommandManager::pushCommand(const QString &path, const QString &cmd, const QVariantMap &arguments)
{
	Q_ASSERT(nam != NULL);

	QString normPath(path);

	// ensure the path is normalized (with starting and ending slashes)
	if (normPath.length() > 0) {
		if (!normPath.startsWith('/'))
			normPath.prepend('/');
		if (!normPath.endsWith('/'))
			normPath.append('/');
	}

	QUrlQuery query;

	foreach (const QString &key, arguments.keys()) {
		query.addQueryItem(key, arguments[key].toString());
	}

	// add some application and authentication keys
	query.addQueryItem("key", apiKey);
	if (!authToken.isEmpty())
		query.addQueryItem("token", authToken);

	QUrl url(QString("%1%2%3.json").arg(websiteUrl).arg(normPath).arg(cmd));
	url.setQuery(query);

	QNetworkRequest request(url);
	request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);

	QNetworkReply *reply = nam->get(request);
	Command *command = new Command(this);
	commands.insert(reply, command);

	connect(reply, &QNetworkReply::finished, this, &CommandManager::httpFinished);
	connect(reply, &QNetworkReply::readyRead, this, &CommandManager::httpReadyRead);
	connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &CommandManager::httpError);

	return command;
}

Command *CommandManager::pushCommandAuth(const QString &path, const QString &command, const QVariantMap &arguments)
{
	Q_ASSERT(!authToken.isEmpty());

	return pushCommand(path, command, arguments);
}

QUrlQuery CommandManager::forgeQuery(const QString &path, const QString &postfix)
{
	Q_ASSERT(path.length() > 0 && path[0] == '/');

	QString postfixsep = "";

	if (!postfix.isEmpty() && path[path.length() - 1] != '/')
		postfixsep = "/";

	return QUrlQuery(QString("%1%2%3%4").arg(websiteUrl).arg(path).arg(postfixsep).arg(postfix.isEmpty() ? "" : postfix + ".json"));
}

Command *CommandManager::showsSearch(const QString &expression)
{
	QVariantMap args;
	args.insert("title", expression);
	return pushCommand("shows", "search", args);
}

Command *CommandManager::showsDisplay(const QString &showId)
{
	return pushCommand("shows/display", showId);
}

Command *CommandManager::showsEpisodes(const QString &showId, int season, int episode, bool summary, bool hide_notes)
{
	QVariantMap args;

	QUrlQuery query = forgeQuery("/shows/episodes/", showId);

	if (season >= 0)
		args.insert("season", season);
	if (episode >= 0)
		args.insert("episode", episode);
	if (summary)
		args.insert("summary", true);
	if (hide_notes)
		args.insert("hide_notes", true);

	return pushCommand("shows/episodes", showId, args);
}

Command *CommandManager::showsAdd(const QString &showId)
{
	return pushCommandAuth("shows/add", showId);
}

Command *CommandManager::showsRemove(const QString &showId)
{
	return pushCommandAuth("shows/remove", showId);
}

Command *CommandManager::showsArchive(const QString &showId)
{
	return pushCommandAuth("shows/archive", showId);
}

Command *CommandManager::subtitlesShow(const QString &showId, int season, int episode, const QString &language)
{
	QVariantMap args;

	if (season >= 0)
		args.insert("season", season);
	if (episode >= 0)
		args.insert("episode", episode);
	if (!language.isEmpty())
		args.insert("language", language);

	return pushCommand("subtitles/show", showId, args);
}

Command *CommandManager::subtitlesShowByFile(const QString &showId, const QString &fileName, const QString &language)
{
	QVariantMap args;

	args.insert("file", fileName);

	if (!language.isEmpty())
		args.insert("language", language);

	return pushCommand("subtitles/show", showId, args);
}

Command *CommandManager::membersAuth(const QString &login, const QString &password)
{
	QVariantMap args;
	args.insert("login", login);
	args.insert("password", password);
	return pushCommand("members", "auth", args);
}

Command *CommandManager::membersInfos(const QString &login)
{
	if (login.isEmpty())
		return pushCommandAuth("members", "infos");
	return pushCommandAuth("members/infos", login);
}

Command *CommandManager::membersWatched(const QString &showId, int season, int episode, int note)
{
	QVariantMap args;
	args.insert("season", season);
	args.insert("episode", episode);
	if (note >= 1)
		args.insert("note", note);
	return pushCommandAuth("members/watched", showId, args);
}

void CommandManager::httpError(QNetworkReply::NetworkError)
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	Command *command = commands[reply];
	command->_httpError = true;
}

void CommandManager::httpReadyRead()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	Command *command = commands[reply];

	Q_ASSERT(command != NULL);

	command->_response.append(reply->readAll());
}

void CommandManager::httpFinished()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	Command *command = commands[reply];

	Q_ASSERT(command != NULL);

	command->finalize();
	commands.remove(reply);
}
