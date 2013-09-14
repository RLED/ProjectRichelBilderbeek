#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "qtpvdbdisplayconceptitem.h"

#include <cstdlib>
#include <sstream>

#include <QKeyEvent>

#include "pvdbexample.h"
#include "pvdbexamples.h"
#include "pvdbhelper.h"
#include "pvdbconcept.h"
#include "pvdbcompetency.h"
#include "qtpvdbbrushfactory.h"
#include "trace.h"
#pragma GCC diagnostic pop

ribi::pvdb::QtPvdbDisplayConceptItem::QtPvdbDisplayConceptItem(const boost::shared_ptr<ribi::pvdb::Concept>& concept)
  : QtPvdbConceptItem(concept)
{
  #ifndef NDEBUG
  Test();
  assert(concept);
  assert(GetConcept());
  #endif

  UpdateBrushesAndPens();

  //?FIX 2013-01-06 22:47
  GetConcept()->m_signal_name_changed.connect(
    boost::bind(&ribi::pvdb::QtPvdbDisplayConceptItem::OnConceptNameChanged,this)); //Obligatory

  GetConcept()->m_signal_examples_changed.connect( //FIX 2013-01-06 22:32
      boost::bind(
        &ribi::pvdb::QtPvdbDisplayConceptItem::UpdateBrushesAndPens,
        this
      )
    );

  GetConcept()->m_signal_rating_complexity_changed.connect(
      boost::bind(
        &ribi::pvdb::QtPvdbDisplayConceptItem::UpdateBrushesAndPens,
        this
      )
    );
  GetConcept()->m_signal_rating_concreteness_changed.connect(
      boost::bind(
        &ribi::pvdb::QtPvdbDisplayConceptItem::UpdateBrushesAndPens,
        this
      )
    );
  GetConcept()->m_signal_rating_specificity_changed.connect(
      boost::bind(
        &ribi::pvdb::QtPvdbDisplayConceptItem::UpdateBrushesAndPens,
        this
      )
    );
}

ribi::pvdb::QtPvdbDisplayConceptItem::~QtPvdbDisplayConceptItem()
{
  GetConcept()->m_signal_examples_changed.disconnect(
      boost::bind(
        &ribi::pvdb::QtPvdbDisplayConceptItem::UpdateBrushesAndPens,
        this
      )
    );

  //Obligatory: because concepts live longer than DisplayConceptItems,
  //these Concepts will signal Items when the Item is destroyed,
  //which results in a segmentation fault
  GetConcept()->m_signal_rating_complexity_changed.disconnect(
      boost::bind(
        &ribi::pvdb::QtPvdbDisplayConceptItem::UpdateBrushesAndPens,
        this
      )
    );
  //2013-08-25
  GetConcept()->m_signal_rating_concreteness_changed.disconnect(
      boost::bind(
        &ribi::pvdb::QtPvdbDisplayConceptItem::UpdateBrushesAndPens,
        this
      )
    );
  //2013-08-25
  GetConcept()->m_signal_rating_specificity_changed.disconnect(
      boost::bind(
        &ribi::pvdb::QtPvdbDisplayConceptItem::UpdateBrushesAndPens,
        this
      )
    );
}

void ribi::pvdb::QtPvdbDisplayConceptItem::UpdateBrushesAndPens()
{
  //TRACE("Start of void ribi::pvdb::QtPvdbDisplayConceptItem::UpdateBrushesAndPens()");
  assert(GetConcept());
  assert(GetConcept()->GetExamples());

  //Brush for the concept being rated
  QBrush new_main_brush = this->brush();
  {
    const int n_rated
      = (GetConcept()->GetRatingComplexity()   != -1 ? 1 : 0)
      + (GetConcept()->GetRatingConcreteness() != -1 ? 1 : 0)
      + (GetConcept()->GetRatingSpecificity()  != -1 ? 1 : 0);
    switch (n_rated)
    {
      case 0:
        new_main_brush = QtPvdbBrushFactory::CreateRedGradientBrush();
        break;
      case 1:
      case 2:
        new_main_brush = QtPvdbBrushFactory::CreateYellowGradientBrush();
        break;
      case 3:
        new_main_brush = QtPvdbBrushFactory::CreateGreenGradientBrush();
        break;
      default: assert(!"Should not get here");
    }
  }
  //Brush and pen for the examples being rated
  QBrush new_indicator_brush = this->GetIndicatorBrush();
  QPen new_indicator_pen = this->GetIndicatorPen();
  if (GetConcept()->GetExamples()->Get().empty())
  {
    //No examples
    new_indicator_brush = QBrush(QColor(0,0,0));
    new_indicator_pen = QPen(QColor(0,0,0));
  }
  else
  {
    const std::vector<boost::shared_ptr<const pvdb::Example> > v = AddConst(GetConcept()->GetExamples()->Get());
    const int n_examples = boost::numeric_cast<int>(v.size());
    const int n_judged
      = std::count_if(v.begin(),v.end(),
        [](const boost::shared_ptr<const pvdb::Example>& p)
        {
          assert(p);
          const pvdb::Competency this_competency = p->GetCompetency();
          return this_competency != pvdb::Competency::uninitialized;
        }
      );
    if (n_judged == 0)
    {
      new_indicator_brush = QBrush(QColor(255,128,128)); //Red
    }
    else if (n_judged < n_examples)
    {
      new_indicator_brush = QBrush(QColor(255,196,128)); //Orange
    }
    else
    {
      assert(n_judged == n_examples);
      new_indicator_brush = QBrush(QColor(128,255,128)); //Green
    }
    if (n_judged == 0)
    {
      new_indicator_pen = QPen(QColor(255,0,0),3); //Thick pen
    }
    else if (n_judged < n_examples)
    {
      new_indicator_pen = QPen(QColor(255,196,0),2); //Less thick pen
    }
    else
    {
      assert(n_judged == n_examples);
      new_indicator_pen = QPen(QColor(0,255,0),1); //Thin pen
    }
  }
  if (this->brush() != new_main_brush
    || this->GetIndicatorBrush() != new_indicator_brush
    || this->GetIndicatorPen() != new_indicator_pen)
  {
    this->setBrush(new_main_brush);
    this->SetIndicatorBrush(new_indicator_brush);
    this->SetIndicatorPen(new_indicator_pen);
    assert(this->brush() == new_main_brush);
    assert(this->GetIndicatorBrush() == new_indicator_brush);
    assert(this->GetIndicatorPen() == new_indicator_pen);
    //TRACE(std::rand()); //GOOD: Detects infinite recursion
    //this->update();
    this->m_signal_item_has_updated(this); //Obligatory
    this->m_signal_request_scene_update(); //Obligatory
  }
  //TRACE("End of void ribi::pvdb::QtPvdbDisplayConceptItem::UpdateBrushesAndPens()");
}
