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
        GeosecureAisViewer.cpp 

HEADERS += \
	GeosecureAisViewer.h

