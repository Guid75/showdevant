#include <QFile>
#include <QUrl>
#include <QDir>
#include <QNetworkAccessManager>

#include "download.h"
#include "downloadmanager.h"

DownloadManager *DownloadManager::_instance = 0;

DownloadManager &DownloadManager::instance()
{
	if (!_instance)
		_instance = new DownloadManager;

	return *_instance;
}

void DownloadManager::setNetworkAccessManager(QNetworkAccessManager *nam)
{
	if (this->nam == nam)
		return;

	this->nam = nam;

	// TODO : remove all current QNetworkReply
}

DownloadManager::DownloadManager(QObject *parent) :
	QObject(parent)
{
}

void DownloadManager::httpFinished()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	Download *download = downloads[reply];

	Q_ASSERT(download != NULL);

	download->file->close();
	download->emitFinished();
	downloads.remove(reply);
	delete download;
}

void DownloadManager::httpReadyRead()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	Download *download = downloads[reply];

	Q_ASSERT(download != NULL);

	if (!download->file->isOpen()) {
		return;
	}
	download->file->write(reply->readAll());
}

void DownloadManager::httpError(QNetworkReply::NetworkError)
{
	// TODO manage errors
}

Download *DownloadManager::download(const QString &file, const QString &url, const QString &dirPath)
{
	Q_ASSERT(nam != NULL);

	QNetworkReply *reply = nam->get(QNetworkRequest(url));
	Download *download = new Download;
	QString filePath = QDir(dirPath).filePath(file);
	download->file = new QFile(filePath);
	download->_filePath = filePath;
	if (!download->file->open(QIODevice::WriteOnly)) {
		qCritical("Error while opening %s", qPrintable(filePath));
		// TODO manage the file opening error
		delete download;
		return NULL;
	}
	downloads.insert(reply, download);
	connect(reply, &QNetworkReply::finished, this, &DownloadManager::httpFinished);
	connect(reply, &QNetworkReply::readyRead, this, &DownloadManager::httpReadyRead);
	connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &DownloadManager::httpError);

	return download;
}
