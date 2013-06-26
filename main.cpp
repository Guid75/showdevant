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

#include <QtGui/QGuiApplication>
#include <QQmlContext>
#include <QtQml>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QThread>

#include "settings.h"
#include "downloadmanager.h"
#include "commandmanager.h"
#include "databasemanager.h"
#include "shortcut.h"
#include "sqltablemodel.h"
#include "showmodel.h"
#include "seasonlistmodel.h"
#include "episodelistmodel.h"
#include "episodemodel.h"
#include "sortfilterproxymodel.h"
#include "cache.h"
#include "cachewatcher.h"
#include "cacheqmlproxy.h"
#include "authenticator.h"
#include "myshowsmodel.h"
#include "jsonparser.h"

#include "qtquick2applicationviewer.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	// for invokeMethod
	//qRegisterMetaType<JsonParser>("JsonParser");

	Cache::instance().start();

	QNetworkAccessManager networkAccessManager;
	CommandManager::instance().setNetworkAccessManager(&networkAccessManager);
	DownloadManager::instance().setNetworkAccessManager(&networkAccessManager);

	QtQuick2ApplicationViewer viewer;
	qmlRegisterType<Shortcut>("com.guid75", 1, 0, "Shortcut");
	qmlRegisterType<SqlTableModel>("com.guid75", 1, 0, "SqlTableModel");
	qmlRegisterType<SqlQueryModel>("com.guid75", 1, 0, "SqlQueryModel");
	qmlRegisterType<SeasonListModel>("com.guid75", 1, 0, "SeasonListModel");
	qmlRegisterType<EpisodeListModel>("com.guid75", 1, 0, "EpisodeListModel");
	qmlRegisterType<EpisodeModel>("com.guid75", 1, 0, "EpisodeModel");
	qmlRegisterType<ShowModel>("com.guid75", 1, 0, "ShowModel");
	qmlRegisterType<SortFilterProxyModel>("com.guid75", 1, 0, "SortFilterProxyModel");
	qmlRegisterType<MyShowsModel>("com.guid75", 1, 0, "MyShowsModel");
	qmlRegisterType<CacheWatcher>("com.guid75", 1, 0, "CacheWatcher");
	// we use qmlRegisterUncreatableType because we only want to access enums from this class and not allow the instanciation
	qmlRegisterUncreatableType<Authenticator>("com.guid75", 1, 0, "Authenticator", "This class cannot be created in QML");

	DatabaseManager::instance().openDB();

	Settings settings;
	viewer.rootContext()->setContextProperty("settings", &settings);

	Authenticator authenticator;
	viewer.rootContext()->setContextProperty("authenticator", &authenticator);

	CacheQmlProxy cacheQmlProxy;
	viewer.rootContext()->setContextProperty("cacheQmlProxy", &cacheQmlProxy);
	viewer.rootContext()->setContextProperty("commandManager", &CommandManager::instance());
	viewer.rootContext()->setContextProperty("downloadManager", &DownloadManager::instance());
	viewer.rootContext()->setContextProperty("databaseManager", &DatabaseManager::instance());
	viewer.rootContext()->setContextProperty("cache", &Cache::instance());
	viewer.setMainQmlFile(QStringLiteral("qml/showdevant/main.qml"));
	viewer.setTitle("ShowDevant - Alpha");
	viewer.showExpanded();

	return app.exec();
}
