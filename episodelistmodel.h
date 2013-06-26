#ifndef EPISODELISTMODEL_H
#define EPISODELISTMODEL_H

#include "cache.h"
#include "sqlquerymodel.h"

class EpisodeListModel : public SqlQueryModel
{
	Q_OBJECT

	Q_PROPERTY(QString show READ getShow() WRITE setShow(show) NOTIFY showChanged())
	Q_PROPERTY(int season READ getSeason() WRITE setSeason(season) NOTIFY seasonChanged())

public:
	explicit EpisodeListModel(QObject *parent = 0);
	
	const QString &getShow() const { return _show; }
	void setShow(const QString &show);

	int getSeason() const { return _season; }
	void setSeason(int season);

signals:
	void showChanged();
	void seasonChanged();

protected:
	void select();

private:
	QString _show;
	int _season;

	void load();
};

#endif // EPISODELISTMODEL_H
