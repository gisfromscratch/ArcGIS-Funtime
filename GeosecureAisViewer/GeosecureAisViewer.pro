#-------------------------------------------------
#  Copyright 2013 ESRI
#
#  All rights reserved under the copyright laws of the United States
#  and applicable international laws, treaties, and conventions.
#
#  You may freely redistribute and use this sample code, with or
#  without modification, provided you include the original copyright
#  notice and use restrictions.
#
#  See the Sample code usage restrictions document for further information.
#-------------------------------------------------

TARGET = GeosecureAisViewer 
TEMPLATE = app

QT += core gui opengl xml network

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

CONFIG += c++11 esri_runtime_qt_10_2 

win32:CONFIG += \
  embed_manifest_exe


SOURCES += \
	main.cpp \
        GeosecureAisViewer.cpp \ 
    AisGraphicFactory.cpp

HEADERS += \
	GeosecureAisViewer.h \
    AisGraphicFactory.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Release/release/ -lGeosecureAisDecoder -lGeosecureCommons -lGeosecureGeodatabase -L$$PWD/../FileGDBAPI/ -lFileGDBAPI
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Debug/debug/ -lGeosecureAisDecoder -lGeosecureCommons -lGeosecureGeodatabase -L$$PWD/../FileGDBAPI/ -lFileGDBAPID
else:unix: LIBS += -L$$PWD/../Debug/ -lGeosecureAisDecoder -lGeosecureCommons -lGeosecureGeodatabase -L$$PWD/../FileGDBAPI/ -lFileGDBAPI

INCLUDEPATH += $$PWD/../GeosecureAisDecoder $$PWD/../GeosecureCommons $$PWD/../GeosecureGeodatabase $$PWD/../FileGDBAPI
DEPENDPATH += $$PWD/../GeosecureAisDecoder $$PWD/../GeosecureCommons $$PWD/../GeosecureGeodatabase $$PWD/../FileGDBAPI

RESOURCES += \
    Icons.qrc
