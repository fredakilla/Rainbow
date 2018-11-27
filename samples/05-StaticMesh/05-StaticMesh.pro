QT -= core gui
TARGET = 05-StaticMesh
TEMPLATE = app
CONFIG += c++14
CONFIG -= qt

DESTDIR = ../../build/bin
QMAKE_CLEAN += $$DESTDIR/$$TARGET

include(../common/samples-common.pri)

SOURCES += src/sample.cpp

