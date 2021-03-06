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
#ifndef CONCEPTMAPCOMMANDDELETECONCEPTMAP_H
#define CONCEPTMAPCOMMANDDELETECONCEPTMAP_H

/*

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "conceptmapcommand.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Delete a concept map
///-Can be used only when there is an existing concept map
struct CommandDeleteConceptMap : public Command
{
  CommandDeleteConceptMap() : m_deleted_concept_map{}, m_widget{} {}
  CommandDeleteConceptMap(const CommandDeleteConceptMap&) = delete;
  CommandDeleteConceptMap& operator=(const CommandDeleteConceptMap&) = delete;
  ~CommandDeleteConceptMap() noexcept {}
  bool CanDoCommandSpecific(const Widget * const widget) const noexcept;
  void DoCommandSpecific(Widget * const widget) noexcept;
  std::string ToStr() const noexcept { return "delete concept map"; }
  void Undo() noexcept;

  private:
  boost::shared_ptr<ConceptMap> m_deleted_concept_map;
  Widget * m_widget;
};

} //~namespace cmap
} //~namespace ribi

*/

#endif // CONCEPTMAPCOMMANDDELETECONCEPTMAP_H
