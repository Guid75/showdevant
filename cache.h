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

#ifndef CACHE_H
#define CACHE_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QVariantMap>
#include <QSignalMapper>

class Command;

class Cache : public QObject
{
	Q_OBJECT
public:
	enum CacheDataType {
		Data_ShowInfos,
		Data_Episodes,
		Data_MemberInfos,
		Data_AddShow,
		Data_RemoveShow
	};

	static Cache &instance();

	void start();

	Q_INVOKABLE int synchronizeShowInfos(const QString &showId);

	// if episode == -1, it means we want to synchronize ALL season episodes
	Q_INVOKABLE int synchronizeEpisodes(const QString &showId, int season, int episode = -1, bool fullInfo = false);

	Q_INVOKABLE int synchronizeMemberInfos();

	Q_INVOKABLE int addShow(const QString &showId, const QString &title);

	Q_INVOKABLE int removeShow(const QString &showId);

signals:
	void synchronizing(CacheDataType dataType, const QVariantMap &id);
	void synchronized(CacheDataType dataType, const QVariantMap &id);
	void synchronizeFailed(CacheDataType dataType, const QVariantMap &id);
	void showAdded(const QString &title);
	void showRemoved(const QString &title);

private:
	QThread *thread;

	struct SynchronizeAction {
		CacheDataType dataType;
		QVariantMap id;
		QString callbackMethodName;
		QList<Command*> commands; // all commands belonging to the action
	};

	static Cache *_instance;
	QList<SynchronizeAction*> currentActions;
	QSignalMapper commandMapper;

	explicit Cache();
	~Cache();

	SynchronizeAction *getAction(CacheDataType dataType,
								 const QVariantMap &id) const;

	SynchronizeAction *getAction(Command *command) const;

private slots:
	// thread callbacks
	void commandFinished(QObject *commandObj);

	// parsing
	void parseEpisodes(const QString &showId, int season, const QJsonObject &root, bool detailMode);
	void parseSeasons(const QString &showId, const QByteArray &response, bool allEpisodes);
	void parseShowInfos(const QString &showId, const QByteArray &response);
	void parseMemberInfos(const QByteArray &response);
	bool parseAddShow(const QString &showId, const QString &title, const QByteArray &response);
	bool parseRemoveShow(const QString &showId, const QByteArray &response);

	// action callbacks
	void showInfosCallback(const QVariantMap &id, const QByteArray &response);
	void episodesCallback(const QVariantMap &id, const QByteArray &response);
	void memberInfosCallback(const QVariantMap &id, const QByteArray &response);
	void addShowCallback(const QVariantMap &id, const QByteArray &response);
	void removeShowCallback(const QVariantMap &id, const QByteArray &response);
};

#endif // CACHE_H
