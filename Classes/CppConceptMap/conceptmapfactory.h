#ifndef CONCEPTMAPCONCEPTMAPFACTORY_H
#define CONCEPTMAPCONCEPTMAPFACTORY_H

#include <array>
#include <vector>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>

#include "conceptmapfwd.h"
#pragma GCC diagnostic pop

namespace ribi {

namespace cmap {

///Factory class to create ConceptMaps
///ConceptMapFactory is the only class using the ConceptMap constructor
struct ConceptMapFactory
{
  //Create an empty-as-can-get ConceptMap
  /*
  static const boost::shared_ptr<ConceptMap> Create(
    const std::string& focal_question);
  */

  static const boost::shared_ptr<ConceptMap> Create(
    const std::vector<boost::shared_ptr<Node> >& nodes = {},
    const std::vector<boost::shared_ptr<Edge> >& edges = {});

  #ifndef NDEBUG
  ///DeepCopy is only used for debugging
  static const boost::shared_ptr<ConceptMap> DeepCopy(
    const boost::shared_ptr<const ConceptMap> map);
  #endif

  ///Obtain a ConceptMap from an XML std::string
  static const boost::shared_ptr<ConceptMap> FromXml(const std::string& s);

  ///Get all the other tests as one vector
  static const std::vector<boost::shared_ptr<ConceptMap> > GetAllTests();

  ///Get the documented heteromorphous test concept maps
  static const std::vector<boost::shared_ptr<ConceptMap> > GetHeteromorphousTestConceptMaps();
  static const boost::shared_ptr<ConceptMap> GetHeteromorphousTestConceptMap(const int index);

  ///Get the documented complex homomorphous test concept maps
  static const std::vector<boost::shared_ptr<ConceptMap> > GetComplexHomomorphousTestConceptMaps();

  ///Get the documented simple homomorphous test concept maps
  static const std::vector<boost::shared_ptr<ConceptMap> > GetSimpleHomomorphousTestConceptMaps();
};

} //~namespace cmap

} //~namespace ribi

#endif // CONCEPTMAPCONCEPTMAPFACTORY_H
