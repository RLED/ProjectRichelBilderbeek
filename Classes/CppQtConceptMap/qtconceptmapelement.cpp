//---------------------------------------------------------------------------
/*
QtConceptMap, Qt classes for display and interaction with ConceptMap
Copyright (C) 2013-2014 The Brainweaver Team

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppQtConceptMap.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmapelement.h"

#include <QCursor>

#include "conceptmapconcept.h"
#include "conceptmapexamples.h"
#include "qtitemdisplaystrategy.h"
#include "trace.h"
#pragma GCC diagnostic pop

void ribi::cmap::QtConceptMapElement::OnConceptRequestsEdit()
{
  assert(this);
  assert(this->GetConcept());
  assert(this->GetConcept()->GetExamples());
  this->setAcceptHoverEvents(true);
  m_signal_conceptmapitem_requests_edit(this);
}

void ribi::cmap::QtConceptMapElement::hoverMoveEvent(QGraphicsSceneHoverEvent *)
{
  this->setCursor(QCursor(Qt::PointingHandCursor));
}
