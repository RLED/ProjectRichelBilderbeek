QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

INCLUDEPATH += \
    ../../Classes/CppAbout \
    ../../Classes/CppConnectThree \
    ../../Classes/CppConnectThreeWidget \
    ../../Classes/CppQtAboutDialog \
    ../../Classes/CppQtConnectThreeWidget \
    ../../Classes/CppQtHideAndShowDialog \
    ../../Classes/CppTrace

SOURCES += \
    ../../Classes/CppAbout/about.cpp \
    ../../Classes/CppConnectThree/connectthree.cpp \
    ../../Classes/CppConnectThreeWidget/connectthreewidget.cpp \
    ../../Classes/CppQtAboutDialog/qtaboutdialog.cpp \
    ../../Classes/CppQtConnectThreeWidget/qtconnectthreewidget.cpp \
    connectthreemenudialog.cpp \
    connectthreeresources.cpp \
    qtconnectthreegamedialog.cpp \
    qtconnectthreemenudialog.cpp \
    qtconnectthreeresources.cpp \
    qtmain.cpp \
    qtselectplayerwidget.cpp \
    qtshowwinnerdialog.cpp \
    ../../Classes/CppQtHideAndShowDialog/qthideandshowdialog.cpp

HEADERS  += \
    ../../Classes/CppAbout/about.h \
    ../../Classes/CppConnectThree/connectthree.h \
    ../../Classes/CppConnectThreeWidget/connectthreewidget.h \
    ../../Classes/CppQtAboutDialog/qtaboutdialog.h \
    ../../Classes/CppQtConnectThreeWidget/qtconnectthreewidget.h \
    ../../Classes/CppTrace/trace.h \
    connectthreemenudialog.h \
    connectthreeresources.h \
    qtconnectthreegamedialog.h \
    qtconnectthreemenudialog.h \
    qtconnectthreeresources.h \
    qtselectplayerwidget.h \
    qtshowwinnerdialog.h \
    ../../Classes/CppQtHideAndShowDialog/qthideandshowdialog.h

FORMS    += \
    ../../Classes/CppQtAboutDialog/qtaboutdialog.ui \
    qtconnectthreegamedialog.ui \
    qtconnectthreemenudialog.ui \
    qtshowwinnerdialog.ui

RESOURCES += \
    GameConnectThree.qrc


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

OTHER_FILES += \
    ../../Classes/CppQtHideAndShowDialog/Licence.txt
