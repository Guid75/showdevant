#ifndef SUBTITLELISTMODEL_H
#define SUBTITLELISTMODEL_H

#include "sqlquerymodel.h"

class SubtitleListModel : public SqlQueryModel
{
	Q_OBJECT
	
	Q_PROPERTY(QString show READ getShow() WRITE setShow(show) NOTIFY showChanged())
	Q_PROPERTY(int season READ getSeason() WRITE setSeason(season) NOTIFY seasonChanged())
	Q_PROPERTY(int episode READ getEpisode() WRITE setEpisode(episode) NOTIFY episodeChanged())

public:
	explicit SubtitleListModel(QObject *parent = 0);

	const QString &getShow() const { return _show; }
	void setShow(const QString &show);

	int getSeason() const { return _season; }
	void setSeason(int season);

	int getEpisode() const { return _episode; }
	void setEpisode(int episode);

signals:
	void showChanged();
	void seasonChanged();
	void episodeChanged();

protected:
	void select();

private:
	QString _show;
	int _season;
	int _episode;

	void load();
};

#endif // SUBTITLELISTMODEL_H
