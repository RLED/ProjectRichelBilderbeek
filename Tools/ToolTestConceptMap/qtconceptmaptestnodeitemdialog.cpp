#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmaptestnodeitemdialog.h"

#include <cassert>

#ifdef COMPILER_SUPPORTS_THREADS_20130507
#include <thread>
#endif

#include <boost/lexical_cast.hpp>
#include <QKeyEvent>
#include "qtconceptmapdisplaystrategy.h"
#include "qtconceptmapeditstrategy.h"
#include "qtconceptmapratestrategy.h"
#include "conceptmapnodefactory.h"
#include "qtconceptmapdisplaystrategy.h"
#include "qtconceptmapbrushfactory.h"
#include "qtconceptmapnode.h"
#include "conceptmapexamples.h"
#include "conceptmapexample.h"
#include "ui_qtconceptmaptestnodeitemdialog.h"
#include "conceptmapconcept.h"
#include "conceptmapnode.h"
#include "trace.h"
#pragma GCC diagnostic pop

ribi::cmap::QtConceptMapTestNodeItemDialog::QtConceptMapTestNodeItemDialog(QWidget *parent) :
  QtHideAndShowDialog(parent),
  ui(new Ui::QtConceptMapTestNodeItemDialog),
  m_node(cmap::NodeFactory().GetTests().at(1)),
  m_display_node(nullptr),
  m_edit_node(nullptr),
  m_rate_node(nullptr)
{
  ui->setupUi(this);
  #ifndef NDEBUG
  Test();
  #endif

  assert(ui->view->scene());

  //Node is used in: m_node
  assert(m_node.use_count() == 1);
  assert(m_node->GetConcept().use_count() == 1);

  {
    const boost::shared_ptr<QtDisplayStrategy> display_strategy(new QtDisplayStrategy(m_node->GetConcept()));
    m_display_node = new cmap::QtNode(m_node,display_strategy);
    m_display_node->m_signal_request_scene_update.connect(
      boost::bind(
        &ribi::cmap::QtConceptMapTestNodeItemDialog::OnRequestsSceneUpdate,
        this));
  }

  //Node is used in: m_node and QtConceptMapNodeConcept::m_node
  assert(m_node.use_count() == 2);
  //Concept is used in: m_node::m_concept, QtDisplayStrategy::m_node::m_concept and QtConceptMapConcept::m_concept
  assert(m_node.get() == m_display_node->GetNode().get());
  assert(m_node->GetConcept().get() == m_display_node->GetNode()->GetConcept().get());
  assert(m_node->GetConcept().use_count() == 2);
  //assert(m_node->GetConceptUseCount() == 2);

  {
    const boost::shared_ptr<QtEditStrategy> item(new QtEditStrategy(m_node->GetConcept()));
    m_edit_node = new cmap::QtNode(m_node,item);
    m_edit_node->m_signal_request_scene_update.connect(
      boost::bind(
        &ribi::cmap::QtConceptMapTestNodeItemDialog::OnRequestsSceneUpdate,
        this));
  }

  assert(m_node.use_count() == 3);
  //assert(m_node->GetConceptUseCount() == 3);
  assert(m_node->GetConcept().use_count() == 3);

  {
    const boost::shared_ptr<QtRateStrategy> item(new QtRateStrategy(m_node->GetConcept()));
    m_rate_node = new cmap::QtNode(m_node,item);
    m_rate_node->m_signal_request_scene_update.connect(
      boost::bind(
        &ribi::cmap::QtConceptMapTestNodeItemDialog::OnRequestsSceneUpdate,
        this));
  }

  assert(m_node.use_count() == 4);
  //assert(m_node->GetConceptUseCount() == 4);
  assert(m_node->GetConcept().use_count() == 4);

  assert(m_display_node->GetNode().get() == m_node.get());
  assert(m_edit_node->GetNode().get()    == m_node.get());
  assert(m_rate_node->GetNode().get()    == m_node.get());

  assert(!m_display_node->scene());
  assert(!m_edit_node->scene());
  assert(!m_rate_node->scene());
  ui->view->scene()->addItem(m_display_node);
  ui->view->scene()->addItem(m_edit_node);
  ui->view->scene()->addItem(m_rate_node);

  assert(ui->view->scene()->items().size() == 3);
  assert(dynamic_cast<cmap::QtNode*>(ui->view->scene()->items()[0]));
  assert(dynamic_cast<cmap::QtNode*>(ui->view->scene()->items()[1]));
  assert(dynamic_cast<cmap::QtNode*>(ui->view->scene()->items()[2]));

  //QtConceptMapConceptItems are aware of their surroundings, but I put them into place manually
  m_display_node->SetPos(0.0,-40.0);
  m_edit_node->SetPos(   0.0,  0.0);
  m_rate_node->SetPos(   0.0, 40.0);

  {
    const std::vector<cmap::Competency> v = cmap::GetAllCompetencies();
    const int sz = boost::numeric_cast<int>(v.size());
    for (int i=0; i!=sz; ++i)
    {
      ui->box_competency->addItem(cmap::CompetencyToDutchStr(v[i]).c_str());
    }
  }

  assert(this->GetNode()->GetConcept()->GetExamples());
  assert(this->GetNode()->GetConcept()->GetExamples()->Get().size() == 1);

  ui->box_complexity->setCurrentIndex(this->GetNode()->GetConcept()->GetRatingComplexity() + 1);
  ui->box_concreteness->setCurrentIndex(this->GetNode()->GetConcept()->GetRatingConcreteness() + 1);
  ui->box_specificity->setCurrentIndex(this->GetNode()->GetConcept()->GetRatingSpecificity() + 1);

  ui->edit_name->setText(this->GetNode()->GetConcept()->GetName().c_str());
  ui->edit_example_text->setText(this->GetNode()->GetConcept()->GetExamples()->Get().at(0)->GetText().c_str());


  ui->box_competency->setCurrentIndex(static_cast<int>(this->GetNode()->GetConcept()->GetExamples()->Get().at(0)->GetCompetency()));
}

