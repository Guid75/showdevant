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

#ifndef SHORTCUT_H
#define SHORTCUT_H

#include <QObject>
#include <QKeySequence>
#include <QVariant>

class Shortcut : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QVariant key READ key WRITE setKey NOTIFY keyChanged)
public:
	explicit Shortcut(QObject *parent = 0);

	void setKey(QVariant key);
	QVariant key() { return m_keySequence; }

	bool eventFilter(QObject *obj, QEvent *e);

signals:
	void keyChanged();
	void activated();
	void pressedAndHold();

public slots:

private:
	QKeySequence m_keySequence;
	bool m_keypressAlreadySend;
};

#endif // SHORTCUT_H
