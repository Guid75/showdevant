//  Copyright 2013 Guillaume Denry (guillaume.denry@gmail.com)
//  This file is part of ShowDevant.
//
//  ShowDevant is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ShowDevant is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with ShowDevant.  If not, see <http://www.gnu.org/licenses/>.

#ifndef EPISODELISTMODEL_H
#define EPISODELISTMODEL_H

#include "cache.h"
#include "sqlquerymodel.h"

class EpisodeListModel : public SqlQueryModel
{
	Q_OBJECT

	Q_PROPERTY(QString show READ getShow() WRITE setShow(show) NOTIFY showChanged())
	Q_PROPERTY(int season READ getSeason() WRITE setSeason(season) NOTIFY seasonChanged())

public:
	explicit EpisodeListModel(QObject *parent = 0);
	
	const QString &getShow() const { return _show; }
	void setShow(const QString &show);

	int getSeason() const { return _season; }
	void setSeason(int season);

signals:
	void showChanged();
	void seasonChanged();

protected:
	void select();

private:
	QString _show;
	int _season;

	void load();
};

#endif // EPISODELISTMODEL_H
