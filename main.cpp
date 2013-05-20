#include <QtGui/QGuiApplication>
#include <QQmlContext>
#include <QtQml>
#include <QDebug>

#include "requestmanager.h"
#include "downloadmanager.h"
#include "commandmanager.h"
#include "databasemanager.h"
#include "showmanager.h"
#include "shortcut.h"
#include "sqltablemodel.h"
#include "sqlquerymodel.h"
#include "showmodel.h"
#include "seasonmodel.h"

#include "qtquick2applicationviewer.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	QtQuick2ApplicationViewer viewer;
	qmlRegisterType<Shortcut>("com.guid75", 1, 0, "Shortcut");
	qmlRegisterType<SqlTableModel>("com.guid75", 1, 0, "SqlTableModel");
	qmlRegisterType<SqlQueryModel>("com.guid75", 1, 0, "SqlQueryModel");
	qmlRegisterType<ShowModel>("com.guid75", 1, 0, "ShowModel");
	qmlRegisterType<SeasonModel>("com.guid75", 1, 0, "SeasonModel");

	DatabaseManager::instance().openDB();

	viewer.rootContext()->setContextProperty("commandManager", &CommandManager::instance());
	viewer.rootContext()->setContextProperty("downloadManager", &DownloadManager::instance());
	viewer.rootContext()->setContextProperty("databaseManager", &DatabaseManager::instance());
	viewer.rootContext()->setContextProperty("showManager", &ShowManager::instance());
	viewer.setMainQmlFile(QStringLiteral("qml/showdevant/main.qml"));
	viewer.setTitle("ShowDevant - Alpha");
	viewer.showExpanded();

	return app.exec();
}
