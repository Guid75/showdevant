#ifndef EPISODELISTMODEL_H
#define EPISODELISTMODEL_H

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

	void refreshQuery();
};

#endif // EPISODELISTMODEL_H
