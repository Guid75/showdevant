#include <QtGui/QGuiApplication>
#include <QQmlContext>

#include "requestmanager.h"
#include "downloadmanager.h"
#include "commandmanager.h"
#include "showsearchmodel.h"
#include "shortcut.h"

#include "qtquick2applicationviewer.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	QtQuick2ApplicationViewer viewer;
	qmlRegisterType<ShowSearchModel>("com.guid75", 1, 0, "ShowSearchModel");
	qmlRegisterType<Shortcut>("com.guid75", 1, 0, "Shortcut");

//	viewer.rootContext()->setContextProperty("requestManager", &RequestManager::instance());
	viewer.rootContext()->setContextProperty("commandManager", &CommandManager::instance());
	viewer.rootContext()->setContextProperty("downloadManager", &DownloadManager::instance());
	viewer.setMainQmlFile(QStringLiteral("qml/showdevant/main.qml"));
	viewer.setTitle("ShowDevant - Alpha");
	viewer.showExpanded();

	return app.exec();
}
