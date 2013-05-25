# Add more folders to ship with the application, here
folder_01.source = qml/showdevant
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

QT += sql

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    requestmanager.cpp \
    downloadmanager.cpp \
    commandmanager.cpp \
    jsonparser.cpp \
    shortcut.cpp \
    databasemanager.cpp \
    sqltablemodel.cpp \
    showmodel.cpp \
    sqlquerymodel.cpp \
    seasonmodel.cpp \
    sortfilterproxymodel.cpp \
    showmanager.cpp \
    episodemodel.cpp

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qml/showdevant/TopToolbar.qml \
    README.md \
    qml/showdevant/SearchResultList.qml \
    qml/showdevant/ShowList.qml \
    qml/showdevant/notifications.js \
    qml/showdevant/NotificationBox.qml \
    qml/showdevant/MessageBox.qml \
    TODO.txt \
    qml/showdevant/SeasonItem.qml \
    qml/showdevant/LoadingWidget.qml \
    qml/showdevant/commands.js \
    qml/showdevant/SeasonsFlickable.qml \
    qml/showdevant/EpisodesFlickable.qml \
    qml/showdevant/EpisodeItem.qml \
    qml/showdevant/SeasonSelector.qml

HEADERS += \
    requestmanager.h \
    downloadmanager.h \
    commandmanager.h \
    jsonparser.h \
    shortcut.h \
    databasemanager.h \
    sqltablemodel.h \
    showmodel.h \
    sqlquerymodel.h \
    seasonmodel.h \
    sortfilterproxymodel.h \
    showmanager.h \
    episodemodel.h
