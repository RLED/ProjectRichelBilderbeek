QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app
SOURCES += main.cpp

#
#
# Type of compile
#
#

CONFIG(debug, debug|release) {
  message(Debug mode)
}

CONFIG(release, debug|release) {
  message(Release mode)

  #Remove all asserts and TRACE
  DEFINES += NDEBUG NTRACE_BILDERBIKKEL
}

#
#
# Platform specific
#
#

#
#
# Compiler flags
#
#
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra

unix {
  QMAKE_CXXFLAGS += -Werror
}

#
#
# Boost
#
#

unix {
  message(Unix: Boost already in include path)
}

win32 {
  message(Windows: add Boost to include path)
  INCLUDEPATH += \
    ../../../Projects/Libraries/boost_1_54_0
}

RESOURCES += \
    CppQtExample1.qrc
