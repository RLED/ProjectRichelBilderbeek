QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app

INCLUDEPATH += \
    ../../Classes/CppAbout \
    ../../Classes/CppQtAboutDialog \
    ../../Classes/CppQtHideAndShowDialog


SOURCES += \
    ../../Classes/CppAbout/about.cpp \
    ../../Classes/CppQtAboutDialog/qtaboutdialog.cpp \
    qtmain.cpp \
    qtmaziakmenudialog.cpp \
    qtmaziakmaindialog.cpp \
    qtmaziakinstructionsdialog.cpp \
    maziakmenudialog.cpp \
    qtmaziakgameoverdialog.cpp \
    qtmaziakgamewondialog.cpp \
    ../../Classes/CppQtHideAndShowDialog/qthideandshowdialog.cpp

HEADERS  += \
    ../../Classes/CppAbout/about.h \
    ../../Classes/CppQtAboutDialog/qtaboutdialog.h \
    qtmaziakmenudialog.h \
    qtmaziakmaindialog.h \
    qtmaziakinstructionsdialog.h \
    maziakmenudialog.h \
    qtmaziakgameoverdialog.h \
    qtmaziakgamewondialog.h \
    ../../Classes/CppQtHideAndShowDialog/qthideandshowdialog.h

FORMS    += \
    ../../Classes/CppQtAboutDialog/qtaboutdialog.ui \
    qtmaziakmenudialog.ui \
    qtmaziakmaindialog.ui \
    qtmaziakinstructionsdialog.ui \
    qtmaziakgameoverdialog.ui \
    qtmaziakgamewondialog.ui

RESOURCES += \
    gamemaziak.qrc

OTHER_FILES += \
    ../../Classes/CppAbout/Licence.txt \
    ../../Classes/CppQtAboutDialog/Licence.txt \
    ../../Classes/CppQtHideAndShowDialog/Licence.txt

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
