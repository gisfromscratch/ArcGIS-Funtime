#-------------------------------------------------
#
# Project created by QtCreator 2014-03-05T13:18:16
#
#-------------------------------------------------

QT       -= gui

TARGET = GeosecureAisDecoder
TEMPLATE = lib

DEFINES += GEOSECUREAISDECODER_LIBRARY

SOURCES += GeosecureAisDecoder.cpp \
    ais.cpp \
    ais1_2_3.cpp

HEADERS += GeosecureAisDecoder.h\
        geosecureaisdecoder_global.h \
    ais.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
