QT += core
QT -= gui

CONFIG += c++11

TARGET = TestSuite
CONFIG += c++11 arcgis_runtime_qml100_0_0
CONFIG += warn_on qmltestcase

TEMPLATE = app

SOURCES += main.cpp

DISTFILES += \
    tst_accessingBasemap.qml
