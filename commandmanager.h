#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QObject>
#include <QMap>

class Command;

class CommandManager : public QObject
{
    Q_OBJECT
public:
    static CommandManager &instance();

	Q_INVOKABLE int showsSearch(const QString &expression);
	Q_INVOKABLE int showsEpisodes(const QString &url, int season = -1, int episode = -1, bool summary = true, bool hide_notes = true);
	Q_INVOKABLE int subtitlesShow(const QString &showId, int season = -1, int episode = -1, const QString &language = QString());
	Q_INVOKABLE int subtitlesShowByFile(const QString &showId, const QString &fileName, const QString &language = QString());

signals:
    // TODO returns a JSON document and parse error in the CommandManager
    void commandFinished(int ticketId, const QByteArray &response);

public slots:

private:
    static CommandManager *_instance;
    QMap<int,Command*> commands;

    explicit CommandManager(QObject *parent = 0);
    int pushCommand(const QString &url);

private slots:
    void requestReadyRead(int ticketId, const QByteArray &response);
    void requestFinished(int ticketId);
};

#endif // COMMANDMANAGER_H