ribi::cmap::QtConceptMapTestNodeItemDialog::~QtConceptMapTestNodeItemDialog() noexcept
{
  delete ui;
}

const boost::shared_ptr<ribi::cmap::Node> ribi::cmap::QtConceptMapTestNodeItemDialog::GetNode()
{
  switch(ui->box_edit->currentIndex())
  {
    case 0: return m_node;
    case 1:
    {
      const auto v = ui->view->scene()->items();
      const auto iter = std::find_if(v.begin(),v.end(),
        [](const QGraphicsItem * const item)
        {
          const cmap::QtNode* const node_item = dynamic_cast<const cmap::QtNode*>(item);
          assert(node_item);
          return dynamic_cast<const QtDisplayStrategy*>(node_item->GetDisplayStrategy().get());
        }
      );
      assert(iter!=v.end());
      cmap::QtNode * const qtnode = dynamic_cast<cmap::QtNode*>(*iter);
      assert(qtnode);
      return qtnode->GetNode();
    }
    case 2:
    {
      const auto v = ui->view->scene()->items();
      const auto iter = std::find_if(v.begin(),v.end(),
        [](const QGraphicsItem * const item)
        {
          const cmap::QtNode* const node_item = dynamic_cast<const cmap::QtNode*>(item);
          assert(node_item);
          return dynamic_cast<const QtEditStrategy*>(node_item->GetDisplayStrategy().get());
        }
      );
      assert(iter!=v.end());
      cmap::QtNode * const qtnode = dynamic_cast<cmap::QtNode*>(*iter);
      assert(qtnode);
      return qtnode->GetNode();
    }
    case 3:
    {
      const auto v = ui->view->scene()->items();
      const auto iter = std::find_if(v.begin(),v.end(),
        [](const QGraphicsItem * const item)
        {
          const cmap::QtNode* const node_item = dynamic_cast<const cmap::QtNode*>(item);
          assert(node_item);
          return dynamic_cast<const QtRateStrategy*>(node_item->GetDisplayStrategy().get());
        }
      );
      assert(iter!=v.end());
      cmap::QtNode * const qtnode = dynamic_cast<cmap::QtNode*>(*iter);
      assert(qtnode);
      return qtnode->GetNode();
    }
    default:
      assert(!"Should not get here");
  }
  assert(!"Should not get here");
  throw std::logic_error("ribi::cmap::QtConceptMapTestNodeItemDialog::GetNode: index unknown");
}

void ribi::cmap::QtConceptMapTestNodeItemDialog::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Escape) { close(); return; }
}

void ribi::cmap::QtConceptMapTestNodeItemDialog::on_box_competency_currentIndexChanged(int index)
{
  const cmap::Competency c = static_cast<cmap::Competency>(index);
  this->GetNode()->GetConcept()->GetExamples()->Get().at(0)->SetCompetency(c);
}

void ribi::cmap::QtConceptMapTestNodeItemDialog::on_edit_name_textChanged(const QString &arg1)
{
  this->GetNode()->GetConcept()->SetName(arg1.toStdString());
}

void ribi::cmap::QtConceptMapTestNodeItemDialog::on_box_complexity_currentIndexChanged(const QString &arg1)
{
  const int rating_complexity = boost::lexical_cast<int>(arg1.toStdString());
  assert(rating_complexity >= -1);
  assert(rating_complexity <=  2);
  this->GetNode()->GetConcept()->SetRatingComplexity(rating_complexity);
}

