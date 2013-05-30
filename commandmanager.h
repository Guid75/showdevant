#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QObject>
#include <QMap>
#include <QNetworkReply>

class QNetworkAccessManager;
class Command;

class CommandManager : public QObject
{
	Q_OBJECT
public:
	static CommandManager &instance();

	void setNetworkAccessManager(QNetworkAccessManager *nam);

	Command *showsSearch(const QString &expression);
	Command *showsDisplay(const QString &showId);
	Command *showsEpisodes(const QString &showId, int season = -1, int episode = -1, bool summary = true, bool hide_notes = true);
	Command *subtitlesShow(const QString &showId, int season = -1, int episode = -1, const QString &language = QString());
	Command *subtitlesShowByFile(const QString &showId, const QString &fileName, const QString &language = QString());

private:
	static CommandManager *_instance;
	QNetworkAccessManager *nam;
	QMap<QNetworkReply*,Command*> commands;

	explicit CommandManager(QObject *parent = 0);
	Command *pushCommand(const QString &url);

private slots:
	void httpFinished();
	void httpReadyRead();
	void httpError(QNetworkReply::NetworkError);
};

#endif // COMMANDMANAGER_H
