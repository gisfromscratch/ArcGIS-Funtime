#-------------------------------------------------
#
# Project created by QtCreator 2014-03-11T21:04:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

TARGET = GeosecureGeodatabase
TEMPLATE = lib

DEFINES += GEOSECUREGEODATABASE_LIBRARY

CONFIG += c++11 esri_runtime_qt_10_2

SOURCES += GeosecureGeodatabase.cpp \
    FileGdbWorkspaceFactory.cpp \
    FileGdbWorkspace.cpp \
    FileGdbTable.cpp

HEADERS += GeosecureGeodatabase.h\
        geosecuregeodatabase_global.h \
    FileGdbWorkspaceFactory.h \
    FileGdbWorkspace.h \
    FileGdbTable.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../FileGDBAPI/ -lFileGDBAPI
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../FileGDBAPI/ -lFileGDBAPID
else:unix: LIBS += -L$$PWD/../FileGDBAPI/ -lFileGDBAPI

INCLUDEPATH += $$PWD/../FileGDBAPI
DEPENDPATH += $$PWD/../FileGDBAPI
