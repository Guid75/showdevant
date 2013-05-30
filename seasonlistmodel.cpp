#include <QSqlQuery>

#include "cache.h"

#include "seasonlistmodel.h"

SeasonListModel::SeasonListModel(QObject *parent) :
	SqlQueryModel(parent)
{
	// workaround to get the sql fields populated
	_show = "a";
	select();

	connect(&Cache::instance(), &Cache::showInfosSynchronizing,
			this, &SeasonListModel::showInfosSynchronizing);

	connect(&Cache::instance(), &Cache::showInfosSynchronized,
			this, &SeasonListModel::showInfosSynchronized);
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

void SeasonListModel::showInfosSynchronizing(const QString &showId)
{
	if (showId != _show)
		return;

	setSynchronizing(true);
	setSynchronized(false);
}

void SeasonListModel::showInfosSynchronized(const QString &showId)
{
	if (showId != _show)
		return;

	setSynchronizing(false);
	setSynchronized(true);

	select();
}
