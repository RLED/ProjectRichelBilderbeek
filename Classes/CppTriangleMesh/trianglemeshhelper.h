#ifndef TRIANGLEMESHHELPER_H
#define TRIANGLEMESHHELPER_H

#include <algorithm>
#include <cassert>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <boost/geometry.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include "trianglemeshfwd.h"
#include "trianglemeshwinding.h"

#pragma GCC diagnostic pop

namespace ribi {
namespace trim {

struct Helper
{
  typedef boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> Coordinat3D;
  typedef std::set<Coordinat3D,std::function<bool(Coordinat3D,Coordinat3D)>> Coordinat3dSet;

  Helper();

  ///Calculate the point in the center of the collection of edges
  #ifdef USE_TRIANGLEMESHEDGE
  Coordinat3D CalcCenter(const std::vector<boost::shared_ptr<Edge>>& edges) const noexcept;
  #endif

  Coordinat3D CalcCenter(const std::vector<boost::shared_ptr<Point>>& points) const noexcept;

  #ifdef USE_TRIANGLEMESHEDGE
  Coordinat3D CalcNormal(
    const std::vector<boost::shared_ptr<Edge>>& edges
  ) const noexcept;
  #endif

  ///Find out the Winding of the edges
  ///knowing that all edges are in the same XY plane
  ///when viewed from above (at an infinite Z coordinat)
  Winding CalcWindingHorizontal(const std::vector<boost::shared_ptr<const Point>>& points) const noexcept;
  #ifdef USE_TRIANGLEMESHEDGE
  Winding CalcWindingHorizontal(const std::vector<boost::shared_ptr<const Edge>>& edges) const noexcept;
  #endif

  Coordinat3dSet ExtractCoordinats(const Face& face);
  std::set<Coordinat3D,std::function<bool(Coordinat3D,Coordinat3D)>> ExtractCoordinats(const std::vector<boost::shared_ptr<const Point>>& points);

  ///Obtain the angle in radians between two deltas
  ///12 o'clock is 0.0 * pi
  /// 3 o'clock is 0.5 * pi
  /// 6 o'clock is 1.0 * pi
  /// 9 o'clock is 1.5 * pi

  /*
   Y          Y
   |    (11)  |  (1)
 -2|          |
   |          |
 -1| (10)     |      (2)
   |          |
  0+----------0--------X
   |          |
 +1| (8)      |      (4)
   |          |
 +2|          |
   |     (7)  |  (5)
   +----------+--------X
        - - -   + + +
        3 2 1 0 1 2 3

  Appriximate coordinat for a point for every hour, with the approximate angle
   1: ( 1,-2) :  1/6 * pi
   2: ( 2,-1) :  2/6 * pi
   3: ( 3, 0) :  3/6 * pi
   4: ( 2, 1) :  4/6 * pi
   5: ( 1, 2) :  5/6 * pi
   6: ( 0, 3) :  6/6 * pi
   7: (-1, 2) :  7/6 * pi
   8: (-2, 1) :  8/6 * pi
   9: (-3, 0) :  9/6 * pi
  10: (-2,-1) : 10/6 * pi
  11: (-1,-2) : 11/6 * pi
  12: ( 0,-3) : 12/6 * pi

  */
  double GetAngle(const boost::shared_ptr<const Point> point) const noexcept;

  ///Are the points ordered clockwise when seen from the observer
  #ifdef USE_TRIANGLEMESHEDGE
  bool IsClockwise(
    const std::vector<boost::shared_ptr<const Edge>>& edges,
    const Coordinat3D& center
  ) const noexcept;
  bool IsClockwise(
    const std::vector<boost::shared_ptr<Edge>>& edges,
    const Coordinat3D& center
  ) const noexcept;
  bool IsClockwiseConst(
    const std::vector<boost::shared_ptr<const Edge>>& edges,
    const Coordinat3D& center
  ) const noexcept;
  #endif

  bool IsClockwise(
    const std::vector<boost::shared_ptr<const Point>>& points,
    const Coordinat3D& observer
  ) const noexcept;

  ///Are the points ordered clockwise in the XY plane seen from above
  /// (e.g. from coordinat {0,0,1} )
  #ifdef USE_TRIANGLEMESHEDGE
  bool IsClockwiseHorizontal(
    const boost::shared_ptr<const Edge> edge,
    const Coordinat3D& observer
  ) const noexcept;
  #endif


  ///Are the points ordered clockwise in the XY plane seen from above
  /// (e.g. from coordinat {0,0,1} )
  bool IsClockwiseHorizontal(const std::vector<boost::shared_ptr<Point>>& points) const noexcept;

