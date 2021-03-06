//---------------------------------------------------------------------------
/*
TestManyDigitNewick, tool to test the two-digit-Newick architecture
Copyright (C) 2010 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/ToolTestManyDigitNewick.htm
//---------------------------------------------------------------------------
#include <algorithm>
#include <string>
//---------------------------------------------------------------------------
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
//---------------------------------------------------------------------------
#include <QDesktopWidget>
//---------------------------------------------------------------------------
#include "BigInteger.hh"
//---------------------------------------------------------------------------
#include "about.h"
#include "binarynewickvector.h"
#include "newickvector.h"
#include "dialogtestmanydigitnewick.h"
#include "manydigitnewick.h"
#include "manydigitnewickindexer.h"
#include "newick.h"
#include "qtaboutdialog.h"
#include "twodigitnewick.h"
#include "ui_dialogtestmanydigitnewick.h"
//---------------------------------------------------------------------------
DialogTestManyDigitNewick::DialogTestManyDigitNewick(QWidget *parent) :
    QDialog(parent,Qt::Window),
    ui(new Ui::DialogTestManyDigitNewick)
{
  ui->setupUi(this);
  QObject::connect(ui->edit_newick,SIGNAL(textChanged(QString)),
    this,SLOT(OnAnyChange()));
  QObject::connect(ui->edit_theta,SIGNAL(textChanged(QString)),
    this,SLOT(OnAnyChange()));

  QObject::connect(ui->button_about,SIGNAL(clicked()),
    this,SLOT(OnAboutClick()));

  //Put the dialog in the screen center
  const QRect screen = QApplication::desktop()->screenGeometry();
  this->move( screen.center() - this->rect().center() );

  //Set the text of edit_theta to the value of 10.0,
  //handled by the locale
  ui->edit_theta->setText(
    QString(boost::lexical_cast<std::string>(10.0).c_str()));

  //Run some test cases
  #ifndef NDEBUG
  Newick::Test();
  BinaryNewickVector::Test();
  NewickVector::Test();
  TwoDigitNewick::Test();
  ManyDigitNewick::Test();

  const std::vector<std::string> v = Newick::CreateValidNewicks();
  const BigInteger max_complexity = 10000;
  BOOST_FOREACH(const std::string& s,v)
  {
    //Only test binary Newicks and relatively small Newicks
    if (Newick::CalcComplexity(Newick::StringToNewick(s)) > max_complexity)
    {
      continue;
    }
    //Start the tests
    const double theta = 1.0 + GetRandomUniform() * 10.0;
    #ifndef NTRACE_BILDERBIKKEL
    if (Newick::IsBinaryNewick(Newick::StringToNewick(s)))
    {
      const std::string t = std::string("Testing ")
        + s
        + std::string(" with complexity ")
        + ::bigIntegerToString(Newick::CalcComplexity(Newick::StringToNewick(s)))
        + std::string(" for three tests");
      TRACE(t);
      #endif
      const double p2 = TwoDigitNewick::CalculateProbability(s,theta);
      const double p5 = NewickVector::CalculateProbability(s,theta);
      //const double p6 = ManyDigitNewick::CalculateProbability(s,theta);
      if ( !Newick::fuzzy_equal_to()(p2,p5))
        //|| !Newick::fuzzy_equal_to()(p2,p6))
      {
        std::cerr
          << "ERROR: DIFFERENT PROBABILITIES FOUND"  << '\n'
          << "FOR UNARY/BINARY NEWICK " << s << '\n'
          << "Probability (TwoDigitNewick): "     << p2 << '\n'
          << "Probability (NewickVector): "       << p5 << '\n';
          //<< "Probability (ManyDigitNewick): "    << p6 << '\n';
      }
      assert(Newick::fuzzy_equal_to()(p2,p5));
      //assert(Newick::fuzzy_equal_to()(p2,p6));
    }
    else
    {
      //Trinary Newick
      TRACE("Testing trinary Newick");
      const double p5 = NewickVector::CalculateProbability(s,theta);
      const double p6 = ManyDigitNewick::CalculateProbability(s,theta);
      if ( !Newick::fuzzy_equal_to()(p5,p6))
      {
        std::cerr
          << "WARNING: DIFFERENT PROBABILITIES FOUND"  << '\n'
          << "FOR HIGHER-ARITY NEWICK " << s << '\n'
          << "Probability (NewickVector): "       << p5 << '\n'
          << "Probability (ManyDigitNewick): "    << p6 << '\n';
      }
    }
  }
  #endif

  #ifndef NDEBUG
  this->setWindowTitle(this->windowTitle() + QString(" (Debug)"));
  #else
  this->setWindowTitle(this->windowTitle() + QString(" (Release)"));
  assert(1==2 && "All asserts are turned off");
  #endif



  OnAnyChange();
}
//---------------------------------------------------------------------------
DialogTestManyDigitNewick::~DialogTestManyDigitNewick()
{
  delete ui;
}
//---------------------------------------------------------------------------
void DialogTestManyDigitNewick::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    ui->retranslateUi(this);
    break;
  default:
    break;
  }
}
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppGetRandomUniform.htm
double DialogTestManyDigitNewick::GetRandomUniform()
{
  return static_cast<double>(std::rand())/static_cast<double>(RAND_MAX);
}
//---------------------------------------------------------------------------
const std::string DialogTestManyDigitNewick::GetVersion()
{
  return "1.0";
}
//---------------------------------------------------------------------------
const std::vector<std::string> DialogTestManyDigitNewick::GetVersionHistory()
{
  std::vector<std::string> v;
  v.push_back("2011-03-02: version 1.0: initial version copied and from TestTwoDigitNewick");
  return v;
}
//---------------------------------------------------------------------------
void DialogTestManyDigitNewick::OnAnyChange()
{
  ui->button_calculate->setEnabled(false);
  ui->edit_text->clear();
  //Check Newicks
  const std::string s = ui->edit_newick->text().toStdString();
  if (!Newick::IsNewick(s))
  {
    ui->edit_text->appendPlainText("Not a valid Newick");
    return;
  }
  if (!Newick::IsBinaryNewick(Newick::StringToNewick(s)))
  {
    ui->edit_text->appendPlainText("Not a binary Newick");
  }
  ui->edit_text->appendPlainText("Newick: " + QString(s.c_str()));
  //Check theta
  try
  {
    boost::lexical_cast<double>(
      std::string(ui->edit_theta->text().toStdString()));
  }
  catch (boost::bad_lexical_cast&)
  {
    ui->edit_text->appendPlainText("Not a valid value for theta");
    return;
  }
  const double theta = boost::lexical_cast<double>(
    std::string(ui->edit_theta->text().toStdString()));
  ui->edit_text->appendPlainText("Theta: "
    + QString(boost::lexical_cast<std::string>(theta).c_str()));

  ui->button_calculate->setEnabled(true);

  ///\warning: theta must be set before NewickIndexer is created
  ManyDigitNewick::SetTheta(theta);
  const NewickVector n(s);
  const ManyDigitNewickIndexer i(n,theta);

  //View all indices in the index table
  /*
  {
    const std::vector<std::vector<int> >& index_table
      = i.GetIndexTable().Get();
    const int maxx = boost::numeric_cast<int>(index_table.size());
    for (int x=0; x!=maxx; ++x)
    {
      const int maxy = boost::numeric_cast<int>(index_table[x].size());
      for (int y=0; y!=maxy; ++y)
      {
        const int z = index_table[x][y];
        if (!z) continue;
        std::string s =
          "F = "
          + boost::lexical_cast<std::string>(z)
          + " = ("
          + boost::lexical_cast<std::string>(x)
          + ","
          + boost::lexical_cast<std::string>(y)
          + "): F' = { ";
        //Add all derivatives
        BOOST_FOREACH(
          const ManyDigitDerivative& di,
          i.GetNewick(z).GetDerivatives())
        {
          s = s
            + boost::lexical_cast<std::string>(di.m_derived_index)
            + " ("
            + boost::lexical_cast<std::string>(di.m_value_changed)
            + "->"
            + boost::lexical_cast<std::string>(di.m_value_changed-1)
            + "), ";
        }
        s[s.size()-2]=' ';
        s[s.size()-1]='}';
        ui->edit_text->appendPlainText(s.c_str());
      }
    }
  }
  */
  //Just display all Newicks
  {
    const int n_newicks = i.GetNewicks().Size();
    for (int ix=0; ix!=n_newicks; ++ix)
    {

      const ManyDigitNewick& n = i.GetNewicks().GetNewick(ix);
      std::string s
        = boost::lexical_cast<std::string>(ix)
        + " : ";
      s+=i.ConstructNewick(ix).ToStr() + " : ";
      if (n.IsProbabilityKnown())
      {
        s+=boost::lexical_cast<std::string>(n.GetProbability());
      }
      else
      {
        s+="[unknown probability]";
      }
      if (n.IsComplete())
      {
        s+=", ";
        s+=boost::lexical_cast<std::string>(n.GetSumTermsAboveZero());
        s+=", ";
        s+=boost::lexical_cast<std::string>(n.GetSumTermsAboveOne());
        s+=", ";
        s+=boost::lexical_cast<std::string>(n.GetDenominator());
      }
      else
      {
        s+=", [not complete]";
      }
      ui->edit_text->appendPlainText(QString(s.c_str()));
    }
  }

  //Display misc information
  {
    const std::string s
      = "Probability of Newick: "
      + boost::lexical_cast<std::string>(i.GetProbability());
    ui->edit_text->appendPlainText(QString(s.c_str()));
  }
  {
    const std::string s
      = "Number of reserved indices: "
      + boost::lexical_cast<std::string>(i.GetReserved());
    ui->edit_text->appendPlainText(QString(s.c_str()));
  }
  //Display IndexTable use
  /*
  {
    const int n_non_zero  = i.GetIndexTable().GetNumUsed();
    const int n_allocated = i.GetIndexTable().GetNumAllocated();
    const double f_used
      = (n_allocated == 0 ? 0.0 :
      boost::numeric_cast<double>(n_non_zero)
      / boost::numeric_cast<double>(n_allocated));
    ui->edit_text->appendPlainText(QString("Indices allocated: ")
    + QString(boost::lexical_cast<std::string>(n_allocated).c_str()) );
    ui->edit_text->appendPlainText(QString("Indices used: ")
    + QString(boost::lexical_cast<std::string>(n_non_zero).c_str()) );
    ui->edit_text->appendPlainText(QString("Fraction used: ")
    + QString(boost::lexical_cast<std::string>(f_used).c_str()) );

  }
  */
  //Check CalcNumOfCombinations and CalcNumOfSymmetries
  /*
  {
    const NewickVector n(s);
    const BigInteger n_s = n.CalcNumOfSymmetries();
    const BigInteger n_c = n.CalcNumOfCombinations();
    ui->edit_text->appendPlainText(
      QString("Number of symmetries: ")
      + QString(boost::lexical_cast<std::string>(n_s).c_str()));
    ui->edit_text->appendPlainText(
      QString("Number of combinations: ")
      + QString(bigIntegerToString(n_c).c_str()));
  }
  */
}
//---------------------------------------------------------------------------
void DialogTestManyDigitNewick::OnAboutClick()
{
  About about(
    "Richel Bilderbeek",
    "TestManyDigitNewick",
    "tool to test the two-digit-Newick architecture",
    "the 20th of February 2011",
    "2010-2011",
    "http://www.richelbilderbeek.nl/ToolTestManyDigitNewick",
    GetVersion(),
    GetVersionHistory());

  about.AddLibrary("BigInt: version 2010.04.30");
  about.AddLibrary("NewickVector: version " + NewickVector::GetVersion());
  about.AddLibrary("ManyDigitNewick: version " + ManyDigitNewick::GetVersion());

  QtAboutDialog d(about);
  d.exec();
}
//---------------------------------------------------------------------------

void DialogTestManyDigitNewick::on_button_calculate_clicked()
{
  ui->edit_text->clear();
  //Check Newicks
  const std::string s = ui->edit_newick->text().toStdString();
  ui->edit_text->appendPlainText("Newick: " + QString(s.c_str()));
  //Check theta
  const double theta = boost::lexical_cast<double>(
    std::string(ui->edit_theta->text().toStdString()));
  ui->edit_text->appendPlainText("Theta: "
    + QString(boost::lexical_cast<std::string>(theta).c_str()));

  ui->button_calculate->setEnabled(true);

  ///\warning: theta must be set before NewickIndexer is created
  ManyDigitNewick::SetTheta(theta);
  const NewickVector n(s);
  const ManyDigitNewickIndexer i(n,theta);

  const std::string probability_str
      = "Probability of Newick: "
      + boost::lexical_cast<std::string>(i.GetProbability());
    ui->edit_text->appendPlainText(QString(probability_str.c_str()));

}
