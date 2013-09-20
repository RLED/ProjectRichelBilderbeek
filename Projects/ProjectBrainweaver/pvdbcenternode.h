#ifndef PVDBCENTERNODE_H
#define PVDBCENTERNODE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "pvdbnode.h"
#include "pvdbfwd.h"
#pragma GCC diagnostic pop

namespace ribi {

namespace pvdb {

struct CenterNodeFactory;

///The focal question of a ConceptMap
///In a full ConceptMap, GetNodes()[0] results in finding a CenterNode
///In a sub-ConceptMap, GetNodes()[0] will be a regular Node
struct CenterNode : public pvdb::Node
{
  ///Block destructor, except for the friend boost::checked_delete
  ~CenterNode() {}
  friend void boost::checked_delete<>(CenterNode* x);

  ///Block construction, except for NodeFactory
  friend CenterNodeFactory;
  CenterNode() = delete;
  explicit CenterNode(
    const boost::shared_ptr<ribi::pvdb::Concept>& concept,
    const double x = 0.0,
    const double y = 0.0);
};

} //~namespace pvdb

} //~namespace ribi

#endif // PVDBCENTERNODE_H
