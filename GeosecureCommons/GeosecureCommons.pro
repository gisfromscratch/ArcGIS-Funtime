#-------------------------------------------------
#
# Project created by QtCreator 2014-03-06T18:00:05
#
#-------------------------------------------------

QT       += gui

TARGET = GeosecureCommons
TEMPLATE = lib

DEFINES += GEOSECURECOMMONS_LIBRARY

CONFIG += c++11 esri_runtime_qt_10_2

SOURCES += GeosecureCommons.cpp \
    ProjectionEngine.cpp \
    EsriProjectionEngine.cpp \
    SimpleProjectionEngine.cpp \
    GraphicSerializer.cpp \
    TimeLayer.cpp

HEADERS += GeosecureCommons.h\
        geosecurecommons_global.h \
    ProjectionEngine.h \
    EsriProjectionEngine.h \
    SimpleProjectionEngine.h \
    GraphicSerializer.h \
    TimeLayer.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
