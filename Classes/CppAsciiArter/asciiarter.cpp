//---------------------------------------------------------------------------
/*
AsciiArter, class to convert greyscale to ASCII art
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
//From http://www.richelbilderbeek.nl/CppAsciiArter.htm
//---------------------------------------------------------------------------
/*

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "asciiarter.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <boost/scoped_ptr.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include "canvas.h"

#pragma GCC diagnostic pop

double ribi::AsciiArter::GetGreyness(
  const std::vector<std::vector<double> >& image,
  const int x,
  const int y)
{
  assert(!image.empty() && "Image must not be empty");
  assert(x >= 0 && "x coordinat is below zero");
  assert(y >= 0 && "y coordinat is below zero");
  assert(y < boost::numeric_cast<int>(image.size())
    && "y coordinat is beyond image height");
  assert(x < boost::numeric_cast<int>(image[y].size())
    && "x coordinat is beyond image width");
  const double greyness = image[y][x];
  assert(greyness >= 0.0);
  assert(greyness <= 1.0);
  return greyness;
}

double ribi::AsciiArter::GetGreyness(
  const std::vector<std::vector<double> >& image,
  const int x1,
  const int x2,
  const int y)
{
  assert(!image.empty() && "Image is NULL");
  assert(x1 >= 0 && "x1 coordinat is below zero");
  assert(x2 >= 0 && "x2 coordinat is below zero");
  assert(y >= 0 && "y coordinat is below zero");
  assert(y < boost::numeric_cast<int>(image.size())
    && "y coordinat is beyond image height");
  assert(x1 < x2
    && "X-coordinats must be different and ordered");
  assert(x1 < boost::numeric_cast<int>(image[y].size())
    && "x1 coordinat is beyond image width");
  assert(x2 < boost::numeric_cast<int>(image[y].size())
    && "x2 coordinat is beyond image width");
  assert(image[y].begin() + x2 != image[y].end()
    && "x2 coordinat iterator must not be beyond image width");
  const double average_greyness = std::accumulate(
    image[y].begin() + x1,
    image[y].begin() + x2,
    0.0) / boost::numeric_cast<double>(x2-x1);
  assert(average_greyness >= 0.0);
  assert(average_greyness <= 1.0);
  return average_greyness;
}

//Get a square of pixels' average greyness
double ribi::AsciiArter::GetGreyness(
  const std::vector<std::vector<double> >& image,
  const int x1,
  const int y1,
  const int x2,
  const int y2)
{
  assert(y1 < y2
    && "Y-coordinats must be ordered");

  double sum = 0.0;

  for (int y=y1; y!=y2; ++y)
  {
    const double grey = GetGreyness(image,x1,x2,y);
    assert(grey >= 0 && grey < 1.0);
    sum+=grey;
  }
  const double average_greyness = sum
    / boost::numeric_cast<double>(y2 - y1);

  assert(average_greyness >=0.0 && average_greyness <= 1.0);
  return average_greyness;
}

//Generalizes a pixel, line or rectangle to one average greyness
double ribi::AsciiArter::GetFractionGrey(
  const std::vector<std::vector<double> >& image,
  const int x1,
  const int y1,
  const int x2,
  const int y2)
{
  assert(x1 <= x2);
  assert(y1 <= y2);
  if (x1 == x2 && y1 == y2) return GetGreyness(image,x1,y1);
  if (y1 == y2) return GetGreyness(image,x1,x2,y1);
  if (x1 == x2)
  {
    assert(y1 < y2);
    double sum = 0;
    for (int y=y1; y!=y2; ++y)
    {
      const double g = GetGreyness(image,x1,y);
      assert(g >= 0.0);
      assert(g <= 1.0);
      sum+=g;
    }
    const double average_greyness
      = sum / boost::numeric_cast<double>(y2-y1);
    assert(average_greyness >= 0.0);
    assert(average_greyness <= 1.0);
    return average_greyness;
  }
  return GetGreyness(image,x1,y1,x2,y2);
}

const std::string ribi::AsciiArter::GetVersion() noexcept
{
  return "2.0";
}

const std::vector<std::string> ribi::AsciiArter::GetVersionHistory() noexcept
{
  return {
    "2011-03-23: Version 1.0: initial version",
    "2014-01-07: Version 2.0: add conversion to Canvas"
  };
}

//'image' must be a y-x-ordered std::vector of grey values
//ranging from [0.0,1.0], where 0.0 denotes black and
//1.0 denotes white.
//From http://www.richelbilderbeek.nl/CppImageToAscii.htm
const std::vector<std::string> ribi::AsciiArter::ImageToAscii(
  const std::vector<std::vector<double> >& image,
  const int width) //How many chars the ASCII image will be wide
{
  //If the number of chars is below 5,
  //the calculation would be more complicated due to a too trivial value of charWidth
  assert(width >= 5);


  //Maxy is in proportion with the bitmap
  const int image_width  = image[0].size();
  const int image_height = image.size();

  const int maxy =
    (static_cast<double>(width)
    / static_cast<double>(image_width))
    * static_cast<double>(image_height);
  assert(maxy > 0);
  const double dX = static_cast<double>(image_width)
    / static_cast<double>(width);
  const double dY = static_cast<double>(image_height)
    / static_cast<double>(maxy);
  assert(dX > 0.0);
  assert(dY > 0.0);

  std::vector<std::string> v;

  for (int y=0; y!=maxy; ++y)
  {
    std::string s;
    for (int x=0; x!=width; ++x)
    {
      const int x1 = std::min(
        static_cast<double>(x) * dX,
        image_width  - 1.0) + 0.5;
      const int y1 = std::min(
        static_cast<double>(y) * dY,
        image_height - 1.0) + 0.5;
      const int x2 = std::min(
        (static_cast<double>(x) * dX) + dX,
        image_width  - 1.0) + 0.5;
      const int y2 = std::min(
        (static_cast<double>(y) * dY) + dY,
        image_height - 1.0) + 0.5;
      assert(x1 >= 0);
      assert(x2 >= 0);
      assert(y1 >= 0);
      assert(y2 >= 0);
      assert(x1 < image_width);
      assert(x2 < image_width);
      assert(y1 < image_height);
      assert(y2 < image_height);

      const double f = GetFractionGrey(image,x1,y1,x2,y2);
      assert(f >= 0.0 && f <= 1.0);

      const std::vector<char> m_gradient {
        Canvas::GetAsciiArtGradient()
      };
      const int i
        = boost::numeric_cast<int>(
          f * boost::numeric_cast<double>(m_gradient.size() - 1));
      assert(i >= 0);
      assert(i < boost::numeric_cast<int>(m_gradient.size()));
      const char c = m_gradient[i];
      s+=c;
    }
    v.push_back(s);
  }
  return v;
}

*/
