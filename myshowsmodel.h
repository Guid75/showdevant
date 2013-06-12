#ifndef MYSHOWSMODEL_H
#define MYSHOWSMODEL_H

#include "sqlquerymodel.h"

class MyShowsModel : public SqlQueryModel
{
	Q_OBJECT
public:
	explicit MyShowsModel(QObject *parent = 0);
	
signals:
	
public slots:

protected:
	void select();
};

#endif // MYSHOWSMODEL_H
