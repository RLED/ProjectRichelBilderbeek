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
#include "qtrateconceptmap.h"

#ifdef COMPILER_SUPPORTS_THREADS_20130507
#include <thread>
#endif

#include <QGraphicsItem>

#include "conceptmapfactory.h"
#include "trace.h"
#pragma GCC diagnostic pop

#ifndef NDEBUG
void ribi::cmap::QtRateConceptMap::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  #ifdef COMPILER_SUPPORTS_THREADS_20130507
  std::thread t(
    []
    {
  #endif
  //Test base class
  {
    const auto v = ribi::cmap::ConceptMapFactory::GetAllTests();
    for (const boost::shared_ptr<ConceptMap> concept_map: v)
    {
      boost::shared_ptr<QtConceptMap> widget(new This_t(concept_map));
      assert(widget);
      QtConceptMap::Test(widget);
    }
  }
  /*
  {
    const auto v = ribi::cmap::ConceptMapFactory::GetAllTests();
    for (const boost::shared_ptr<ConceptMap> concept_map: v)
    {
      boost::shared_ptr<QtConceptMap> widget(new This_t(concept_map));
      assert(widget);
      for(QGraphicsItem * const item: widget->items())
      {
        assert(item);
        item->setFocus();
      }
    }
  }
  */
  TRACE("ribi::cmap::QtConceptMapRateWidget::Test finished successfully");
  #ifdef COMPILER_SUPPORTS_THREADS_20130507
    }
  );
  t.detach();
  #endif
}
#endif

