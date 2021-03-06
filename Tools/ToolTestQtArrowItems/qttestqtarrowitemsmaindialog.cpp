// ---------------------------------------------------------------------------
/*
TestQtArrowItems, tool to test Qt arrow QGraphicsItems
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
// ---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/ToolTestQtArrowItems.htm
// ---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "qttestqtarrowitemsmaindialog.h"

#include <cassert>
#include <cmath>

#include <boost/lexical_cast.hpp>
#include <boost/math/constants/constants.hpp>

#include <QKeyEvent>

#include "qtaboutdialog.h"
#include "qtarrowitem.h"
#include "qtpatharrowitem.h"
#include "qtquadbezierarrowitem.h"
#include "qtlabeledquadbezierarrowitem.h"
#include "qtroundedrectitem.h"
#include "qtroundedtextrectitem.h"
#include "trace.h"
#include "ui_qttestqtarrowitemsmaindialog.h"
#pragma GCC diagnostic pop

ribi::QtTestQtArrowItemsMainDialog::QtTestQtArrowItemsMainDialog(QWidget *parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtTestQtArrowItemsMainDialog)
{
  #ifndef NDEBUG
  Test();
  #endif
  ui->setupUi(this);

  assert(this->ui->view->scene());

  const double pi = boost::math::constants::pi<double>();

  //Add normal arrows
  {
    const int n_items = 12;
    const double ray = 100;
    for (int i=0; i<n_items; i+=2) //1 arrow per 2 items
    {
      const double angle1 = 2.0 * pi * (static_cast<double>(i) / static_cast<double>(n_items));
      const double x1 =  std::sin(angle1) * ray;
      const double y1 = -std::cos(angle1) * ray;
      const bool tail = (std::rand() >> 4) % 2;
      const double angle2 = 2.0 * pi * (static_cast<double>(i + 1) / static_cast<double>(n_items));
      const double x2 =  std::sin(angle2) * ray;
      const double y2 = -std::cos(angle2) * ray;
      const bool head = (std::rand() >> 4) % 2;
      QtArrowItem * const item = new QtArrowItem(x1,y1,tail,x2,y2,head);
      item->SetFocusPen(QPen(QColor(255,0,0),2));
      assert(!item->scene());
      this->ui->view->scene()->addItem(item);
    }
  }
  //Add path arrows
  {
    const int n_items = 16;
    const double ray = 150;
    for (int i=0; i<n_items; i+=2) //1 arrow per 2 items
    {
      const double angle1 = 2.0 * pi * (static_cast<double>(i) / static_cast<double>(n_items));
      const QPointF tail_pos(std::sin(angle1) * ray,-std::cos(angle1) * ray);
      const bool tail = (std::rand() >> 4) % 2;
      const double angle2 = 2.0 * pi * (static_cast<double>(i+1) / static_cast<double>(n_items));
      const QPointF head_pos(std::sin(angle2) * ray,-std::cos(angle2) * ray);
      const bool head = (std::rand() >> 4) % 2;


      const int n_mid = 1 + ((std::rand() >> 4) % 4);
      std::vector<QPointF> mid;
      for (int i=0; i!=n_mid; ++i)
      {
        const double f = static_cast<double>(i + 1) / static_cast<double>(n_mid + 1);
        const double dx = (f * (head_pos.x() - tail_pos.x())) - 10.0 + (20.0 * GetRandomUniform());
        const double dy = (f * (head_pos.y() - tail_pos.y())) - 10.0 + (20.0 * GetRandomUniform());
        QPointF p(tail_pos.x() + dx, tail_pos.y() + dy);
        mid.push_back(p);
      }


      QtPathArrowItem * const item = new QtPathArrowItem(tail_pos,tail,mid,head,head_pos);
      item->SetFocusPen(QPen(QColor(255,0,0),2));
      assert(!item->scene());
      this->ui->view->scene()->addItem(item);
    }
  }
  //Add QtQuadBezierArrowItem
  {
    const int n_items = 18;
    const double ray = 200;
    std::vector<QtRoundedRectItem *> rects;

    for (int i=0; i!=n_items; ++i)
    {
      const double angle = 2.0 * pi * (static_cast<double>(i) / static_cast<double>(n_items));
      const double x1 =  std::sin(angle) * ray;
      const double y1 = -std::cos(angle) * ray;
      QtRoundedRectItem * rect = nullptr;
      if ((i%3)!=1)
      {
        rect = new QtRoundedRectItem;
        rect->SetRoundedRect(QRectF(-4.0,-4.0,4.0,4.0),0.0,0.0);
      }
      else
      {
        rect = new QtRoundedTextRectItem(boost::lexical_cast<std::string>(i));
      }
      rect->SetFocusPen(QPen(QColor(255,0,0),2));
      rect->setPos(x1,y1);
      assert(!rect->scene());
      this->ui->view->scene()->addItem(rect);
      rects.push_back(rect);

      rect->m_signal_request_scene_update.connect(
        boost::bind(&ribi::QtTestQtArrowItemsMainDialog::OnRequestSceneUpdate,this));
    }
    for (int i=0; i<n_items-2; i+=3)
    {
      assert(i + 2 < n_items);
      QtQuadBezierArrowItem * const item = new QtQuadBezierArrowItem(
        rects[(i+0) % n_items],
        false,
        rects[(i+1) % n_items],
        true,
        rects[(i+2) % n_items]);
      assert(!item->scene());
      this->ui->view->scene()->addItem(item);
    }
  }
  //Add QtQuadBezierArrowItem, no mid
  {
    const int n_items = 18;
    const double ray = 250;
    std::vector<QtRoundedRectItem *> rects;

    for (int i=0; i!=n_items; ++i)
    {
      const double angle = 2.0 * pi * (static_cast<double>(i) / static_cast<double>(n_items));
      const double x1 =  std::sin(angle) * ray;
      const double y1 = -std::cos(angle) * ray;
      QtRoundedRectItem * rect = nullptr;
      //if ((i%3)!=1)
      {
        rect = new QtRoundedRectItem;
        rect->SetRoundedRect(QRectF(-4.0,-4.0,4.0,4.0),0.0,0.0);
      }
      //else
      {
        rect = new QtRoundedTextRectItem(boost::lexical_cast<std::string>(i));
      }
      rect->SetFocusPen(QPen(QColor(255,0,0),2));
      rect->setPos(x1,y1);
      assert(!rect->scene());
      this->ui->view->scene()->addItem(rect);
      rects.push_back(rect);

      rect->m_signal_request_scene_update.connect(
        boost::bind(&ribi::QtTestQtArrowItemsMainDialog::OnRequestSceneUpdate,this));
    }
    for (int i=0; i<n_items-1; i+=2)
    {
      assert(i + 1 < n_items);
      QtQuadBezierArrowItem * const item = new QtQuadBezierArrowItem(
        rects[(i+0) % n_items],
        false,
        nullptr, //Straight line
        true,
        rects[(i+1) % n_items]);
      assert(!item->scene());
      this->ui->view->scene()->addItem(item);
    }
  }
  //Add labeled quad arrows
  {
    const double ray = 300;
    const int n_items = 18;
    std::vector<QtRoundedRectItem *> rects;
    for (int i=0; i!=n_items; ++i)
    {
      const double angle = 2.0 * pi * (static_cast<double>(i) / static_cast<double>(n_items));
      const double x1 =  std::sin(angle) * ray;
      const double y1 = -std::cos(angle) * ray;
      QtRoundedRectItem * rect = new QtRoundedRectItem;
      assert(rect);
      rect->SetRoundedRect(QRectF(-4.0,-4.0,4.0,4.0),0.0,0.0);
      rect->SetFocusPen(QPen(QColor(255,0,0),2));
      rect->setPos(x1,y1);
      assert(!rect->scene());
      this->ui->view->scene()->addItem(rect);
      rects.push_back(rect);
      rect->m_signal_request_scene_update.connect(
        boost::bind(&ribi::QtTestQtArrowItemsMainDialog::OnRequestSceneUpdate,this));
    }
    for (int i=0; i<n_items-1; i+=2)
    {
      assert(i  < n_items);
      QtLabeledQuadBezierArrowItem * const item = new QtLabeledQuadBezierArrowItem(
        rects.at(i),
        false,
        boost::lexical_cast<std::string>(i/2),
        true,
        rects.at(i+1)
      );
      assert(!item->scene());
      this->ui->view->scene()->addItem(item);
    }
  }
}

ribi::QtTestQtArrowItemsMainDialog::~QtTestQtArrowItemsMainDialog() noexcept
{
  delete ui;
}

//From http://www.richelbilderbeek.nl/CppGetRandomUniform.htm
double ribi::QtTestQtArrowItemsMainDialog::GetRandomUniform()
{
  return static_cast<double>(std::rand())/static_cast<double>(RAND_MAX);
}

void ribi::QtTestQtArrowItemsMainDialog::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Escape) { close(); return; }
}


void ribi::QtTestQtArrowItemsMainDialog::OnRequestSceneUpdate()
{
  this->ui->view->scene()->update();
}

#ifndef NDEBUG
void ribi::QtTestQtArrowItemsMainDialog::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting ribi::QtTestQtArrowItemsMainDialog::Test");
  TRACE("Finished ribi::QtTestQtArrowItemsMainDialog::Test successfully");
}
#endif
