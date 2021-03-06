//---------------------------------------------------------------------------
/*
ConceptMap, concept map classes
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
//From http://www.richelbilderbeek.nl/CppConceptMap.htm
//---------------------------------------------------------------------------
#ifndef CONCEPTMAPEDGE_H
#define CONCEPTMAPEDGE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>
#include "conceptmapfwd.h"
#include "conceptmapelement.h"
#pragma GCC diagnostic pop


namespace ribi {
namespace cmap {

struct EdgeFactory;

///An Edge is the GUI-independent part of the edges used in QtConceptMap.
///An Edge goes from one Node to another, which must a different Node
struct Edge : public Element
{
  Edge(const Edge&) = delete;
  Edge& operator=(const Edge&) = delete;
  boost::shared_ptr<const Concept> GetConcept() const noexcept { return m_concept; }
  boost::shared_ptr<      Concept> GetConcept()       noexcept { return m_concept; }

  ///Get the Node this edge originates from
  boost::shared_ptr<const Node> GetFrom() const noexcept { return m_from; }
  boost::shared_ptr<      Node> GetFrom()       noexcept { return m_from; }

  ///Get the Node index this edge goes to
  boost::shared_ptr<const Node> GetTo() const noexcept { return m_to; }
  boost::shared_ptr<      Node> GetTo()       noexcept { return m_to; }

  ///Get the x coordinat
  double GetX() const noexcept { return m_x; }

  ///Get the y coordinat
  double GetY() const noexcept { return m_y; }

  ///Does the edge have an arrow at the head?
  bool HasHeadArrow() const noexcept { return m_head_arrow; }

  //Similar to operator==, except that the coordinats are not checked
  static bool HasSameContent(const boost::shared_ptr<const Edge>& lhs, const boost::shared_ptr<const Edge>& rhs) noexcept;

  ///Does the edge have an arrow at the tail?
  bool HasTailArrow() const noexcept { return m_tail_arrow; }

  ///Set the concept
  void SetConcept(const boost::shared_ptr<Concept> concept) noexcept { m_concept = concept; }

  ///Set the Node index this edge originates from
  void SetFrom(const boost::shared_ptr<Node> from) noexcept;

  ///Set if the head has an arrow
  void SetHeadArrow(const bool has_head_arrow) noexcept;

  ///Set the coordinat of the concept at the center of the node
  void SetPos(const double x, const double y) noexcept { SetX(x); SetY(y); }

  ///Set if the tail has an arrow
  void SetTailArrow(const bool has_tail_arrow) noexcept;

  ///Set the Node index this edge goes to
  void SetTo(const boost::shared_ptr<Node> to) noexcept;

  ///Set the x coordinat of the concept at the center of the node
  void SetX(const double x) noexcept;

  ///Set the y coordinat of the concept at the center of the node
  void SetY(const double y) noexcept;

  ///Convert an Edge from an XML std::string
  ///The container of nodes is needed to convert the 'to' and 'from'
  ///field to indices
  static std::string ToXml(
    const boost::shared_ptr<const Edge>& c,
    const std::vector<boost::shared_ptr<const Node> >& nodes
    ) noexcept;

  ///Emitted when an Edge attribute has changed
  boost::signals2::signal<void (const Edge*)> m_signal_edge_changed;

  private:

  ///The Concept on the Edge
  boost::shared_ptr<Concept> m_concept;

  ///The Node index this edge originates from
  ///Cannot be an index, see [1] below
  boost::shared_ptr<Node> m_from;

  ///Is there an arrowhead at the 'to' node?
  bool m_head_arrow;

  ///Is there an arrowhead at the 'from' node?
  bool m_tail_arrow;

  ///The Node index this edge goes to
  ///Cannot be an index, see [1] below
  boost::shared_ptr<Node> m_to;

  ///The x-coordinat
  double m_x;

  ///The y-coordinat
  double m_y;

  void EmitSignalEdgeChanged();

  #ifndef NDEBUG
  ///Test this class
  static void Test() noexcept;
  #endif

  Edge() = delete;

  ///Block destructor, except for the friend boost::checked_delete
  ~Edge() noexcept {}
  friend void boost::checked_delete<>(Edge* x);

  ///Block constructor, except for EdgeFactory
  friend EdgeFactory;
  Edge(
    const boost::shared_ptr<Concept> & concept,
    const double concept_x,
    const double concept_y,
    const boost::shared_ptr<Node> from,
    const bool tail_arrow,
    const boost::shared_ptr<Node> to,
    const bool head_arrow);

};

bool IsConnectedToCenterNode(const boost::shared_ptr<const Edge> edge) noexcept;

bool operator==(const Edge& lhs, const Edge& rhs);
bool operator!=(const Edge& lhs, const Edge& rhs);

bool operator<(
  const boost::shared_ptr<Edge>& lhs,
  const boost::shared_ptr<Edge>& rhs) = delete;
bool operator<(
  const boost::shared_ptr<const Edge>& lhs,
  const boost::shared_ptr<      Edge>& rhs) = delete;
bool operator<(
  const boost::shared_ptr<      Edge>& lhs,
  const boost::shared_ptr<const Edge>& rhs) = delete;
bool operator<(
  const boost::shared_ptr<const Edge>& lhs,
  const boost::shared_ptr<const Edge>& rhs) = delete;

///Notes:
/// [1] Node::m_from and Node::m_to cannot be indices, because of the desired copying behavior
/// of Edge: when copying an edge, it is natural that it keeps pointing to the same nodes.
/// When using pointers, this will work. Indices, on the other hand, are context-specific:
/// Example: imagine a concept map like this:
///
/// NodeA Edge1 NodeB Edge2 NodeC
/// [0] [1] [2]
///
/// In this example, Edge2 goes from [1] to [2]
///
/// A sub-concept map will be (when NodeC is the focal node):
///
/// NodeB Edge2 NodeC
/// [0] [1]
///
/// In this example, Edge2 goes from [0] to [1]! Due to this, the same Edge2 cannot behave identical in the different contexts
///
/// Indices are only used when saving and loading

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPEDGE_H
