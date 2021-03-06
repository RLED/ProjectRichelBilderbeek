#ifdef _WIN32
//See http://www.richelbilderbeek.nl/CppCompileErrorSwprintfHasNotBeenDeclared.htm
#undef __STRICT_ANSI__
#endif

//#include own header file as first substantive line of code, from:
// * John Lakos. Large-Scale C++ Software Design. 1996. ISBN: 0-201-63362-0. Section 3.2, page 110
#include "qtpvdbrateexamplesdialog.h"

#include <QKeyEvent>
#include <QListWidgetItem>

#include <pvdbexamplefactory.h>
#include <pvdbexamplesfactory.h>
#include "pvdbconcept.h"
#include "qtpvdbcompetency.h"
#include "ui_qtpvdbrateexamplesdialog.h"
#include "pvdbconceptfactory.h"
#include "trace.h"

struct QtPvdbListWidgetItem : public QListWidgetItem
{
  QtPvdbListWidgetItem(const boost::shared_ptr<const pvdb::Example>& example)
  {
    this->setText(example->GetText().c_str());
    this->setIcon(pvdb::QtCompetency::CompetencyToIcon(example->GetCompetency()));
  }
};


QtPvdbRateExamplesDialog::QtPvdbRateExamplesDialog(
  const boost::shared_ptr<pvdb::Concept> concept,
  QWidget* parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtPvdbRateExamplesDialog),
    m_concept(concept)
{
  ui->setupUi(this);
  #ifndef NDEBUG
  Test();
  assert(m_concept);
  assert(m_concept->GetExamples());
  #endif

  //Convert the Concept to GUI elements
  ui->label_concept_name->setText(concept->GetName().c_str());
  ui->list->clear();
  const auto v = concept->GetExamples()->Get();
  std::for_each(v.begin(),v.end(),
    [this](const boost::shared_ptr<const pvdb::Example>& example)
    {
      QtPvdbListWidgetItem * const item
        = new QtPvdbListWidgetItem(example);
      ui->list->addItem(item);
    }
  );

  //Align the button icons and texts to the left
  {
    const std::vector<QPushButton*> v
      =
      {
        ui->button_misc,
        ui->button_organisations,
        ui->button_prof,
        ui->button_prof_development,
        ui->button_social,
        ui->button_target_audience,
        ui->button_ti_knowledge
      };
    for(QPushButton* button: v) button->setStyleSheet("text-align: left;");
  }
}

QtPvdbRateExamplesDialog::~QtPvdbRateExamplesDialog()
{
  delete ui;
}

const boost::shared_ptr<pvdb::Examples> QtPvdbRateExamplesDialog::GetRatedExamples() const
{
  std::vector<boost::shared_ptr<pvdb::Example> > v;

  const int sz = ui->list->count();
  for (int i=0; i!=sz; ++i)
  {
    const QListWidgetItem* const qtitem = ui->list->item(i);
    assert(qtitem);
    const QtPvdbListWidgetItem* const item = dynamic_cast<const QtPvdbListWidgetItem*>(qtitem);
    assert(item);
    const boost::shared_ptr<pvdb::Example> example
      = pvdb::ExampleFactory::Create(
        item->text().toStdString(),
        pvdb::QtCompetency::IconToCompetency(item->icon())
      );
    assert(example);
    v.push_back(example);
  }

  const boost::shared_ptr<pvdb::Examples> examples
    = pvdb::ExamplesFactory::Create(v);
  assert(examples);
  return examples;
}

void QtPvdbRateExamplesDialog::keyPressEvent(QKeyEvent* e)
{
  if (e->key()  == Qt::Key_Escape) { close(); return; }
  QDialog::keyPressEvent(e);
}

void QtPvdbRateExamplesDialog::Test()
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("QtPvdbRateExamplesDialog::Test started");
  ///Test conversion between reading and writing a concept
  {
    const int sz = pvdb::ConceptFactory::GetTests().size();
    for (int i=0; i!=sz; ++i)
    {
      const auto a = QtPvdbRateExamplesDialog(pvdb::ConceptFactory::GetTests()[i]).GetRatedExamples();
      assert(a);
      for (int j=0; j!=sz; ++j)
      {
        const auto b = QtPvdbRateExamplesDialog(pvdb::ConceptFactory::GetTests()[j]).GetRatedExamples();
        assert(b);
        if (i == j)
        {
          assert(a == b);
        }
        else
        {
          assert(a != b);
        }
      }
    }
  }
  TRACE("QtPvdbRateExamplesDialog::Test finished successfully");
}

void QtPvdbRateExamplesDialog::on_button_prof_clicked()
{
  if (ui->list->currentItem())
  {
    QtPvdbListWidgetItem* const item = dynamic_cast<QtPvdbListWidgetItem*>(ui->list->currentItem());
    item->setIcon(pvdb::QtCompetency::CompetencyToIcon(pvdb::Competency::profession));
  }
}

void QtPvdbRateExamplesDialog::on_button_organisations_clicked()
{
  if (ui->list->currentItem())
  {
    QtPvdbListWidgetItem* const item = dynamic_cast<QtPvdbListWidgetItem*>(ui->list->currentItem());
    item->setIcon(pvdb::QtCompetency::CompetencyToIcon(pvdb::Competency::organisations));
  }
}

void QtPvdbRateExamplesDialog::on_button_social_clicked()
{
  if (ui->list->currentItem())
  {
    QtPvdbListWidgetItem* const item = dynamic_cast<QtPvdbListWidgetItem*>(ui->list->currentItem());
    item->setIcon(pvdb::QtCompetency::CompetencyToIcon(pvdb::Competency::social_surroundings));
  }
}

void QtPvdbRateExamplesDialog::on_button_target_audience_clicked()
{
  if (ui->list->currentItem())
  {
    QtPvdbListWidgetItem* const item = dynamic_cast<QtPvdbListWidgetItem*>(ui->list->currentItem());
    item->setIcon(pvdb::QtCompetency::CompetencyToIcon(pvdb::Competency::target_audience));
  }
}

void QtPvdbRateExamplesDialog::on_button_prof_development_clicked()
{
  if (ui->list->currentItem())
  {
    QtPvdbListWidgetItem* const item = dynamic_cast<QtPvdbListWidgetItem*>(ui->list->currentItem());
    item->setIcon(pvdb::QtCompetency::CompetencyToIcon(pvdb::Competency::prof_growth));
  }
}

void QtPvdbRateExamplesDialog::on_button_misc_clicked()
{
  if (ui->list->currentItem())
  {
    QtPvdbListWidgetItem* const item = dynamic_cast<QtPvdbListWidgetItem*>(ui->list->currentItem());
    item->setIcon(pvdb::QtCompetency::CompetencyToIcon(pvdb::Competency::misc));
  }
}

void QtPvdbRateExamplesDialog::on_button_ti_knowledge_clicked()
{
  if (ui->list->currentItem())
  {
    QtPvdbListWidgetItem* const item = dynamic_cast<QtPvdbListWidgetItem*>(ui->list->currentItem());
    item->setIcon(pvdb::QtCompetency::CompetencyToIcon(pvdb::Competency::ti_knowledge));
  }
}

void QtPvdbRateExamplesDialog::on_button_ok_clicked()
{
  const boost::shared_ptr<pvdb::Examples> p = GetRatedExamples();
  assert(p);
  assert(m_concept);
  m_concept->SetExamples(p);
  close();
}
