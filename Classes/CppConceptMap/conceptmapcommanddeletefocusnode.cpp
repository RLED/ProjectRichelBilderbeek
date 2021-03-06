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
#include "conceptmapcommanddeletefocusnode.h"

#include <cassert>

#include "conceptmapwidget.h"

bool ribi::cmap::CommandDeleteFocusNode::CanDoCommandSpecific(const Widget * const widget) const noexcept
{
  assert(widget);
  return widget->GetConceptMap().get() && widget->GetFocus();
}

void ribi::cmap::CommandDeleteFocusNode::DoCommandSpecific(Widget * const widget) noexcept
{
  assert(!m_widget);
  assert(widget);
  assert(widget->GetConceptMap().get());
  assert(CanDoCommand(widget));

  m_widget = widget;
  m_old_focus = m_widget->GetFocus();
  assert(m_old_focus);
  m_widget->DeleteNode(m_old_focus);
  assert(m_widget);
}

void ribi::cmap::CommandDeleteFocusNode::Undo() noexcept
{
  assert(m_widget);
  assert(m_widget->GetConceptMap().get());

  m_widget->AddNode(m_old_focus);

  m_widget = nullptr;
  m_old_focus = nullptr;

}
