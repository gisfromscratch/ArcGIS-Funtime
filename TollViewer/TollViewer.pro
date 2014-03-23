#-------------------------------------------------
#
# Project created by QtCreator 2014-03-23T14:35:27
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TollViewer
TEMPLATE = app

CONFIG += c++11 esri_runtime_qt_10_2

SOURCES += main.cpp\
        MainWindow.cpp \
    ShapefileWorkspaceFactory.cpp

HEADERS  += MainWindow.h \
    ShapefileWorkspaceFactory.h

FORMS    += MainWindow.ui

RESOURCES += \
    Icons.qrc
