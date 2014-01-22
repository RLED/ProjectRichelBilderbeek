//---------------------------------------------------------------------------
/*
RubiksClockWidget, class for displaying a RubiksClock
Copyright (C) 2011-2014 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/CppRubiksClockWidget.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "rubiksclockwidget.h"

#include <cassert>
#include <cmath>

#include <iostream>

#include <boost/numeric/conversion/cast.hpp>

#include "dial.h"
#include "drawcanvas.h"
#include "rubiksclock.h"
#include "rubiksclockdial.h"
#include "rubiksclockdialwidget.h"
#include "rectangle.h"
#include "textcanvas.h"
#include "togglebutton.h"
#include "togglebuttonwidget.h"
//#include "trace.h"

#pragma GCC diagnostic pop

ribi::RubiksClockWidget::RubiksClockWidget(
  const int x,
  const int y,
  const int width,
  const int height) noexcept
  : m_signal_widget_flipped{},
    m_clock{new RubiksClock},
    m_display_front{true}
{
  m_signal_geometry_changed.connect(
    boost::bind(
      &ribi::RubiksClockWidget::OnResize,
      this));

  this->SetGeometry(Rect(x,y,width,height));
}

void ribi::RubiksClockWidget::Click(const int x,const int y,const bool button_left) noexcept
{
  RubiksClock::Times& times = (m_display_front ? m_clock->GetFrontTimes() : m_clock->GetBackTimes());
  RubiksClock::Pegs& pegs = m_clock->GetFrontPegs();

  for (int i=0; i!=2; ++i)
  {
    for (int j=0; j!=2; ++j)
    {
      if (times.times[i*2][j*2]->IsClicked(x,y))
      {
        if(m_display_front)
        {
          this->m_clock->TurnWheel(
            i
            ? (j ? RubiksClock::Side::bottomRight : RubiksClock::Side::topRight)
            : (j ? RubiksClock::Side::bottomLeft : RubiksClock::Side::topLeft),
            button_left ? 1 : -1);
        }
        else
        {
          this->m_clock->TurnWheel(
            i
            ? (j ? RubiksClock::Side::bottomLeft : RubiksClock::Side::topLeft)
            : (j ? RubiksClock::Side::bottomRight : RubiksClock::Side::topRight),
            button_left ? 1 : -1);
        }
      }
      else if (pegs.m_pegs[i][j]->GetGeometry().IsIn(x,y))
      {
        m_clock->TogglePeg(
          i
          ? (j ? RubiksClock::Side::bottomRight : RubiksClock::Side::topRight)
          : (j ? RubiksClock::Side::bottomLeft : RubiksClock::Side::topLeft));
      }
    }
  }
}

void ribi::RubiksClockWidget::Flip() noexcept
{
  m_display_front = !m_display_front;
  m_signal_widget_flipped();
}

const std::string ribi::RubiksClockWidget::GetVersion() noexcept
{
  return "1.3";
}

const std::vector<std::string> ribi::RubiksClockWidget::GetVersionHistory() noexcept
{
  return {
    "2011-09-01: Version 1.0: initial version",
    "2011-09-09: Version 1.1: use of geometries",
    "2011-09-15: Version 1.2: allow flipping the clock",
    "2014-01-16: Version 1.3: added noexcept and enum class"
  };
}

void ribi::RubiksClockWidget::OnResize() noexcept
{
  m_clock->SetGeometry(this->GetGeometry());
}

const boost::shared_ptr<ribi::DrawCanvas> ribi::RubiksClockWidget::ToDrawCanvas(const int size) const noexcept
{
  const boost::shared_ptr<ribi::DrawCanvas> c(
    new DrawCanvas(size,size)
  );

  assert(!"TODO");
  return c;
}

const boost::shared_ptr<ribi::TextCanvas> ribi::RubiksClockWidget::ToTextCanvas() const noexcept
{
  const int size = 22;
  const boost::shared_ptr<ribi::TextCanvas> c(
    new TextCanvas(size,size)
  );

/*

  01234567890123456789012

0 /-\    ---------    /-\
1 | |----         ----| |
2 \-/                 \-/
3  | /-\    /-\    /-\ |
4  | |4|    |5|    |9| |
5  | \-/    \-/    \-/ |
6  |     /\            |
7 |      ||     /\      |
8 |      \/     \/      |
9 |  /-\    /-\    /-\  |
0 |  |1|    |1|    |3|  |
1 |  \-/    \-/    \-/  |
2 |      /\             |
3 |      ||     /\      |
4  |     \/     \/     |
5  | /-\    /-\    /-\ |
6  | |3|    |3|    |B| |
7  | \-/    \-/    \-/ |
8 /-\                 /-\
9 | |----         ----| |
0 \-/    ---------    \-/

*/
  const RubiksClock::Pegs pegs {
    GetDisplayFront()
    ? GetRubiksClock()->GetFrontPegs()
    : GetRubiksClock()->GetBackPegs()
  };

  for (int peg_row = 0; peg_row!=2; ++peg_row)
  {
    for (int peg_col = 0; peg_col!=2; ++peg_col)
    {
      pegs.m_pegs[peg_row][peg_col]->GetToggleButton()->IsPressed();
    }
  }

  assert(!"TODO");
  return c;
}

std::ostream& ribi::operator<<(std::ostream& os, const RubiksClockWidget& widget) noexcept
{
  os
    << "<RubiksClockWidget>"
    << *widget.m_clock
    << widget.GetGeometry()
    << "</RubiksClockWidget>";
  return os;
}
