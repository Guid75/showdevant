#ifndef SEASONMODEL_H
#define SEASONMODEL_H

#include "sqlquerymodel.h"

class SeasonModel : public SqlQueryModel
{
	Q_OBJECT

	Q_PROPERTY(QString show READ getShow() WRITE setShow(show))
public:
	explicit SeasonModel(QObject *parent = 0);

	const QString &getShow() const { return _show; }
	void setShow(const QString &show);
	
signals:
	
public slots:

private:
	QString _show;
	
};

#endif // SEASONMODEL_H
