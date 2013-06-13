#include <QSqlQuery>

#include "myshowsmodel.h"

MyShowsModel::MyShowsModel(QObject *parent) :
	SqlQueryModel(parent)
{
	cacheWatcher.watchFor(Cache::Data_MemberInfos, QVariantMap());
	cacheWatcher.watchFor(Cache::Data_AddShow, QVariantMap());
	select();
}

void MyShowsModel::select()
{
	setQuery(QSqlQuery("SELECT * FROM myshows"));
}
