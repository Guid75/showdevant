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
    showsearchmodel.cpp \
    jsonparser.cpp \
    shortcut.cpp \
    databasemanager.cpp

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qml/showdevant/TopToolbar.qml \
    README.md \
    qml/showdevant/SearchResultList.qml \
    qml/showdevant/ShowList.qml

HEADERS += \
    requestmanager.h \
    downloadmanager.h \
    commandmanager.h \
    showsearchmodel.h \
    jsonparser.h \
    shortcut.h \
    databasemanager.h
