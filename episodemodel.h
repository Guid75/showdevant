#ifndef EPISODEMODEL_H
#define EPISODEMODEL_H

#include "sqlquerymodel.h"

class EpisodeModel : public SqlQueryModel
{
	Q_OBJECT

	Q_PROPERTY(QString show READ getShow() WRITE setShow(show))
	Q_PROPERTY(int season READ getSeason() WRITE setSeason(episode))

public:
	explicit EpisodeModel(QObject *parent = 0);
	
	const QString &getShow() const { return _show; }
	void setShow(const QString &show);

	int getSeason() const { return _season; }
	void setSeason(int season);

private:
	QString _show;
	int _season;

	void refreshQuery();
};

#endif // EPISODEMODEL_H
