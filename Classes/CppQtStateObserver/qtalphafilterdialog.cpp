#ifdef _WIN32
//See http://www.richelbilderbeek.nl/CppCompileErrorSwprintfHasNotBeenDeclared.htm
#undef __STRICT_ANSI__
#endif

//#include own header file as first substantive line of code, from:
// * John Lakos. Large-Scale C++ Software Design. 1996. ISBN: 0-201-63362-0. Section 3.2, page 110
#include "qtalphafilterdialog.h"

#include "ui_qtalphafilterdialog.h"

QtAlphaFilterDialog::QtAlphaFilterDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::QtAlphaFilterDialog)
{
  ui->setupUi(this);
}

QtAlphaFilterDialog::~QtAlphaFilterDialog()
{
  delete ui;
}
