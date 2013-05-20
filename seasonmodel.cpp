#include <QSqlQuery>

#include "seasonmodel.h"

SeasonModel::SeasonModel(QObject *parent) :
    SqlQueryModel(parent)
{
	setShow("a");
}

void SeasonModel::setShow(const QString &show)
{
	if (show == _show)
		return;

	_show = show;

	QSqlQuery query;
	query.prepare("select *, (select count(show_id) from episode where show_id=:show_id and season=season.number) as episode_count from season where show_id=:show_id");
	query.bindValue(":show_id", show);
	query.exec();
	setQuery(query);
}
