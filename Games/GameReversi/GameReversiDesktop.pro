include(../../DesktopApplication.pri)

include(../../Libraries/Boost.pri)

include(../../Classes/CppAbout/CppAbout.pri)
include(../../Classes/CppAsciiArter/CppAsciiArter.pri)
include(../../Classes/CppFileIo/CppFileIo.pri)
include(../../Classes/CppHelp/CppHelp.pri)
include(../../Classes/CppMenuDialog/CppMenuDialog.pri)
include(../../Classes/CppRichelBilderbeekProgram/CppRichelBilderbeekProgram.pri)
include(../../Classes/CppTrace/CppTrace.pri)

include(../../Classes/CppQtAboutDialog/CppQtAboutDialog.pri)
include(../../Classes/CppQtHideAndShowDialog/CppQtHideAndShowDialog.pri)

include(GameReversiDesktop.pri)

#Won't link: no main
#SOURCES += qtmain.cpp