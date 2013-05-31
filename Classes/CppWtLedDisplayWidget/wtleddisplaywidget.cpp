//---------------------------------------------------------------------------
/*
WtLedDisplayWidget, Wt widget for displaying the Led class
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
//From http://www.richelbilderbeek.nl/CppWtLedDisplayWidget.htm
//---------------------------------------------------------------------------
#include <boost/bind.hpp>
#include <boost/numeric/conversion/cast.hpp>
//---------------------------------------------------------------------------
#include <Wt/WPainter>
#include <Wt/WPen>
#include <Wt/WBrush>
//---------------------------------------------------------------------------
#include "led.h"
#include "wtleddisplaywidget.h"
//---------------------------------------------------------------------------
WtLedDisplayWidget::WtLedDisplayWidget(
  const double intensity,
  const unsigned char red,
  const unsigned char green,
  const unsigned char blue)
  : m_led(new Led(intensity,red,green,blue))
{
  assert(m_led);
  m_led->m_signal_intensity_changed.connect(
    boost::bind(
      &WtLedDisplayWidget::DoRepaint,
      this));
}
//---------------------------------------------------------------------------
void WtLedDisplayWidget::DoRepaint()
{
  this->update();
}
//---------------------------------------------------------------------------
const std::string WtLedDisplayWidget::GetVersion()
{
  return "1.0";
}
//---------------------------------------------------------------------------
const std::vector<std::string> WtLedDisplayWidget::GetVersionHistory()
{
  std::vector<std::string> v;
  v.push_back("YYYY-MM-DD: version X.Y: [description]");
  v.push_back("2011-04-11: version 1.0: initial version");
  return v;
}
//---------------------------------------------------------------------------
void WtLedDisplayWidget::paintEvent(Wt::WPaintDevice *paintDevice)
{
  Wt::WPainter p(paintDevice);
  const int red   = boost::numeric_cast<int>(m_led->GetRed());
  const int green = boost::numeric_cast<int>(m_led->GetGreen());
  const int blue  = boost::numeric_cast<int>(m_led->GetBlue());
  const double intensity = m_led->GetIntensity();
  const int width  = this->width().toPixels();
  const int height = this->height().toPixels();
  //Fraction red/green/blue
  const double fR = static_cast<double>(red  ) / 255.0;
  const double fG = static_cast<double>(green) / 255.0;
  const double fB = static_cast<double>(blue ) / 255.0;

  {
    //Draw circle and major surface
    const double maxBrightness = 0.66 * 255.0;
    const double minBrightness = 0.25 * maxBrightness;
    const int r = static_cast<int>(
      minBrightness + (fR * intensity * (maxBrightness - minBrightness) ) );
    const int g = static_cast<int>(
      minBrightness + (fG * intensity * (maxBrightness - minBrightness) ) );
    const int b = static_cast<int>(
      minBrightness + (fB * intensity * (maxBrightness - minBrightness) ) );
    assert( r >= 0 ); assert( r < 256);
    assert( g >= 0 ); assert( g < 256);
    assert( b >= 0 ); assert( b < 256);
    const int pen_width = 1 + (std::min(width,height) / 25);
    {
      Wt::WPen pen = p.pen();
      pen.setWidth(pen_width);
      pen.setColor(Wt::WColor(0,0,0));
      p.setPen(pen);
    }
    p.setBrush(Wt::WBrush(Wt::WColor(r,g,b)));
    p.drawEllipse(
      pen_width,
      pen_width,
      width  - (2 * pen_width),
      height - (2 * pen_width));
  }
  {
    //Draw topleft smaller lighter surface
    const double maxBrightness = 1.00 * 255.0;
    const double minBrightness = 0.25 * maxBrightness;
    const int r = static_cast<int>(
      minBrightness + (fR * intensity * (maxBrightness - minBrightness) ) );
    const int g = static_cast<int>(
      minBrightness + (fG * intensity * (maxBrightness - minBrightness) ) );
    const int b = static_cast<int>(
      minBrightness + (fB * intensity * (maxBrightness - minBrightness) ) );
    assert( r >= 0 ); assert( r < 256);
    assert( g >= 0 ); assert( g < 256);
    assert( b >= 0 ); assert( b < 256);
    const int x = (width  / 2) - (0.707 * static_cast<double>(width ) * 0.5);
    const int y = (height / 2) - (0.707 * static_cast<double>(height) * 0.5);
    const int w = (0.707 * static_cast<double>(width ) * 0.5);
    const int h = (0.707 * static_cast<double>(height) * 0.5);
    {
      Wt::WPen pen = p.pen();
      pen.setWidth(1);
      pen.setColor(Wt::WColor(r,g,b));
      p.setPen(pen);
    }
    p.setBrush(Wt::WBrush(Wt::WColor(r,g,b)));
    p.drawEllipse(x,y,w,h);
  }
  {
    //Draw bottomright smaller lighter surface
    const double maxBrightness = 0.33 * 255.0;
    const double minBrightness = 0.25 * maxBrightness;
    const int r = static_cast<int>(
      minBrightness + (fR * intensity * (maxBrightness - minBrightness) ) );
    const int g = static_cast<int>(
      minBrightness + (fG * intensity * (maxBrightness - minBrightness) ) );
    const int b = static_cast<int>(
      minBrightness + (fB * intensity * (maxBrightness - minBrightness) ) );
    assert( r >= 0 ); assert( r < 256);
    assert( g >= 0 ); assert( g < 256);
    assert( b >= 0 ); assert( b < 256);
    const int x = (width  / 2) - 1;
    const int y = (height / 2) - 1;
    const int w = (0.707 * static_cast<double>(width ) * 0.5);
    const int h = (0.707 * static_cast<double>(height) * 0.5);

    {
      Wt::WPen pen = p.pen();
      pen.setWidth(1);
      pen.setColor(Wt::WColor(r,g,b));
      p.setPen(pen);
    }
    p.setBrush(Wt::WBrush(Wt::WColor(r,g,b)));
    p.drawEllipse(x,y,w,h);
  }
}
//---------------------------------------------------------------------------
