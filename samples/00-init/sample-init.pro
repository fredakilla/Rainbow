QT -= core gui
TARGET = 00-init
TEMPLATE = app
CONFIG += c++11
CONFIG -= qt

include(../common/samples-common.pri)

SOURCES += src/sample.cpp

HEADERS += src/sample.h
