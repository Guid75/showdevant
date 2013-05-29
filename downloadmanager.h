#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QMap>
#include <QNetworkReply>

class Download;
class QNetworkAccessManager;

class DownloadManager : public QObject
{
	Q_OBJECT
public:
	static DownloadManager &instance();

	void setNetworkAccessManager(QNetworkAccessManager *nam);

	/*! \brief Schedule a file downloading
	 */
	Download *download(const QString &fileName, const QString &url, const QString &dirPath);

private:
	static DownloadManager *_instance;
	QNetworkAccessManager *nam;
	QMap<QNetworkReply*,Download*> downloads;

	explicit DownloadManager(QObject *parent = 0);

private slots:
	void httpFinished();
	void httpReadyRead();
	void httpError(QNetworkReply::NetworkError);
};

#endif // DOWNLOADMANAGER_H
