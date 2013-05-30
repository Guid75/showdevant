#ifndef SEASONLISTMODEL_H
#define SEASONLISTMODEL_H

#include "sqlquerymodel.h"

class SeasonListModel : public SqlQueryModel
{
	Q_OBJECT

	Q_PROPERTY(QString show READ getShow() WRITE setShow(show))
public:
	explicit SeasonListModel(QObject *parent = 0);

	const QString &getShow() const { return _show; }
	void setShow(const QString &show);

private:
	QString _show;

	void select();

private slots:
	void showInfosSynchronizing(const QString &showId);
	void showInfosSynchronized(const QString &showId);
};

#endif // SEASONLISTMODEL_H
