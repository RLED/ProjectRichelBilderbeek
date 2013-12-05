#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "qttestconceptmapmenudialog.h"

#include "qtaboutdialog.h"
#include "testconceptmapmenudialog.h"
#include "qttestconceptmapdisplaywidgetdialog.h"
#include "qttestconceptmapeditwidgetdialog.h"
#include "qttestconceptmapratewidgetdialog.h"
#include "qtconceptmapviewtestsdialog.h"
#include "qtconceptmaptestconceptitemdialog.h"
#include "qtconceptmaptestedgeitemdialog.h"
#include "qtconceptmaptestnodeitemdialog.h"
#include "ui_qttestconceptmapmenudialog.h"
#include "trace.h"
#pragma GCC diagnostic pop

ribi::cmap::QtTestConceptMapMenuDialog::QtTestConceptMapMenuDialog(QWidget *parent) :
    QtHideAndShowDialog(parent),
    ui(new Ui::QtTestConceptMapMenuDialog)
{
  #ifndef NDEBUG
  Test();
  #endif
  ui->setupUi(this);
}

ribi::cmap::QtTestConceptMapMenuDialog::~QtTestConceptMapMenuDialog() noexcept
{
  delete ui;
}

void ribi::cmap::QtTestConceptMapMenuDialog::on_button_about_clicked()
{
  QtAboutDialog d(TestConceptMapMenuDialog().GetAbout());
  this->ShowChild(&d);
}

void ribi::cmap::QtTestConceptMapMenuDialog::on_button_quit_clicked()
{
  this->close();
}

#ifndef NDEBUG
void ribi::cmap::QtTestConceptMapMenuDialog::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting ribi::cmap::QtTestConceptMapMenuDialog::Test");
  QtTestConceptMapDisplayWidgetDialog();
  QtTestConceptMapEditWidgetDialog();
  QtTestConceptMapRateWidgetDialog();
  TRACE("Finished ribi::cmap::QtTestConceptMapMenuDialog::Test successfully");
}
#endif

void ribi::cmap::QtTestConceptMapMenuDialog::on_button_readonly_conceptmap_clicked()
{
  QtTestConceptMapDisplayWidgetDialog d;
  this->ShowChild(&d);
}

void ribi::cmap::QtTestConceptMapMenuDialog::on_button_edit_conceptmap_clicked()
{
  QtTestConceptMapEditWidgetDialog d;
  this->ShowChild(&d);
}

void ribi::cmap::QtTestConceptMapMenuDialog::on_button_rate_conceptmap_clicked()
{
  QtTestConceptMapRateWidgetDialog d;
  this->ShowChild(&d);
}

void ribi::cmap::QtTestConceptMapMenuDialog::on_button_concept_clicked()
{
  QtConceptMapTestConceptItemDialog d;
  this->ShowChild(&d);
}

void ribi::cmap::QtTestConceptMapMenuDialog::on_button_node_clicked()
{
  QtConceptMapTestNodeItemDialog d;
  this->ShowChild(&d);
}

void ribi::cmap::QtTestConceptMapMenuDialog::on_button_edge_clicked()
{
  QtConceptMapTestEdgeItemDialog d;
  this->ShowChild(&d);
}

void ribi::cmap::QtTestConceptMapMenuDialog::on_button_view_concept_maps_clicked()
{
  QtConceptMapViewTestsDialog d;
  this->ShowChild(&d);
}