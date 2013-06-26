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

#ifndef SHOWMODEL_H
#define SHOWMODEL_H

#include <QAbstractListModel>
#include <QSqlRecord>

#include "sqltablemodel.h"

class ShowModel : public SqlTableModel
{
	Q_OBJECT

public:
	explicit ShowModel(QObject *parent = 0);

	// custom
	Q_INVOKABLE bool addShow(const QString &showId, const QString &title);
};

#endif // SHOWMODEL_H
