//---------------------------------------------------------------------------
/*
Plane, 3D plane class
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
//From http://www.richelbilderbeek.nl/CppPlane.htm
//---------------------------------------------------------------------------
#include "planey.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "plane.h"

#include <cassert>

#include <boost/xpressive/xpressive.hpp>

#include "geometry.h"
#include "trace.h"
#pragma GCC diagnostic pop

std::vector<boost::geometry::model::d2::point_xy<double>> ribi::PlaneY::CalcProjection(
  const std::vector<boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>>& points
) const
{
  auto v(points);
  for(auto& i: v) { i = Rotate(i); }
  try
  {
    return m_plane_z.CalcProjection(v);
  }
  catch (std::logic_error&)
  {
    throw std::logic_error("PlaneY::CalcProjection: cannot calculate projection");
  }
}

double ribi::PlaneY::CalcY(const double x, const double z) const
{
  try
  {
    return m_plane_z.CalcZ(x,z);
  }
  catch (std::logic_error&)
  {
    throw std::logic_error("ribi::PlaneY::CalcY: cannot calculate Y of a horizontal plane");
  }
}

ribi::PlaneZ ribi::PlaneY::Create(
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& p1,
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& p2,
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& p3
) noexcept
{
  return PlaneZ(Rotate(p1), Rotate(p2), Rotate(p3));
}

const std::vector<double> ribi::PlaneY::GetCoefficients() const noexcept
{
  const auto v(m_plane_z.GetCoefficients());
  assert(v.size() == 4);
  return { v[1],v[2],v[0],v[3] };
}

std::vector<std::string>
  ribi::PlaneY::GetRegexMatchesBoostXpressive(
  const std::string& s,
  const std::string& r_str)
{
  const boost::xpressive::sregex r(boost::xpressive::sregex::compile(r_str));
  std::vector<std::string> v;
  boost::xpressive::sregex_iterator cur(s.begin(),s.end(),r);
  boost::xpressive::sregex_iterator end;
  for( ; cur != end; ++cur )
  {
    const boost::xpressive::smatch& what = *cur;
    v.push_back(what[0]);
  }
  return v;
}

std::string ribi::PlaneY::GetVersion() const noexcept
{
  return "1.1";
}

std::vector<std::string> ribi::PlaneY::GetVersionHistory() const noexcept
{
  return {
    "2014-03-10: version 1.0: initial version, split off from PlaneX",
    "2014-03-13: version 1.1: bug fixed"
  };
}

std::vector<double> ribi::PlaneY::Rotate(const std::vector<double>& coefficients) noexcept
{
  assert(coefficients.size() == 4);
  return
  {
    coefficients[1],
    coefficients[2],
    coefficients[0],
    coefficients[3]
  };
}

boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> ribi::PlaneY::Rotate(
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& point
) noexcept
{
  //The 0-2-1 order is confirmed by doing a projection of a triangle on the Y=0 plane
  //on a Y=0 plane
  return Coordinat3D(
    boost::geometry::get<0>(point),
    boost::geometry::get<2>(point),
    boost::geometry::get<1>(point)
  );
}

#ifndef NDEBUG
void ribi::PlaneY::Test() noexcept
{
  {
    static bool is_tested { false };
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting ribi::PlaneY::Test");
  typedef boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> Point3D;
  const bool verbose = false;
  if (verbose) TRACE("Default construction");
  {
    const PlaneY p;
    assert(!p.ToFunction().empty());
    assert(!p.GetCoefficients().empty());
  }
  if (verbose) TRACE("GetProjection, for plane Y = 0.0, points on plane");
  {
    typedef boost::geometry::model::d2::point_xy<double> Point2D;
    typedef boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> Point3D;
    using boost::geometry::get;
    /*

    A: (0,0,1)                  A: (0,1)
    B: (0,0,0)                  B: (0,0)
    C: (1,0,0)                  C: (1,0)

    |    /
    |   /                       |
    A  /                        A
    |\/         -> becomes ->   |\
    |/\                         | \
  --B--C---                   --B--C-----
   /|                           |
  / |                           |

    */
    const Point3D a(0.0,0.0,1.0);
    const Point3D b(0.0,0.0,0.0);
    const Point3D c(1.0,0.0,0.0);
    assert(PlaneY().ToFunction() == PlaneY(a,b,c).ToFunction()
      && "The three points are on the Y = 0 plane");
    const std::vector<Point2D> v { PlaneY().CalcProjection( { a,b,c } ) };
    assert(v.size() == 3);
    assert(std::abs(get<0>(v[0]) - 0.0 ) < 0.001);
    assert(std::abs(get<1>(v[0]) - 1.0 ) < 0.001);
    assert(std::abs(get<0>(v[1]) - 0.0 ) < 0.001);
    assert(std::abs(get<1>(v[1]) - 0.0 ) < 0.001);
    assert(std::abs(get<0>(v[2]) - 1.0 ) < 0.001);
    assert(std::abs(get<1>(v[2]) - 0.0 ) < 0.001);
  }
  /*
  {


    typedef Point3D Point;
    const double p1_x { 1.0 };
    const double p1_y { 2.0 };
    const double p1_z { 3.0 };
    const double p2_x { 4.0 };
    const double p2_y { 6.0 };
    const double p2_z { 9.0 };
    const double p3_x {12.0 };
    const double p3_y {11.0 };
    const double p3_z { 9.0 };
    PlaneY p(
      Point(p1_x,p1_y,p1_z),
      Point(p2_x,p2_y,p2_z),
      Point(p3_x,p3_y,p3_z)
    );
    const auto t(p.GetCoefficients());
    assert(t.size() == 4);
    const double a { t[0] };
    const double b { t[1] };
    const double c { t[2] };
    const double d { t[3] };
    const double a_expected {  30.0 };
    const double b_expected { -48.0 };
    const double c_expected {  17.0 };
    const double d_expected { -15.0 };
    assert(std::abs(a - a_expected) < 0.001);
    assert(std::abs(b - b_expected) < 0.001);
    assert(std::abs(c - c_expected) < 0.001);
    assert(std::abs(d - d_expected) < 0.001);
    const double d_p1_expected { (a * p1_x) + (b * p1_y) + (c * p1_z) };
    const double d_p2_expected { (a * p2_x) + (b * p2_y) + (c * p2_z) };
    const double d_p3_expected { (a * p3_x) + (b * p3_y) + (c * p3_z) };

    const bool verbose = false;
    if (verbose)
    {
      std::clog
        << "(a * x) + (b * y) + (c * z) = d" << '\n'
        << "(" << a << " * x) + (" << b << " * y) + (" << c << " * z) = " << d << '\n'
        << "(" << a << " * " << p1_x << ") + (" << b << " * " << p1_y << ") + (" << c << " * " << p1_z << ") = " << d << '\n'
        << "(" << (a * p1_x) << ") + (" << (b * p1_y) << ") + (" << (c * p1_z) << ") = " << d << '\n'
        << "(" << a << " * " << p2_x << ") + (" << b << " * " << p2_y << ") + (" << c << " * " << p2_z << ") = " << d << '\n'
        << "(" << (a * p2_x) << ") + (" << (b * p2_y) << ") + (" << (c * p2_z) << ") = " << d << '\n'
        << "(" << a << " * " << p3_x << ") + (" << b << " * " << p3_y << ") + (" << c << " * " << p3_z << ") = " << d << '\n'
        << "(" << (a * p3_x) << ") + (" << (b * p3_y) << ") + (" << (c * p3_z) << ") = " << d << '\n'
      ;

    }
    assert(std::abs(d - d_p1_expected) < 0.001);
    assert(std::abs(d - d_p2_expected) < 0.001);
    assert(std::abs(d - d_p3_expected) < 0.001);
  }
  */
  if (verbose) TRACE("CalcPlaneY");
  /*
  {
    //CalcPlaneY return the coefficients in the following form:
    // A.x + B.y + C.z = D
    //Converting this to z being a function of x and y:
    // -C.z = A.x + B.y - D
    // z = -A/C.x - B/C.y + D/C
    //In this test, use the formula:
    //  z = (2.0 * x) + (3.0 * y) + (5.0)
    //Coefficients must then become:
    //  -A/C = 2.0
    //  -B/C = 3.0
    //   D/C = 5.0
    //Coefficients are, when setting C to 1.0:
    //  -A = 2.0 => A = -2.0
    //  -B = 3.0 => B = -3.0
    //   C = 1.0
    //   D = 5.0


    const Point3D p1(1.0,1.0,10.0);
    const Point3D p2(1.0,2.0,13.0);
    const Point3D p3(2.0,1.0,12.0);
    PlaneY p(p1,p2,p3);
    const auto t(p.GetCoefficients());
    const double a { t[0] };
    const double b { t[1] };
    const double c { t[2] };
    const double d { t[3] };
    const double a_expected { -2.0 };
    const double b_expected { -3.0 };
    const double c_expected {  1.0 };
    const double d_expected {  5.0 };
    assert(std::abs(a - a_expected) < 0.001);
    assert(std::abs(b - b_expected) < 0.001);
    assert(std::abs(c - c_expected) < 0.001);
    assert(std::abs(d - d_expected) < 0.001);
    const double d_p1_expected { (a * 1.0) + (b * 1.0) + (c * 10.0) };
    const double d_p2_expected { (a * 1.0) + (b * 2.0) + (c * 13.0) };
    const double d_p3_expected { (a * 2.0) + (b * 1.0) + (c * 12.0) };
    assert(std::abs(d - d_p1_expected) < 0.001);
    assert(std::abs(d - d_p2_expected) < 0.001);
    assert(std::abs(d - d_p3_expected) < 0.001);
    TRACE(p.ToFunction());
  }
  */
  if (verbose) TRACE("CalcY, diagonal plane");
  {


    const Point3D p1(1.0,2.0,3.0);
    const Point3D p2(2.0,5.0,8.0);
    const Point3D p3(3.0,7.0,11.0);
    PlaneY p(p1,p2,p3);
    assert( std::abs(p.CalcY(1.0, 3.0)- 2.0) < 0.001);
    assert( std::abs(p.CalcY(2.0, 8.0)- 5.0) < 0.001);
    assert( std::abs(p.CalcY(3.0,11.0)- 7.0) < 0.001);
  }
  if (verbose) TRACE("CalcY, vertical plane Y = 3.0");
  /*

    |####/
    |###/#
    |##/##
    |#/###
    |/####
 ---+-----
   /|
  / |
 /  |

  */
  {


    const Point3D p1( 2.0,3.0, 5.0);
    const Point3D p2( 7.0,3.0,11.0);
    const Point3D p3(13.0,3.0,17.0);
    PlaneY p(p1,p2,p3);
    assert( std::abs(p.CalcY(1.0,2.0)-3.0) < 0.001);
    assert( std::abs(p.CalcY(3.0,5.0)-3.0) < 0.001);
    assert( std::abs(p.CalcY(7.0,9.0)-3.0) < 0.001);
  }
  if (verbose) TRACE("ToFunction, 3 points and 4 points");
  {
    std::function<double(double,double)> f {
      [](const double x, const double z)
      {
        return (2.0 * x) + (3.0 * z) + 5.0;
      }
    };


    const double x1 { 2.0 };
    const double z1 { 3.0 };
    const double x2 { 5.0 };
    const double z2 { 7.0 };
    const double x3 { 11.0 };
    const double z3 { 13.0 };
    const double x4 { 17.0 };
    const double z4 { 29.0 };
    const Point3D p1(x1,f(x1,z1),z1);
    const Point3D p2(x2,f(x2,z2),z2);
    const Point3D p3(x3,f(x3,z3),z3);
    const PlaneY a(p1,p2,p3);
    assert(a.ToFunction() == "y=(2*x) + (3*z) + 5");
    const Point3D p4(x4,f(x4,z4),z4);
    assert(a.ToFunction() == PlaneY(p1,p2,p4).ToFunction());
    assert(a.ToFunction() == PlaneY(p1,p3,p4).ToFunction());
    assert(a.ToFunction() == PlaneY(p1,p4,p3).ToFunction());
    assert(a.ToFunction() == PlaneY(p2,p1,p3).ToFunction());
    assert(a.ToFunction() == PlaneY(p2,p1,p4).ToFunction());
    assert(a.ToFunction() == PlaneY(p2,p3,p1).ToFunction());
    assert(a.ToFunction() == PlaneY(p2,p3,p4).ToFunction());
    assert(a.ToFunction() == PlaneY(p2,p4,p1).ToFunction());
    assert(a.ToFunction() == PlaneY(p2,p4,p3).ToFunction());
    assert(a.ToFunction() == PlaneY(p3,p1,p2).ToFunction());
    assert(a.ToFunction() == PlaneY(p3,p1,p4).ToFunction());
    assert(a.ToFunction() == PlaneY(p3,p2,p1).ToFunction());
    assert(a.ToFunction() == PlaneY(p3,p2,p4).ToFunction());
    assert(a.ToFunction() == PlaneY(p3,p4,p1).ToFunction());
    assert(a.ToFunction() == PlaneY(p3,p4,p2).ToFunction());
    assert(a.ToFunction() == PlaneY(p4,p1,p2).ToFunction());
    assert(a.ToFunction() == PlaneY(p4,p1,p3).ToFunction());
    assert(a.ToFunction() == PlaneY(p4,p2,p1).ToFunction());
    assert(a.ToFunction() == PlaneY(p4,p2,p3).ToFunction());
    assert(a.ToFunction() == PlaneY(p4,p3,p1).ToFunction());
    assert(a.ToFunction() == PlaneY(p4,p3,p2).ToFunction());
  }


  if (verbose) TRACE("GetProjection, for plane Y = 0.0, points not on plane");
  {
    typedef boost::geometry::model::d2::point_xy<double> Point2D;
    typedef boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> Point3D;
    using boost::geometry::get;
    /*

    A: (0,0,1)                  A: (0,1)
    B: (1,0,0)                  B: (1,0)
    C: (1,1,0)                  C: (SQRT(2),0)

    |    /
    |   /                       |
    A-----C                     A
    |\/  /      -> becomes ->   |\
    |/\ /                       | \
  --+--B---                   --B--C-----
   /|                           |
  / |                           |

    */
    const std::vector<Point2D> v {
      PlaneY().CalcProjection(
        {
          Point3D(0.0,0.0,1.0),
          Point3D(1.0,0.0,0.0),
          Point3D(1.0,1.0,0.0)
        }
      )
    };
    assert(v.size() == 3);
    assert(std::abs(get<0>(v[0]) -           0.0 ) < 0.001);
    assert(std::abs(get<1>(v[0]) -           1.0 ) < 0.001);
    assert(std::abs(get<0>(v[1]) -           1.0 ) < 0.001);
    assert(std::abs(get<1>(v[1]) -           0.0 ) < 0.001);
    assert(std::abs(get<0>(v[2]) - std::sqrt(2.0)) < 0.001);
    assert(std::abs(get<1>(v[2]) -           0.0 ) < 0.001);
  }
  TRACE("Finished ribi::PlaneY::Test successfully");
}
#endif

std::string ribi::PlaneY::ToFunction() const
{
  try
  {
    std::string s = m_plane_z.ToFunction();
    // 'z=(2*x) + (3*y) + 5'
    //          =>
    // 'y=(2*x) + (3*z) + 5'
    assert(!s.empty());
    s = boost::algorithm::replace_all_copy(s,"*x","*x");
    s = boost::algorithm::replace_all_copy(s,"*y","*z");
    s = boost::algorithm::replace_all_copy(s,"z=","y=");
    return s;
  }
  catch (std::logic_error&)
  {
    throw std::logic_error("ribi::PlaneY::ToFunction: cannot calculate X of a horizontal plane");
  }
}