  ///Are the points ordered clockwise in the XY plane seen from above
  /// (e.g. from coordinat {0,0,1} )
  bool IsClockwiseVertical(
    const std::vector<boost::shared_ptr<Point>>& points,
    const boost::shared_ptr<const Point>& observer
  ) const noexcept;

  bool IsConvex(const std::vector<boost::shared_ptr<const ribi::trim::Point>>& points) const noexcept;
  bool IsConvex(const std::vector<boost::shared_ptr<ribi::trim::Point>>& points) const noexcept;

  bool IsCounterClockwise(
    const std::vector<boost::shared_ptr<const Point>>& points,
    const Coordinat3D& observer
  ) const noexcept;
  bool IsCounterClockwise(
    const std::vector<boost::shared_ptr<Point>>& points,
    const Coordinat3D& observer
  ) const noexcept;

  ///Do the Edges go from A->B->C
  #ifdef USE_TRIANGLEMESHEDGE
  bool IsDirected(
    const std::vector<boost::shared_ptr<const Edge>>& edges
  ) const noexcept { return IsDirectedConst(edges); }
  bool IsDirected(
    const std::vector<boost::shared_ptr<Edge>>& edges
  ) const noexcept;
  bool IsDirectedConst(
    const std::vector<boost::shared_ptr<const Edge>>& edges
  ) const noexcept;
  #endif

  bool IsHorizontal(const Face& face) noexcept;

  bool IsPlane(
    const std::vector<boost::shared_ptr<const Point>>& points
  ) const noexcept;

  bool IsPlane(
    const std::vector<boost::shared_ptr<Point>>& points
  ) const noexcept;

  #ifdef USE_TRIANGLEMESHEDGE
  bool IsPlane(const std::vector<boost::shared_ptr<const Edge>>& edges) const noexcept;
  bool IsPlane(const std::vector<boost::shared_ptr<      Edge>>& edges) const noexcept;
  #endif

  bool IsVertical(const Face& face) noexcept;

  ///Order the points so that these are convex
  void MakeConvex(std::vector<boost::shared_ptr<Point>>& points) const noexcept;

  std::vector<boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>>
    PointsToCoordinats3D(const std::vector<boost::shared_ptr<const ribi::trim::Point>>& points
  ) const noexcept;

  ///Set the edges to get a certain Winding,
  ///knowing that all edges are in the same XY plane
  ///when viewed from above (at an infinite Z coordinat)
  #ifdef USE_TRIANGLEMESHEDGE
  void SetWindingHorizontal(
    std::vector<boost::shared_ptr<Edge>>& edges,
    const Winding winding
  ) const noexcept;
  #endif

  std::string ToStr(const std::vector<boost::shared_ptr<const Point>>& p) const noexcept;
  std::string ToXml(const boost::geometry::model::d2::point_xy<double>& p) const noexcept;


  private:
  #ifdef USE_TRIANGLEMESHEDGE
  bool IsPlaneConst(const std::vector<boost::shared_ptr<const Edge>>& edges) const noexcept;
  #endif

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

///Help adding constness a bit
template <class T>
std::vector<boost::shared_ptr<const T>> AddConst(
  const std::vector<boost::shared_ptr<T>> v)
{
  assert(std::count(v.begin(),v.end(),nullptr) == 0);
  const std::vector<boost::shared_ptr<const T>> w(v.begin(),v.end());
  assert(std::count(w.begin(),w.end(),nullptr) == 0);
  return w;
}

//From http://www.richelbilderbeek.nl/CppCanLexicalCast.htm
template <class TargetType>
bool CanLexicalCast(const std::string& from)
{
  try
  {
    boost::lexical_cast<TargetType>(from);
  }
  catch (boost::bad_lexical_cast&)
  {
    return false;
  }
  catch (...)
  {
    assert(!"Something unexpected happened");
    throw;
  }
  return true;
}

//bool operator==(
//  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& lhs,
//  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& rhs
//) noexcept;

bool operator<(
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& lhs,
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& rhs
) noexcept;

boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> operator+(
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& lhs,
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& rhs
) noexcept;

boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& operator+=(
  boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& lhs,
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& rhs
) noexcept;

boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& operator/=(
  boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& p,
  const double factor
) noexcept;

boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> operator/(
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& p,
  const double factor
) noexcept;

//Name for operator<
bool Less(
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& lhs,
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& rhs
) noexcept;


//Name for operator+
boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> Add(
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& lhs,
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& rhs
) noexcept;


} //~namespace trim
} //~namespace ribi

#endif // TRIANGLEMESHHELPER_H
