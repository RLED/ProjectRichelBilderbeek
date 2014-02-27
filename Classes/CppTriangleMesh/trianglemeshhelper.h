#ifndef TRIANGLEMESHHELPER_H
#define TRIANGLEMESHHELPER_H

#include <algorithm>
#include <cassert>
#include <map>
#include <set>
#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <boost/lexical_cast.hpp>

#include "constcoordinat2d.h"
#include "coordinat3d.h"
#include "trianglemeshfwd.h"
#include "trianglemeshwinding.h"

#pragma GCC diagnostic pop

namespace ribi {
namespace trim {

struct Helper
{
  Helper();

  ///Calculate the point in the center of the collection of edges
  const Coordinat3D CalcCenter(const std::vector<boost::shared_ptr<Edge>>& edges) const noexcept;

  const Coordinat3D CalcCenter(const std::vector<boost::shared_ptr<Point>>& points) const noexcept;

  const Coordinat3D CalcNormal(
    const std::vector<boost::shared_ptr<Edge>>& edges
  ) const noexcept;

  ///Find out the Winding of the edges
  ///knowing that all edges are in the same XY plane
  ///when viewed from above (at an infinite Z coordinat)
  Winding CalcWindingHorizontal(const std::vector<boost::shared_ptr<const Edge>>& edges) const noexcept;

  const std::set<ribi::Coordinat3D> ExtractCoordinats(const Face& face);
  const std::set<ribi::Coordinat3D> ExtractCoordinats(const std::vector<boost::shared_ptr<Point>>& points);

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
  bool IsClockwise(
    const std::vector<boost::shared_ptr<const Edge>>& edges,
    const Coordinat3D& center
  ) const noexcept;

  bool IsClockwise(
    const std::vector<boost::shared_ptr<const Point>>& points,
    const Coordinat3D& observer
  ) const noexcept;

  ///Are the points ordered clockwise in the XY plane seen from above
  /// (e.g. from coordinat {0,0,1} )
  bool IsClockwiseHorizontal(
    const boost::shared_ptr<const Edge> edge,
    const Coordinat3D& observer
  ) const noexcept;

  ///Are the points ordered clockwise in the XY plane seen from above
  /// (e.g. from coordinat {0,0,1} )
  bool IsClockwiseHorizontal(const std::vector<boost::shared_ptr<Point>>& points) const noexcept;

  ///Are the points ordered clockwise in the XY plane seen from above
  /// (e.g. from coordinat {0,0,1} )
  bool IsClockwiseVertical(
    const std::vector<boost::shared_ptr<Point>>& points,
    const boost::shared_ptr<const Point>& observer
  ) const noexcept;

  bool IsHorizontal(const Face& face) noexcept;
  bool IsVertical(const Face& face) noexcept;

  ///Set the edges to get a certain Winding,
  ///knowing that all edges are in the same XY plane
  ///when viewed from above (at an infinite Z coordinat)
  void SetWindingHorizontal(
    std::vector<boost::shared_ptr<Edge>>& edges,
    const Winding winding
  ) const noexcept;

  private:
  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

///Help adding constness a bit
template <class T>
const std::vector<boost::shared_ptr<const T> > AddConst(
  const std::vector<boost::shared_ptr<T> > v)
{
  return std::vector<boost::shared_ptr<const T> >(v.begin(),v.end());
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

} //~namespace trim
} //~namespace ribi

#endif // TRIANGLEMESHHELPER_H
