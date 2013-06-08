#include <QSqlQuery>

#include "cache.h"

#include "seasonlistmodel.h"

SeasonListModel::SeasonListModel(QObject *parent) :
	SqlQueryModel(parent)
{
}

void SeasonListModel::setShow(const QString &show)
{
	if (show == _show)
		return;

	_show = show;

	cacheWatcher.clearWatching();
	QVariantMap id;
	id.insert("showId", _show);
	cacheWatcher.watchFor(Cache::Data_ShowInfos, id);

	Cache::instance().synchronizeShowInfos(_show);
	select();

	emit showChanged();
}

void SeasonListModel::select()
{
	QSqlQuery query;
	query.prepare("SELECT * FROM season WHERE show_id=:show_id");
	query.bindValue(":show_id", _show);
	query.exec();
	setQuery(query);
}
