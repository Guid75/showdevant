#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>

class QFile;

class Download : public QObject
{
	friend class DownloadManager;
	Q_OBJECT
public:
	explicit Download(QObject *parent = 0);

	const QString &filePath() const { return _filePath; }
	
signals:
	void finished();

private:
	QString _filePath;
	QFile *file;

	void emitFinished();
};

#endif // DOWNLOAD_H
