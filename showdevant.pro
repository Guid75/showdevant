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
    downloadmanager.cpp \
    commandmanager.cpp \
    jsonparser.cpp \
    shortcut.cpp \
    databasemanager.cpp \
    sqltablemodel.cpp \
    showmodel.cpp \
    sqlquerymodel.cpp \
    sortfilterproxymodel.cpp \
    seasonlistmodel.cpp \
    command.cpp \
    download.cpp \
    cache.cpp \
    episodelistmodel.cpp \
    cachewatcher.cpp \
    settings.cpp \
    authenticator.cpp \
    myshowsmodel.cpp \
    cacheqmlproxy.cpp \
    episodemodel.cpp \
    subtitlelistmodel.cpp

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
    qml/showdevant/MessageBox.qml \
    TODO.txt \
    qml/showdevant/SeasonItem.qml \
    qml/showdevant/LoadingWidget.qml \
    qml/showdevant/commands.js \
    qml/showdevant/EpisodeItem.qml \
    qml/showdevant/RangeSelector.qml \
    qml/showdevant/EpisodeDetail.qml \
    qml/showdevant/EpisodesViewer.qml \
    qml/showdevant/SeasonsViewer.qml \
    qml/showdevant/EpisodeResume.qml \
    api_samples/shows_display.json \
    qml/showdevant/LoginBox.qml \
    qml/showdevant/ModalBox.qml \
    qml/showdevant/LoadingMask.qml \
    qml/showdevant/NotifyZone.qml \
    qml/showdevant/NotifyBox.qml \
    qml/showdevant/BubbleMenu.qml \
    qml/showdevant/MainStackViewDelegate.qml \
    qml/showdevant/ScrollText.qml \
    api_samples/members_info.json \
    api_samples/subtitles_show.json

HEADERS += \
    downloadmanager.h \
    commandmanager.h \
    jsonparser.h \
    shortcut.h \
    databasemanager.h \
    sqltablemodel.h \
    showmodel.h \
    sqlquerymodel.h \
    sortfilterproxymodel.h \
    seasonlistmodel.h \
    command.h \
    download.h \
    cache.h \
    episodelistmodel.h \
    cachewatcher.h \
    settings.h \
    authenticator.h \
    myshowsmodel.h \
    cacheqmlproxy.h \
    episodemodel.h \
    subtitlelistmodel.h
