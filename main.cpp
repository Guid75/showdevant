#include <QtGui/QGuiApplication>
#include <QQmlContext>
#include <QtQml>
#include <QDebug>
#include <QNetworkAccessManager>

#include "downloadmanager.h"
#include "commandmanager.h"
#include "databasemanager.h"
#include "shortcut.h"
#include "sqltablemodel.h"
#include "sqlquerymodel.h"
#include "showmodel.h"
#include "seasonlistmodel.h"
#include "episodemodel.h"
#include "sortfilterproxymodel.h"
#include "showmanager.h"

#include "qtquick2applicationviewer.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	QNetworkAccessManager networkAccessManager;
	CommandManager::instance().setNetworkAccessManager(&networkAccessManager);
	DownloadManager::instance().setNetworkAccessManager(&networkAccessManager);

	QtQuick2ApplicationViewer viewer;
	qmlRegisterType<Shortcut>("com.guid75", 1, 0, "Shortcut");
	qmlRegisterType<SqlTableModel>("com.guid75", 1, 0, "SqlTableModel");
	qmlRegisterType<SqlQueryModel>("com.guid75", 1, 0, "SqlQueryModel");
	qmlRegisterType<SeasonListModel>("com.guid75", 1, 0, "SeasonListModel");
	qmlRegisterType<EpisodeModel>("com.guid75", 1, 0, "EpisodeModel");
	qmlRegisterType<ShowModel>("com.guid75", 1, 0, "ShowModel");
	qmlRegisterType<SortFilterProxyModel>("com.guid75", 1, 0, "SortFilterProxyModel");

	DatabaseManager::instance().openDB();

/*	ShowModel showModel;

	SortFilterProxyModel showProxyModel;
	showProxyModel.setSourceModel(&showModel);
	showProxyModel.sort(0, Qt::AscendingOrder);
	viewer.rootContext()->setContextProperty("showModel", &showProxyModel);*/

	viewer.rootContext()->setContextProperty("commandManager", &CommandManager::instance());
	viewer.rootContext()->setContextProperty("downloadManager", &DownloadManager::instance());
	viewer.rootContext()->setContextProperty("databaseManager", &DatabaseManager::instance());
	viewer.rootContext()->setContextProperty("showManager", &ShowManager::instance());
	viewer.setMainQmlFile(QStringLiteral("qml/showdevant/main.qml"));
	viewer.setTitle("ShowDevant - Alpha");
	viewer.showExpanded();

	return app.exec();
}
