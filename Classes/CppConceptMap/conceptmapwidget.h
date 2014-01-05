#ifndef CONCEPTMAPWIDGET_H
#define CONCEPTMAPWIDGET_H

#include <iosfwd>
#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>
#include "conceptmapfwd.h"
#include "conceptmapfactory.h"

#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Widget does the Command-driven communication with ConceptMap
///Note that a Widget does not know the type of ConceptMap (Display/Edit/Rate)
struct Widget
{
  Widget(const boost::shared_ptr<ConceptMap> conceptmap = boost::shared_ptr<ConceptMap>());

  #ifndef NDEBUG
  Widget(const Widget& other); //Only to be used in debugging
  #else
  Widget(const Widget& other) = delete;
  #endif

  Widget& operator=(const Widget& rhs) = delete;

  bool CanDoCommand(const boost::shared_ptr<const Command> command) const noexcept;

  void DoCommand(const boost::shared_ptr<Command> command) noexcept;

  const boost::shared_ptr<const ConceptMap> GetConceptMap() const noexcept { return m_conceptmap; }
  const boost::shared_ptr<      ConceptMap> GetConceptMap()       noexcept { return m_conceptmap; }

  ///Obtain all commands possible
  //const std::vector<boost::shared_ptr<Command>> GetAllCommands() const noexcept;

  ///Obtain the version
  static const std::string GetVersion() noexcept;

  ///Obtain the version history
  static const std::vector<std::string> GetVersionHistory() noexcept;

  void Undo();

  ///Emitted when the ConceptMap is modified as a whole: deleted, created or overwritten
  boost::signals2::signal<void()> m_signal_concept_map_changed;

  private:

  boost::shared_ptr<ConceptMap> m_conceptmap;

  ///The element in focus, if any. This might be:
  ///- a true Node
  ///- the label in the middle of an edge
  ///- the CenterNode
  ///nullptr denotes no Node has focus
  Node * m_focus;

  ///The undo stack (use std::vector because it is a true STL container)
  ///The Commands aren't const, because Command::Undo changes their state
  std::vector<boost::shared_ptr<Command>> m_undo;

  ///Creates a new Node in the concept map. The return value is
  ///that node. This is used by CommandCreateNode::Undo
  const boost::shared_ptr<Node> CreateNewNode() noexcept;

  ///Delete a Node in the concept map
  void DeleteNode(const boost::shared_ptr<Node> node) noexcept;

  const Node * GetFocus() const noexcept { return m_focus; }

  ///Start, reset or delete a/the concept map
  void SetConceptMap(const boost::shared_ptr<ConceptMap> conceptmap) noexcept;

  void SetFocus(Node * const node) noexcept { m_focus = node; }

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif

  //friend class Command;
  friend class CommandCreateNewConceptMap;
  friend class CommandCreateNewNode;
  friend class CommandDeleteConceptMap;
  friend class CommandDeleteNode;
  friend class CommandLoseFocus;
  friend bool operator==(const Widget& lhs, const Widget& rhs);

};

bool operator==(const Widget& lhs, const Widget& rhs);
bool operator!=(const Widget& lhs, const Widget& rhs);

//std::ostream& operator<<(std::ostream& os, const Widget& w);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPWIDGET_H
