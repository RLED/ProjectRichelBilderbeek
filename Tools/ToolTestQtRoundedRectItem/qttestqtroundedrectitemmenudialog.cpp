//---------------------------------------------------------------------------
/*
TestQtRoundedRectItem, tool to test QtRoundedRectItem
Copyright (C) 2012-2014 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/ToolTestQtRoundedRectItem.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

#include "qttestqtroundedrectitemmenudialog.h"

#include <QDesktopWidget>
#include <QKeyEvent>

#include "qtkeyboardfriendlygraphicsview.h"
#include "qtroundedrectitem.h"
#include "testqtroundedrectitemmenudialog.h"
#include "qtaboutdialog.h"
#include "qttestqtroundedrectitemmaindialog.h"
#include "trace.h"
#include "ui_qttestqtroundedrectitemmenudialog.h"

#pragma GCC diagnostic pop

ribi::QtTestQtRoundedRectItemMenuDialog::QtTestQtRoundedRectItemMenuDialog(QWidget *parent) :
    QtHideAndShowDialog(parent),
    ui(new Ui::QtTestQtRoundedRectItemMenuDialog)
{
  #ifndef NDEBUG
  Test();
  #endif
  ui->setupUi(this);
}

ribi::QtTestQtRoundedRectItemMenuDialog::~QtTestQtRoundedRectItemMenuDialog() noexcept
{
  delete ui;
}

void ribi::QtTestQtRoundedRectItemMenuDialog::keyPressEvent(QKeyEvent * event)
{
  if (event->key() == Qt::Key_Escape) { close(); return; }
}

void ribi::QtTestQtRoundedRectItemMenuDialog::on_button_about_clicked()
{
  About a = TestQtRoundedRectItemMenuDialog().GetAbout();
  a.AddLibrary("QtHideAndShowDialog version: " + QtHideAndShowDialog::GetVersion());
  a.AddLibrary("QtKeyboardFriendlyGraphicsView version: " + QtKeyboardFriendlyGraphicsView::GetVersion());
  a.AddLibrary("QtRoundedRectItem version: " + QtRoundedRectItem::GetVersion());
  QtAboutDialog d(a);
  d.setWindowIcon(this->windowIcon());
  d.setStyleSheet(this->styleSheet());
  this->ShowChild(&d);
}

void ribi::QtTestQtRoundedRectItemMenuDialog::on_button_quit_clicked()
{
  close();
}

void ribi::QtTestQtRoundedRectItemMenuDialog::on_button_start_clicked()
{
  QtTestQtRoundedRectItemMainDialog d;
  ShowChild(&d);
}

#ifndef NDEBUG
void ribi::QtTestQtRoundedRectItemMenuDialog::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting ribi::QtTestQtRoundedRectItemMenuDialog::Test");
  TRACE("Finished ribi::QtTestQtRoundedRectItemMenuDialog::Test successfully");
}
#endif
