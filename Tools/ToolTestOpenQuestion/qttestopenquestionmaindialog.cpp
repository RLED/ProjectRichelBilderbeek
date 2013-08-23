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
#ifdef _WIN32
//See http://www.richelbilderbeek.nl/CppCompileErrorSwprintfHasNotBeenDeclared.htm
#undef __STRICT_ANSI__
#endif

//#include own header file as first substantive line of code, from:
// * John Lakos. Large-Scale C++ Software Design. 1996. ISBN: 0-201-63362-0. Section 3.2, page 110
#include "qttestopenquestionmaindialog.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <QVBoxLayout>
#include <QKeyEvent>
#include <QLabel>

#include "openquestion.h"
#include "openquestiondialog.h"
#include "qtaboutdialog.h"
#include "qtopenquestiondialog.h"
#include "questiondialog.h"
#include "testopenquestionmaindialog.h"
#include "trace.h"
#include "ui_qttestopenquestionmaindialog.h"
#pragma GCC diagnostic pop

QtTestOpenQuestionMainDialog::QtTestOpenQuestionMainDialog(QWidget *parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtTestOpenQuestionMainDialog)
{
  #ifndef NDEBUG
  Test();
  #endif
  ui->setupUi(this);

  this->SetQuestion("-,1+1=,2");
}

QtTestOpenQuestionMainDialog::~QtTestOpenQuestionMainDialog()
{
  delete ui;
}

boost::shared_ptr<QtOpenQuestionDialog> QtTestOpenQuestionMainDialog::CreateQtOpenQuestionDialog(const std::string& s)
{
  boost::shared_ptr<QtOpenQuestionDialog> p;

  try
  {
    boost::shared_ptr<QuestionDialog> d(new OpenQuestionDialog(s));
    if (d) p.reset(new QtOpenQuestionDialog(d));
    assert(p);
    return p;
  }
  catch(std::exception&)
  {
    //No problem
  }
  return p;
}


void QtTestOpenQuestionMainDialog::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Escape) { close(); return; }
}

void QtTestOpenQuestionMainDialog::on_edit_question_textChanged(const QString &arg1)
{
  const std::string s = arg1.toStdString();
  m_dialog = CreateQtOpenQuestionDialog(s);

  if (ui->contents_here->layout())
  {
    delete ui->contents_here->layout();
  }
  assert(!ui->contents_here->layout());

  if (m_dialog)
  {
    assert(m_dialog);
    assert(!ui->contents_here->layout());
    QLayout * const my_layout = new QVBoxLayout;
    ui->contents_here->setLayout(my_layout);
    assert(ui->contents_here->layout());
    my_layout->addWidget(m_dialog.get());
  }
}

void QtTestOpenQuestionMainDialog::SetQuestion(const std::string& s)
{
  this->ui->edit_question->setText(s.c_str());
}

#ifndef NDEBUG
void QtTestOpenQuestionMainDialog::Test()
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting QtTestOpenQuestionMainDialog::Test");
  QtTestOpenQuestionMainDialog d;
  d.SetQuestion("-,1+1=,2");
  assert(d.GetDialog());
  d.SetQuestion("nonsense");
  assert(!d.GetDialog());
  d.SetQuestion("-,1+1=,2");
  assert(d.GetDialog());
  d.SetQuestion("more nonsense");
  assert(!d.GetDialog());
  TRACE("Finished QtTestOpenQuestionMainDialog::Test successfully");
}
#endif