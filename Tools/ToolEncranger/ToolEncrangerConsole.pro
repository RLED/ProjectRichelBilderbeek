QT += core
QT -= gui
TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle

include(../../Tools/ToolEncranger/ToolEncrangerConsole.pri)
include(../../Classes/CppAbout/CppAbout.pri)
include(../../Classes/CppEncranger/CppEncranger.pri)
include(../../Classes/CppHelp/CppHelp.pri)
include(../../Classes/CppLoopReader/CppLoopReader.pri)
include(../../Classes/CppMenuDialog/CppMenuDialog.pri)
include(../../Classes/CppRichelBilderbeekProgram/CppRichelBilderbeekProgram.pri)
include(../../Classes/CppTrace/CppTrace.pri)

SOURCES += main.cpp

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
