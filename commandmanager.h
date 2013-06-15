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

	Q_INVOKABLE void recordAuthToken(const QString &token);

	// show commands
	Command *showsSearch(const QString &expression);
	Command *showsDisplay(const QString &showId);
	Command *showsEpisodes(const QString &showId, int season = -1, int episode = -1, bool summary = true, bool hide_notes = true);
	Command *showsAdd(const QString &showId);
	Command *showsRemove(const QString &showId);

	// subtitles commands
	Command *subtitlesShow(const QString &showId, int season = -1, int episode = -1, const QString &language = QString());
	Command *subtitlesShowByFile(const QString &showId, const QString &fileName, const QString &language = QString());

	// members commands
	Command *membersAuth(const QString &login, const QString &password);
	Command *membersInfos(const QString &login = "");

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
