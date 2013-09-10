TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += \
    ../../Classes/CppApproximator \
    ../../Classes/CppCanvas \
    ../../Classes/CppTrace

SOURCES += main.cpp \
    ../../Classes/CppApproximator/approximator.cpp \
    ../../Classes/CppApproximator/exceptionnoextrapolation.cpp \
    ../../Classes/CppCanvas/canvas.cpp

HEADERS += \
    ../../Classes/CppApproximator/approximator.h \
    ../../Classes/CppApproximator/exceptionnoextrapolation.h \
    ../../Classes/CppTrace/trace.h \
    ../../Classes/CppCanvas/canvas.h

OTHER_FILES += \
    ../../Classes/CppApproximator/Licence.txt \
    ../../Classes/CppCanvas/Licence.txt
    ../../Classes/CppTrace/Licence.txt

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
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Weffc++ -Werror

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
    ../../Libraries/boost_1_54_0
}

