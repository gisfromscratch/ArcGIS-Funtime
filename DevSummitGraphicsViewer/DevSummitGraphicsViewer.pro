#-------------------------------------------------
#
# Project created by QtCreator 2014-03-13T00:12:45
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DevSummitGraphicsViewer
TEMPLATE = app

CONFIG += c++11 esri_runtime_qt_10_2

SOURCES += main.cpp\
        MainWindow.cpp \
    GraphicsFileReader.cpp \
    TimeLayer.cpp \
    TimeFieldInfo.cpp \
    GraphicsCache.cpp

HEADERS  += MainWindow.h \
    GraphicsFileReader.h \
    TimeLayer.h \
    TimeFieldInfo.h \
    GraphicsCache.h

FORMS    += MainWindow.ui

RESOURCES += \
    Icons.qrc
