QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../../Classes/CppAbout/CppAbout.pri)
include(../../Classes/CppQtAboutDialog/CppQtAboutDialog.pri)
include(../../Classes/CppQtHideAndShowDialog/CppQtHideAndShowDialog.pri)
include(../../Games/GameBoenken/GameBoenkenDesktop.pri)

TEMPLATE = app

SOURCES += qtmain.cpp


#
#
# Type of compile
#
#

CONFIG(release, debug|release) {
  DEFINES += NDEBUG NTRACE_BILDERBIKKEL
}

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Weffc++

unix {
  QMAKE_CXXFLAGS += -Werror
}

#
#
# Boost
#
#
win32 {
  INCLUDEPATH += \
    ../../Libraries/boost_1_54_0
}