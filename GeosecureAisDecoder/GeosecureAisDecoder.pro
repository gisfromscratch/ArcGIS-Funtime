#-------------------------------------------------
#
# Project created by QtCreator 2014-03-05T13:18:16
#
#-------------------------------------------------

QT       += gui

TARGET = GeosecureAisDecoder
TEMPLATE = lib

DEFINES += GEOSECUREAISDECODER_LIBRARY

CONFIG += c++11 esri_runtime_qt_10_2

SOURCES += GeosecureAisDecoder.cpp \
    ais.cpp \
    ais1_2_3.cpp \
    AisMessage.cpp \
    AisReader.cpp

HEADERS += GeosecureAisDecoder.h\
        geosecureaisdecoder_global.h \
    ais.h \
    AisMessage.h \
    AisReader.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
