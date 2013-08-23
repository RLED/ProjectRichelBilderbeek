QT += core
QT -= gui
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Werror

INCLUDEPATH += \
    ../../Classes/CppAbout \
    ../../Classes/CppCanvas \
    ../../Classes/CppTrace

SOURCES += \
    ../../Classes/CppAbout/about.cpp \
    ../../Classes/CppCanvas/canvas.cpp \
    main.cpp

HEADERS += \
    ../../Classes/CppAbout/about.h \
    ../../Classes/CppCanvas/canvas.h

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
  message(Unix)
  #Strict error handling
  QMAKE_CXXFLAGS += -Werror
}

#
#
# Boost
#
#

unix {
  message(Unix: Boost already in INCLUDEPATH)
  #Don't link to Boost when crosscompiling is desired
}

win32 {
  message(Windows: add Boost to INCLUDEPATH)

  INCLUDEPATH += \
    ../../Libraries/boost_1_54_0

  #Don't link to Boost when crosscompiling is desired
}