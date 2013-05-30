#ifndef EPISODELISTMODEL_H
#define EPISODELISTMODEL_H

#include "cache.h"
#include "sqlquerymodel.h"

class EpisodeListModel : public SqlQueryModel
{
	Q_OBJECT

	Q_PROPERTY(QString show READ getShow() WRITE setShow(show))
	Q_PROPERTY(int season READ getSeason() WRITE setSeason(episode))

public:
	explicit EpisodeListModel(QObject *parent = 0);
	
	const QString &getShow() const { return _show; }
	void setShow(const QString &show);

	int getSeason() const { return _season; }
	void setSeason(int season);

private:
	QString _show;
	int _season;

	void select();
	void load();

private slots:
	void synchronizing(Cache::CacheDataType dataType, const QMap<QString,QVariant> &id);
	void synchronized(Cache::CacheDataType dataType, const QMap<QString,QVariant> &id);
};

#endif // EPISODELISTMODEL_H
