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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QVariant>
#include <QList>
#include <QDateTime>

class Settings : public QObject
{
	Q_OBJECT
public:
	explicit Settings(QObject *parent = 0);

	Q_INVOKABLE void setValue(const QString &key, const QVariant & value);
	Q_INVOKABLE QVariant getValue(const QString &key, const QVariant & defaultValue = QVariant()) const;
	Q_INVOKABLE bool getBool(const QString &key, bool defaultValue = false) const;
	Q_INVOKABLE int getInt(const QString &key, int defaultValue = -1) const;

	Q_INVOKABLE void remove(const QString &key);
	Q_INVOKABLE void removeArray(const QString &key);
	Q_INVOKABLE void removeArrayEntry(const QString & key , int index);
	Q_INVOKABLE void appendToArray(const QString & key, QMap<QString, QVariant> values);
	Q_INVOKABLE QList<QVariantMap> getArray(const QString & key);
	Q_INVOKABLE QString getArrayJson(const QString & key);
	Q_INVOKABLE QString getArrayXml(const QString & key);
	Q_INVOKABLE bool checkValueArray(const QString & key, const QString & arrayKey , const QString & value);
	Q_INVOKABLE int getIndexOfValueInArray(const QString & key, const QString & arrayKey , const QString & value );

	// Time functions
	Q_INVOKABLE int getTimeZone();
	Q_INVOKABLE QDateTime stringToDate(QString s);
};

#endif
