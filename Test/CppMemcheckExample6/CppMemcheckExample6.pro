#-------------------------------------------------
#
# Project created by QtCreator 2010-11-18T12:10:20
#
#-------------------------------------------------
QT       += core
QT       -= gui
TARGET = CppValgrindExample6
LIBS += -lwt -lwthttp
QMAKE_CXXFLAGS += -std=c++0x
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
SOURCES += main.cpp