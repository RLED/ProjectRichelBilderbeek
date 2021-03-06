#ifndef CANVASCOLORSYSTEMS_H
#define CANVASCOLORSYSTEMS_H

#include <string>
#include <vector>

#include "canvascolorsystem.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <boost/bimap.hpp>
#pragma GCC diagnostic pop

namespace ribi {

struct CanvasColorSystems
{
  static std::vector<CanvasColorSystem> GetAll() noexcept;
  static std::string ToStr(const CanvasColorSystem s) noexcept;
  static CanvasColorSystem ToType(const std::string& s);

  private:
  static boost::bimap<CanvasColorSystem,std::string> m_map;
  static boost::bimap<CanvasColorSystem,std::string> CreateMap();

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace ribi

#endif // CANVASCOLORSYSTEMS_H
