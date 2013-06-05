#include <QNetworkAccessManager>

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

CommandManager::CommandManager(QObject *parent) :
	QObject(parent)
{
}

Command * CommandManager::pushCommand(const QString &url)
{
	Q_ASSERT(nam != NULL);

	QNetworkReply *reply = nam->get(QNetworkRequest(url));
	Command *command = new Command(this);
	commands.insert(reply, command);

	connect(reply, &QNetworkReply::finished, this, &CommandManager::httpFinished);
	connect(reply, &QNetworkReply::readyRead, this, &CommandManager::httpReadyRead);
	connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &CommandManager::httpError);

	return command;
}

Command *CommandManager::showsSearch(const QString &expression)
{
	return pushCommand(QString("%1/shows/search.json?title=%2&key=%3").arg(websiteUrl).arg(expression).arg(apiKey));
}

Command *CommandManager::showsDisplay(const QString &showId)
{
	QString str = QString("%1/shows/display/%2.json?key=%3").arg(websiteUrl).arg(showId).arg(apiKey);
	return pushCommand(str);
}

Command *CommandManager::showsEpisodes(const QString &showId, int season, int episode, bool summary, bool hide_notes)
{
	QString str = QString("%1/shows/episodes/%2.json?key=%3").arg(websiteUrl).arg(showId).arg(apiKey);

	if (season >= 0)
		str.append(QString("&season=%1").arg(season));
	if (episode >= 0)
		str.append(QString("&episode=%1").arg(episode));
	if (summary)
		str.append("&summary=1");
	if (hide_notes)
		str.append("hide_notes=1");

	return pushCommand(str);
}

Command *CommandManager::subtitlesShow(const QString &showId, int season, int episode, const QString &language)
{
	QString str = QString("%1/subtitles/show/%2.json?key=%3").arg(websiteUrl).arg(showId).arg(apiKey);
	if (season >= 0)
		str.append(QString("&season=%1").arg(season));
	if (episode >= 0)
		str.append(QString("&episode=%1").arg(episode));
	if (!language.isEmpty())
		str.append(QString("&language=%1").arg(language));

	return pushCommand(str);
}

Command *CommandManager::subtitlesShowByFile(const QString &showId, const QString &fileName, const QString &language)
{
	QString str = QString("%1/subtitles/show/%2.json?key=%3&file=%4").arg(websiteUrl).arg(showId).arg(apiKey).arg(fileName);
	if (!language.isEmpty())
		str.append(QString("&language=%1").arg(language));

	return pushCommand(str);
}

void CommandManager::httpError(QNetworkReply::NetworkError)
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	Command *command = commands[reply];
	command->_error = true;
	qCritical("CommandManager error: %s", qPrintable(reply->errorString()));
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

	command->emitFinished();
	commands.remove(reply);
//	delete command;
}
