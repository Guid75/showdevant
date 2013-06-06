#ifndef SEASONLISTMODEL_H
#define SEASONLISTMODEL_H

#include "sqlquerymodel.h"
#include "cache.h"

class SeasonListModel : public SqlQueryModel
{
	Q_OBJECT

	Q_PROPERTY(QString show READ getShow() WRITE setShow(show) NOTIFY showChanged())
public:
	explicit SeasonListModel(QObject *parent = 0);

	const QString &getShow() const { return _show; }
	void setShow(const QString &show);

signals:
	void showChanged();

protected:
	void select();

private:
	QString _show;
};

#endif // SEASONLISTMODEL_H
