//---------------------------------------------------------------------------
/*
Coordinat, coordinat classes
Copyright (C) 2013-2014 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/CppCoordinat.htm
//---------------------------------------------------------------------------
#ifndef COORDINAT2D_H
#define COORDINAT2D_H

//typedef boost::geometry::model::d2::point_xy<double> Coordinat2D;

#ifdef USE_CUSTOM_RIBI_COORDINAT2D

#include <array>
#include <iosfwd>
#include <vector>

namespace ribi {

///An X-Y coordinat
//Note: I dislike to write this class: I wish there was a library (STL, Boost)
//with an alternative.
struct Coordinat2D
{
  Coordinat2D(
    const double x = 0.0,
    const double y = 0.0
  ) noexcept;

  void ChangeX(const double dx) noexcept { m_co[0] += dx; }
  void ChangeY(const double dy) noexcept { m_co[1] += dy; }

  double GetX() const noexcept { return m_co[0]; }
  double GetY() const noexcept { return m_co[1]; }

  void SetX(const double x) noexcept { m_co[0] = x; }
  void SetY(const double y) noexcept { m_co[1] = y; }

  Coordinat2D& operator+=(const Coordinat2D& rhs) noexcept;
  Coordinat2D& operator-=(const Coordinat2D& rhs) noexcept;

  private:
  static const int dimensionality = 2;
  std::array<double,dimensionality> m_co;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

bool operator==(const Coordinat2D& lhs, const Coordinat2D& rhs) noexcept;
bool operator<(const Coordinat2D& lhs, const Coordinat2D& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const Coordinat2D& n) noexcept;

///The dot product
double operator*(const Coordinat2D& v1,const Coordinat2D& v2) noexcept;

///Calculate the point in the center of the collection of points
Coordinat2D CalcCenter(const std::vector<Coordinat2D>& points) noexcept;

double Distance(const Coordinat2D& lhs,const Coordinat2D& rhs) noexcept;

///Distance to origin
double Length(const Coordinat2D& v) noexcept;

Coordinat2D Scale(
  const double scalar,
  const Coordinat2D& v
) noexcept;

Coordinat2D operator+(
  const Coordinat2D& v1,
  const Coordinat2D& v2) noexcept;

Coordinat2D operator*(
  const double scalar,
  const Coordinat2D& v) noexcept;

Coordinat2D operator-(
  const Coordinat2D& v1,
  const Coordinat2D& v2) noexcept;

} //~namespace ribi

#endif

#endif // COORDINAT2D_H
