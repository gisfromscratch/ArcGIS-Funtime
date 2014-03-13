#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T22:41:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

TARGET = DevSummitGeometryEngineConsole
CONFIG += console
CONFIG -= app_bundle

CONFIG += c++11 esri_runtime_qt_10_2

TEMPLATE = app


SOURCES += main.cpp \
    ProjectionEngine.cpp \
    EsriProjectionEngine.cpp \
    SimpleProjectionEngine.cpp

HEADERS += \
    ProjectionEngine.h \
    EsriProjectionEngine.h \
    SimpleProjectionEngine.h
