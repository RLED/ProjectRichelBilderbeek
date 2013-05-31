//---------------------------------------------------------------------------
/*
WtChessBoardWidget, Wt widget for displaying the Chess::Board class
Copyright (C) 2011 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/CppWtChessBoardWidget.htm
//---------------------------------------------------------------------------
#include <iostream>
//---------------------------------------------------------------------------
#include <boost/filesystem.hpp>
//---------------------------------------------------------------------------
#include <Wt/WPaintDevice>
#include <Wt/WPainter>
//---------------------------------------------------------------------------
#include "chessboard.h"
#include "chesssquare.h"
#include "chesssquareselector.h"
#include "trace.h"
#include "wtchessboardwidget.h"
#include "wtchessresources.h"
//---------------------------------------------------------------------------
/*
WtChessBoardWidget::WtChessBoardWidget(QWidget *parent)
  : QWidget(parent),
    m_resources(new Chess::WtResources),
    m_widget(new Chess::BoardWidget(
      boost::shared_ptr<Chess::Board>(new Chess::Board),
      Rect(0,0,200,400)))
{
  assert(m_widget);

  m_widget->m_signal_geometry_changed.connect(
    boost::bind(
      &WtChessBoardWidget::DoRepaint,
      this));

  m_widget->m_signal_board_changed.connect(
    boost::bind(
      &WtChessBoardWidget::DoRepaint,
      this));

  resize(200,200);
}
*/
//---------------------------------------------------------------------------
WtChessBoardWidget::WtChessBoardWidget(
  const int width, const int height)
  : m_resources(new Chess::WtResources),
    m_widget(new Chess::BoardWidget(
      boost::shared_ptr<Chess::Board>(new Chess::Board),
      Rect(0,0,width,height)))
{
  assert(m_widget);

  //m_widget->GetMachine()->GetDialBack()->GetDial()->m_signal_position_changed.connect(boost::bind(
  //  &WtChessBoardWidget::DoRepaint,this));
  //m_widget->GetMachine()->GetDialFront()->GetDial()->m_signal_position_changed.connect(boost::bind(
  //  &WtChessBoardWidget::DoRepaint,this));
  //m_widget->GetMachine()->GetToggleButton()->GetToggleButton()->m_signal_toggled.connect(boost::bind(
  //  &WtChessBoardWidget::DoRepaint,this));
  //m_widget->m_signal_geometry_changed.connect(
  //  boost::bind(
  //    &WtChessBoardWidget::DoRepaint,
  //    this));

  m_widget->m_signal_geometry_changed.connect(
    boost::bind(
      &WtChessBoardWidget::OnResize,
      this));

  m_widget->m_signal_board_changed.connect(
    boost::bind(
      &WtChessBoardWidget::DoRepaint,
      this));

  this->clicked().connect(this,&WtChessBoardWidget::OnClicked);

  resize(width,height);
}
//---------------------------------------------------------------------------
void WtChessBoardWidget::DoRepaint()
{
  //this->repaint();
  this->update();
}
//---------------------------------------------------------------------------
void WtChessBoardWidget::DrawChessBoard(
  Wt::WPainter& painter,
  const Chess::BoardWidget * const widget)
{
  const int w = widget->GetGeometry().GetWidth();
  const int h = widget->GetGeometry().GetHeight();

  //Draw the plain chessboard
  DrawChessBoard(
    painter,
    widget->GetGeometry().GetX(),
    widget->GetGeometry().GetY(),
    widget->GetGeometry().GetWidth(),
    widget->GetGeometry().GetHeight(),
    widget->GetBoard());

  //Draw the selected square
  static const Chess::WtResources r;
  const int square_w = w / 8;
  const int square_h = h / 8;
  const boost::scoped_ptr<Chess::Square>& s = widget->GetSelector()->GetSelected();
  if (s)
  {
    const int x_co = s->GetFile().ToInt() * square_w;
    const int y_co = s->GetRank().ToInt() * square_h;
    const Wt::WRectF rect = Wt::WRectF(x_co,y_co,square_w,square_h);
    if (widget->GetBoard()->GetPiece(*s.get()))
    {
      const std::string filename = r.Find(widget->GetBoard()->GetPiece(*s.get()),Chess::SquareSelector::m_selected_color);
      assert(boost::filesystem::exists(filename));
      const Wt::WPainter::Image image = Wt::WPainter::Image(filename,filename);
      painter.drawImage(rect,image);
    }
    else
    {
      assert(!"Should not get here");
    }

    //Draw the possible moves
    const std::vector<Chess::Move> moves = widget->GetBoard()->GetMoves(*s.get());
    std::for_each(moves.begin(),moves.end(),
      [&painter,square_w,square_h,widget](const Chess::Move& move)
      {
        if (move.To())
        {
          const int x_co = move.To()->GetFile().ToInt() * square_w;
          const int y_co = move.To()->GetRank().ToInt() * square_h;
          const Wt::WRectF rect = Wt::WRectF(x_co,y_co,square_w,square_h);
          if (widget->GetBoard()->GetPiece(*move.To().get()))
          {
            const std::string filename = Chess::Resources::Find(widget->GetBoard()->GetPiece(*move.To().get()),Chess::SquareSelector::m_moves_color);
            assert(boost::filesystem::exists(filename));
            const Wt::WPainter::Image image = Wt::WPainter::Image(filename,filename);
            painter.drawImage(rect,image);
          }
          else
          {
            const std::string filename
              = Chess::Resources::Find(Chess::Square(move.To()->GetFile().ToInt(),move.To()->GetRank().ToInt()),Chess::SquareSelector::m_moves_color);
            assert(boost::filesystem::exists(filename));
            const Wt::WPainter::Image image = Wt::WPainter::Image(filename,filename);
            painter.drawImage(rect,image);
          }
        }
      }
    );
  }

}
//---------------------------------------------------------------------------
void WtChessBoardWidget::DrawChessBoard(
  Wt::WPainter& painter,
  const int left, const int top,
  const int width, const int height,
  const Chess::Board * const board)
{
  static const Chess::WtResources r;
  const double square_w = static_cast<double>(width ) / 8.0;
  const double square_h = static_cast<double>(height) / 8.0;
  for (int y=0; y!=8; ++y)
  {
    for (int x=0; x!=8; ++x)
    {
      const int x_co = left + (x * square_w);
      const int y_co = top + (y * square_h);
      const Wt::WRectF rect = Wt::WRectF(x_co,y_co,square_w,square_h);
      if (board->GetPiece(Chess::Square(x,y)))
      {
        const std::string filename = r.Find(board->GetPiece(Chess::Square(x,y)));
        const Wt::WPainter::Image image = Wt::WPainter::Image(filename,filename);
        painter.drawImage(rect,image);
      }
      else
      {
        const std::string filename = r.Find(Chess::Square(x,y));
        const Wt::WPainter::Image image = Wt::WPainter::Image(filename,filename);
        painter.drawImage(rect,image);
      }
    }
  }
}
//---------------------------------------------------------------------------
const std::string WtChessBoardWidget::GetVersion()
{
  return "1.0";
}
//---------------------------------------------------------------------------
const std::vector<std::string> WtChessBoardWidget::GetVersionHistory()
{
  std::vector<std::string> v;
  v.push_back("2012-01-26: version 1.0: initial version");
  return v;
}
//---------------------------------------------------------------------------
void WtChessBoardWidget::OnClicked(const Wt::WMouseEvent& e)
{
  const int x = e.widget().x;
  const int y = e.widget().y;
  m_widget->Click(x,y);
  //DoRepaint();
}
//---------------------------------------------------------------------------
void WtChessBoardWidget::OnResize()
{
  WtChessBoardWidget::resize(m_widget->GetGeometry().GetWidth(),m_widget->GetGeometry().GetHeight());
}
//---------------------------------------------------------------------------
void WtChessBoardWidget::paintEvent(Wt::WPaintDevice * paintdevice)
{
  Wt::WPainter painter(paintdevice);
  DrawChessBoard(painter,this->m_widget.get());
}
//---------------------------------------------------------------------------
void WtChessBoardWidget::resize(const Wt::WLength& width, const Wt::WLength& height)
{
  Wt::WPaintedWidget::resize(width,height);
}
//---------------------------------------------------------------------------