void ribi::cmap::QtConceptMapTestNodeItemDialog::on_box_concreteness_currentIndexChanged(const QString &arg1)
{
  this->GetNode()->GetConcept()->SetRatingConcreteness(boost::lexical_cast<int>(arg1.toStdString()));
}

void ribi::cmap::QtConceptMapTestNodeItemDialog::on_box_specificity_currentIndexChanged(const QString &arg1)
{
  this->GetNode()->GetConcept()->SetRatingSpecificity(boost::lexical_cast<int>(arg1.toStdString()));
}

void ribi::cmap::QtConceptMapTestNodeItemDialog::on_edit_example_text_textChanged(const QString &arg1)
{
  this->GetNode()->GetConcept()->GetExamples()->Get().at(0)->SetText(arg1.toStdString());
}

#ifndef NDEBUG
void ribi::cmap::QtConceptMapTestNodeItemDialog::Test() noexcept
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
  TRACE("ribi::cmap::QtConceptMapTestNodeItemDialog::Test started");
  QtConceptMapTestNodeItemDialog d;
  assert(d.m_node.get() == d.m_display_node->GetNode().get());
  assert(d.m_node.get() == d.m_edit_node->GetNode().get());
  assert(d.m_node.get() == d.m_rate_node->GetNode().get());
  assert(d.m_node->GetConcept().get() == d.m_display_node->GetNode()->GetConcept().get());
  assert(d.m_node->GetConcept().get() == d.m_edit_node->GetNode()->GetConcept().get());
  assert(d.m_node->GetConcept().get() == d.m_rate_node->GetNode()->GetConcept().get());

  //Test resizing due to text being changed
  {
    TRACE("TODO");
    /*
    const std::string s = d.m_node->GetConcept()->GetName();
    assert(d.m_display_node->boundingRect().width() == d.m_edit_node->boundingRect().width());
    assert(d.m_display_node->boundingRect().width() == d.m_rate_node->boundingRect().width());
    const double w = d.m_display_node->boundingRect().width();
    d.m_node->GetConcept()->SetName(s + "*");
    assert(d.m_display_node->boundingRect().width() > w);
    assert(d.m_edit_node->boundingRect().width() > w);
    assert(d.m_rate_node->boundingRect().width() > w);
    */
  }
  //Test resizing due to shorter text being set
  {
    TRACE("TODO");
    /*
    d.m_node->GetConcept()->SetName("1234567890");
    const double w = d.m_display_node->boundingRect().width();
    d.m_node->GetConcept()->SetName("123456789");
    assert(d.m_display_node->boundingRect().width() < w);
    */
  }
  //Test brushes being changed when ratings are given
  {
    TRACE("TODO?");
    /*
    const boost::shared_ptr<Concept> concept = d.m_node->GetConcept();
    concept->SetRatingComplexity(-1);
    concept->SetRatingConcreteness(-1);
    concept->SetRatingSpecificity(-1);
    QtRateStrategy item(concept);
    assert(d.m_edit_node->brush()    == QtConceptMapBrushFactory::CreateGrayGradientBrush());
    assert(d.m_display_node->brush() == QtConceptMapBrushFactory::CreateRedGradientBrush());
    assert(d.m_rate_node->brush()    == QtConceptMapBrushFactory::CreateRedGradientBrush());

    concept->SetRatingComplexity(0);
    concept->SetRatingConcreteness(1);

    assert(d.m_edit_node->brush()     == QtConceptMapBrushFactory::CreateGrayGradientBrush());
    assert(d.m_display_node->brush()  == QtConceptMapBrushFactory::CreateYellowGradientBrush());
    assert(d.m_rate_node->brush()     == QtConceptMapBrushFactory::CreateYellowGradientBrush());

    concept->SetRatingSpecificity(2);

    assert(d.m_edit_node->brush()    == QtConceptMapBrushFactory::CreateGrayGradientBrush());
    assert(d.m_display_node->brush() == QtConceptMapBrushFactory::CreateGreenGradientBrush());
    assert(d.m_rate_node->brush()    == QtConceptMapBrushFactory::CreateGreenGradientBrush());
    */
  }

  TRACE("ribi::cmap::QtConceptMapTestNodeItemDialog::Test finished successfully");
  #ifdef COMPILER_SUPPORTS_THREADS_20130507
    }
  );
  t.detach();
  #endif
}
#endif

void ribi::cmap::QtConceptMapTestNodeItemDialog::OnRequestsSceneUpdate()
{
  this->ui->view->scene()->update();
}
