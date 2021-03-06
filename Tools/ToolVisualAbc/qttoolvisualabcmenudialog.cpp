#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "about.h"
#include "qtaboutdialog.h"
#include "qttoolvisualabcmaindialog.h"
#include "qttoolvisualabcmenudialog.h"
#include "trace.h"
#include "ui_qttoolvisualabcmenudialog.h"
#include "toolvisualabcmenudialog.h"
#pragma GCC diagnostic pop

ribi::QtVisualAbcMenuDialog::QtVisualAbcMenuDialog(QWidget *parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtVisualAbcMenuDialog)
{
  #ifndef NDEBUG
  Test();
  #endif
  ui->setupUi(this);
}

ribi::QtVisualAbcMenuDialog::~QtVisualAbcMenuDialog() noexcept
{
  delete ui;
}

void ribi::QtVisualAbcMenuDialog::on_button_start_clicked()
{
  QtVisualAbcMainDialog d;
  this->ShowChild(&d);
}

void ribi::QtVisualAbcMenuDialog::on_button_about_clicked()
{
  About a = VisualAbcMenuDialog().GetAbout();
  QtAboutDialog d(a);
  this->ShowChild(&d);
}

void ribi::QtVisualAbcMenuDialog::on_button_quit_clicked()
{
  close();
}

#ifndef NDEBUG
void ribi::QtVisualAbcMenuDialog::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting ribi::QtVisualAbcMenuDialog::Test");
  TRACE("Finished ribi::QtVisualAbcMenuDialog::Test successfully");
}
#endif
