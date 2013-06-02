#ifndef EPISODE_H
#define EPISODE_H

#include "cacheobject.h"

class Episode : public CacheObject
{
	Q_OBJECT

	Q_PROPERTY(QString showId READ showId() WRITE setShowId(showId))
	Q_PROPERTY(int season READ season() WRITE setSeason(season))
	Q_PROPERTY(int episode READ episode() WRITE setEpisode(episode))
public:
	explicit Episode(QObject *parent = 0);

	const QString &showId() const { return _showId; }
	void setShowId(const QString &showId);

	int season() const { return _season; }
	void setSeason(int season);

	int episode() const { return _episode; }
	void setEpisode(int episode);

protected:
	bool handleSynchronizing(Cache::CacheDataType dataType, const QMap<QString,QVariant> &id);
	bool handleSynchronized(Cache::CacheDataType dataType, const QMap<QString,QVariant> &id);
	
signals:

private:
	QString _showId;
	int _season;
	int _episode;

	void synchronizeData();
	bool checkSynchronizeEvent(Cache::CacheDataType dataType, const QMap<QString,QVariant> &id) const;
};

#endif // EPISODE_H
