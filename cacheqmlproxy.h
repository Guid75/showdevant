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

#ifndef CACHEQMLPROXY_H
#define CACHEQMLPROXY_H

#include <QObject>

class CacheQmlProxy : public QObject
{
	Q_OBJECT
public:
	explicit CacheQmlProxy(QObject *parent = 0);

signals:
	void showAdded(const QString &title);
	void showRemoved(const QString &title);

public slots:
	
};

#endif // CACHEQMLPROXY_H
