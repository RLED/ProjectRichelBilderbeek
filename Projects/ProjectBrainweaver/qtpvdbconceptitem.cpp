#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "qtpvdbconceptitem.h"

#include <string>
#include <boost/lexical_cast.hpp>

#include <QCursor>
#include <QFont>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPainterPathStroker>

#include "fuzzy_equal_to.h"
#include "pvdbconceptfactory.h"
#include "pvdbconcept.h"
#include "pvdbconcept.h"
#include "pvdbedge.h"
#include "pvdbexample.h"
#include "pvdbexamples.h"
#include "pvdbhelper.h"
#include "pvdbnodefactory.h"
#include "pvdbnode.h"
#include "qtpvdbexamplesitem.h"
#include "trace.h"
#pragma GCC diagnostic pop

ribi::pvdb::QtPvdbConceptItem::QtPvdbConceptItem(
  const boost::shared_ptr<ribi::pvdb::Concept>& concept)
  : m_signal_position_changed{},
    m_concept(concept),
    m_indicator_brush(QBrush(QColor(0,0,0,0))),
    m_indicator_pen(QPen(QColor(0,0,0)))
{
  #ifndef NDEBUG
  Test();
  #endif

  assert(m_concept);
  assert(m_concept->GetExamples());

  this->setFlags(
      QGraphicsItem::ItemIsFocusable
    | QGraphicsItem::ItemIsSelectable);

  //Allow mouse tracking
  this->setAcceptHoverEvents(true);

  this->SetPadding(Base::Padding(1.0,6.0,1.0,2.0));

  //?FIX 2013-01-06 22:47
  GetConcept()->m_signal_name_changed.connect(
    boost::bind(&ribi::pvdb::QtPvdbConceptItem::OnConceptNameChanged,this)); //Obligatory

  //FIX? 2013-06-25
  //this->SetText("DUMMY TEXT");
  //assert(this->GetText() != m_concept->GetName()
  //  && "The text must be set, to get a resize");

  this->SetName(m_concept->GetName());
}

const boost::shared_ptr<const ribi::pvdb::Concept> ribi::pvdb::QtPvdbConceptItem::GetConcept() const
{
  boost::shared_ptr<const ribi::pvdb::Concept> p(m_concept);
  assert(p);
  return p;
}

const std::string ribi::pvdb::QtPvdbConceptItem::GetName() const
{
  return Unwordwrap(this->GetText());
}

void ribi::pvdb::QtPvdbConceptItem::hoverStartEvent(QGraphicsSceneHoverEvent *)
{
  std::exit(1); //Never called
  this->setCursor(QCursor(Qt::PointingHandCursor));
}

void ribi::pvdb::QtPvdbConceptItem::hoverMoveEvent(QGraphicsSceneHoverEvent *)
{
  //std::exit(2);
  this->setCursor(QCursor(Qt::PointingHandCursor));
}

void ribi::pvdb::QtPvdbConceptItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  Base::mouseMoveEvent(event);
  //this->m_signal_item_has_updated(); //2013-06-25
  m_signal_request_scene_update();
}

void ribi::pvdb::QtPvdbConceptItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  Base::mousePressEvent(event);
  //this->m_signal_item_has_updated(); //2013-06-25
  m_signal_request_scene_update();
}

void ribi::pvdb::QtPvdbConceptItem::OnConceptNameChanged()
{
  assert(this);
  assert(m_concept);
  assert(m_concept->GetExamples());
  this->SetName(m_concept->GetName());
  //m_signal_item_changed(this); //Called by SetText
}

void ribi::pvdb::QtPvdbConceptItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  assert(m_concept);
  assert(m_concept->GetExamples());
  //assert(!this->hasFocus()); //Can have focus when used as stand-alone
  //assert(!this->isSelected()); //Can have focus when used as stand-alone
  this->UpdateBrushesAndPens();

  Base::paint(painter,option,widget);

  if (!m_concept->GetExamples()->Get().empty())
  {
    painter->setBrush(m_indicator_brush);
    painter->setPen(m_indicator_pen);
    //Draw indicator that a concept has examples in it
    painter->drawRect(
      rect().right() - 5.0,
      rect().top() + 3.0,
      3.0,
      3.0
      );
  }
}

void ribi::pvdb::QtPvdbConceptItem::SetIndicatorBrush(const QBrush& brush)
{
  if (m_indicator_brush != brush)
  {
    m_indicator_brush = brush;
    this->update();
  }
}

void ribi::pvdb::QtPvdbConceptItem::SetIndicatorPen(const QPen& pen)
{
  if (m_indicator_pen != pen)
  {
    m_indicator_pen = pen;
    this->update();
  }
}

void ribi::pvdb::QtPvdbConceptItem::SetMainBrush(const QBrush& any_brush)
{

  if (this->brush() != any_brush)
  {
    this->setBrush(any_brush);
    this->update();
  }
  assert(this->brush() == any_brush);
}

void ribi::pvdb::QtPvdbConceptItem::SetPos(const double x, const double y)
{
  if (x != this->x() || y != this->y())
  {
    this->setPos(x,y);
    this->m_signal_position_changed(x,y);
  }
}

void ribi::pvdb::QtPvdbConceptItem::SetName(const std::string& s)
{
  //Set the text
  const std::vector<std::string> v { Wordwrap(s,40) };
  this->SetText(v);

  #ifndef NDEBUG
  if (this->GetName() != s)
  {
    TRACE(this->GetName());
  }
  #endif
  assert(this->GetName() == s);
}

/*
QPainterPath ribi::pvdb::QtPvdbConceptItem::shape() const
{
  const int click_easy_width = 5;
  QPainterPath path;
  QPainterPathStroker stroker;

  path.addRect(this->boundingRect());
  stroker.setWidth(click_easy_width);
  return stroker.createStroke(path);
}
*/

#ifndef NDEBUG
void ribi::pvdb::QtPvdbConceptItem::Test()
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting ribi::pvdb::QtPvdbConceptItem::Test()");

  TRACE("Successfully finished ribi::pvdb::QtPvdbConceptItem::Test()");
}
#endif
