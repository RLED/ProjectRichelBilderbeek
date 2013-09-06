//---------------------------------------------------------------------------
/*
TicTacToe, tic-tac-toe game
Copyright (C) 2010-2011 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/GameTicTacToe.htm
//---------------------------------------------------------------------------
//#include own header file as first substantive line of code, from:
// * John Lakos. Large-Scale C++ Software Design. 1996. ISBN: 0-201-63362-0. Section 3.2, page 110
#include "qttictactoegamedialog.h"

#include <cassert>

#include <QDesktopWidget>

#include "tictactoe.h"
#include "qttictactoewidget.h"
#include "qttictactoewinnerdialog.h"
#include "ui_qttictactoegamedialog.h"

ribi::QtTicTacToeGameDialog::QtTicTacToeGameDialog(QWidget *parent) :
    QDialog(parent, Qt::Window),
    ui(new Ui::QtTicTacToeGameDialog),
    m_tictactoe(new QtTicTacToeWidget)
{
  ui->setupUi(this);
  ui->layout->addWidget(m_tictactoe.get());
  QObject::connect(m_tictactoe.get(),SIGNAL(hasWinner()),
    this,SLOT(HasWinner()));

  //Put the dialog in the screen center
  const QRect screen = QApplication::desktop()->screenGeometry();
  this->setGeometry(0,0,3 * screen.width() / 4,3 * screen.height() / 4);
  this->move( screen.center() - this->rect().center() );
}

ribi::QtTicTacToeGameDialog::~QtTicTacToeGameDialog()
{
  delete ui;
}

void ribi::QtTicTacToeGameDialog::HasWinner()
{

  QtTicTacToeWinnerDialog d;
  switch (m_tictactoe->GetTicTacToe()->GetWinner())
  {
    case TicTacToe::player1: d.SetWinnerCross(); break;
    case TicTacToe::player2: d.SetWinnerCircle(); break;
    case TicTacToe::draw: d.SetDraw(); break;
    default: assert(!"Should not get here");
  }
  d.exec();
  m_tictactoe->Restart();
}

