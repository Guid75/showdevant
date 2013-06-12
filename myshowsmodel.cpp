#include <QSqlQuery>

#include "myshowsmodel.h"

MyShowsModel::MyShowsModel(QObject *parent) :
	SqlQueryModel(parent)
{
	QVariantMap id;
	cacheWatcher.watchFor(Cache::Data_MemberInfos, id);
	select();
}

void MyShowsModel::select()
{
	setQuery(QSqlQuery("SELECT * FROM myshows"));
}
