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

#ifndef EPISODEMODEL_H
#define EPISODEMODEL_H

#include "sqlquerymodel.h"

class EpisodeModel : public SqlQueryModel
{
	Q_OBJECT

	Q_PROPERTY(QString show READ getShow() WRITE setShow(show) NOTIFY showChanged())
	Q_PROPERTY(int season READ getSeason() WRITE setSeason(season) NOTIFY seasonChanged())
	Q_PROPERTY(int episode READ getEpisode() WRITE setEpisode(episode) NOTIFY episodeChanged())

public:
	explicit EpisodeModel(QObject *parent = 0);

	const QString &getShow() const { return _show; }
	void setShow(const QString &show);

	int getSeason() const { return _season; }
	void setSeason(int season);

	int getEpisode() const { return _episode; }
	void setEpisode(int episode);

signals:
	void showChanged();
	void seasonChanged();
	void episodeChanged();

protected:
	void select();

private:
	QString _show;
	int _season;
	int _episode;

	void load();
};

#endif // EPISODEMODEL_H
