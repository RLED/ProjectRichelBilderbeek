//---------------------------------------------------------------------------
/*
Maziak, a simple maze game
Copyright (C) 2007-2014 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/GameMaziak.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "qtmaziakgamewondialog.h"

#include <cassert>
#include <iostream>

#include <boost/timer.hpp>

#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

#include "ui_qtmaziakgamewondialog.h"

#pragma GCC diagnostic pop

ribi::QtMaziakGameWonDialog::QtMaziakGameWonDialog(QWidget *parent) :
  QtHideAndShowDialog(parent),
  ui(new Ui::QtMaziakGameWonDialog),
  m_timer(new QTimer),
  m_allow_close(false)
{
  ui->setupUi(this);

  m_timer->setInterval(2500);
  QObject::connect(m_timer.get(),SIGNAL(timeout()),this,SLOT(onTimer()));
  m_timer->start();
}

ribi::QtMaziakGameWonDialog::~QtMaziakGameWonDialog() noexcept
{
  delete ui;
}

void ribi::QtMaziakGameWonDialog::onTimer()
{
  m_allow_close = true;
  m_timer->stop();
}

void ribi::QtMaziakGameWonDialog::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  {
    //Top
    QPixmap pixmap(":/images/Congratulations.png");
    assert(!pixmap.isNull());
    painter.drawPixmap(ui->widget_top->geometry(),pixmap);
  }
  {
    //Center
    QPixmap pixmap(":/images/PlayerWon1.png");
    assert(!pixmap.isNull());
    painter.drawPixmap(ui->widget_center->geometry(),pixmap);
  }
  {
    //Bottom
    QPixmap pixmap(":/images/Congratulations.png");
    assert(!pixmap.isNull());
    painter.drawPixmap(ui->widget_bottom->geometry(),pixmap);
  }
}

void ribi::QtMaziakGameWonDialog::mousePressEvent(QMouseEvent*)
{
  if (m_allow_close) close();
}

void ribi::QtMaziakGameWonDialog::keyPressEvent(QKeyEvent*)
{
  if (m_allow_close) close();
}

