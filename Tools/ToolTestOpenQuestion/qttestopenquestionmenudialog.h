//---------------------------------------------------------------------------
/*
TestOpenQuestion, tool to test the OpenQuestion and OpenQuestionDialog classes
Copyright (C) 2013 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/ToolTestOpenQuestion.htm
//---------------------------------------------------------------------------
#ifndef QTTESTOPENQUESTIONMENUDIALOG_H
#define QTTESTOPENQUESTIONMENUDIALOG_H

#include <boost/scoped_ptr.hpp>

#include "qthideandshowdialog.h"

namespace Ui {
class QtTestOpenQuestionMenuDialog;
}
struct TestOpenQuestionMenuDialog;

class QtTestOpenQuestionMenuDialog : public QtHideAndShowDialog
{
  Q_OBJECT

public:
  explicit QtTestOpenQuestionMenuDialog(QWidget *parent = 0);
  ~QtTestOpenQuestionMenuDialog();

protected:
  void keyPressEvent(QKeyEvent* event);

private slots:
  void on_button_start_clicked();
  void on_button_about_clicked();
  void on_button_quit_clicked();

private:
  Ui::QtTestOpenQuestionMenuDialog *ui;
  boost::scoped_ptr<TestOpenQuestionMenuDialog> m_dialog;

  #ifndef NDEBUG
  static void Test();
  #endif
};

#endif // QTTESTOPENQUESTIONMENUDIALOG_H