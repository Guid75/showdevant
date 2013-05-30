#include <QSqlQuery>

#include "cache.h"

#include "seasonlistmodel.h"

SeasonListModel::SeasonListModel(QObject *parent) :
	SqlQueryModel(parent)
{
	// workaround to get the sql fields populated
	_show = "a";
	select();

	connect(&Cache::instance(), &Cache::synchronizing,
			this, &SeasonListModel::synchronizing);

	connect(&Cache::instance(), &Cache::synchronized,
			this, &SeasonListModel::synchronized);
}

void SeasonListModel::setShow(const QString &show)
{
	if (show == _show)
		return;

	_show = show;

	if (Cache::instance().synchronizeShowInfos(_show) == 0) {
		select();
		setSynchronized(true);
		setSynchronizing(false);
		return;
	}

	clear();
	select();
	setSynchronized(false);
	setSynchronizing(true);
}

void SeasonListModel::select()
{
	QSqlQuery query;
	query.prepare("SELECT * FROM season WHERE show_id=:show_id");
	query.bindValue(":show_id", _show);
	query.exec();
	setQuery(query);
}

void SeasonListModel::synchronizing(Cache::CacheDataType dataType, const QMap<QString,QVariant> &id)
{
	if (dataType != Cache::Data_ShowInfos)
		return;

	if (id["showId"].toString() != _show)
		return;

	setSynchronizing(true);
	setSynchronized(false);

}

void SeasonListModel::synchronized(Cache::CacheDataType dataType, const QMap<QString,QVariant> &id)
{
	if (dataType != Cache::Data_ShowInfos)
		return;

	if (id["showId"].toString() != _show)
		return;

	setSynchronizing(false);
	setSynchronized(true);

	select();
}
