#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <cassert>
#include <boost/lexical_cast.hpp>

#include <QDesktopWidget>
#include <QDoubleSpinBox>
#include <QGridLayout>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

#if QWT_VERSION >= 0x060100
#include <qwt_point_data.h>
#endif

//#include "fparser.hh"

#include "functionplottermaindialog.h"
#include "qtfunctionplottermaindialog.h"
#include "trace.h"
#include "ui_qtfunctionplottermaindialog.h"
#pragma GCC diagnostic pop

ribi::QtFunctionPlotterMainDialog::QtFunctionPlotterMainDialog(QWidget *parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtFunctionPlotterMainDialog),
    m_curve(new QwtPlotCurve),
    m_plot(new QwtPlot)
{
  #ifndef NDEBUG
  Test();
  #endif
  ui->setupUi(this);

  //Create plot
  {
    assert(!ui->plot_contents->layout());
    QGridLayout * const layout = new QGridLayout;
    ui->plot_contents->setLayout(layout);

    layout->addWidget(m_plot);

    #ifdef _WIN32
    m_plot->setCanvasBackground(QBrush(QColor(255,255,255)));
    #else
    m_plot->setCanvasBackground(QColor(255,255,255));
    #endif

    m_curve->attach(m_plot);
    m_curve->setStyle(QwtPlotCurve::Lines);
    m_curve->setPen(QPen(QColor(128,128,128)));
  }

  QObject::connect(
    this->ui->edit_equation,
    &QLineEdit::textChanged,
    this,
    &ribi::QtFunctionPlotterMainDialog::OnAnyChange
  );
  QObject::connect(
    this->ui->box_minx,
    SIGNAL(valueChanged(QString)),
    this,
    SLOT(OnAnyChange())
  );
  QObject::connect(
    this->ui->box_maxx,
    SIGNAL(valueChanged(QString)),
    this,
    SLOT(OnAnyChange())
  );


  ui->box_minx->setValue(-1.0);
  ui->box_maxx->setValue(1.0);

  ui->edit_equation->setText("cos(x)");


  {
    //Put the dialog in the screen center at 50% x 50% of its size
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->setGeometry(0,0,screen.width() / 2,screen.height() /2 );
    this->move( screen.center() - this->rect().center() );
  }
}

ribi::QtFunctionPlotterMainDialog::~QtFunctionPlotterMainDialog() noexcept
{
  delete ui;
}

void ribi::QtFunctionPlotterMainDialog::OnAnyChange() noexcept
{
  const std::string formula { ui->edit_equation->text().toStdString() };
  const double x_min = ui->box_minx->value();
  const double x_max = ui->box_maxx->value();
  const int n_cols = ui->area_plot->width();

  try
  {
    const FunctionPlotterMainDialog d(
      formula,
      x_min,
      x_max,
      n_cols
    );

    const std::vector<double> v_x { d.GetXs() };
    const std::vector<double> v_y { d.GetYs() };

    this->setWindowTitle("Function plotted successfully");

    //Plot
    #if QWT_VERSION >= 0x060100 || !WIN32
    m_curve->setData(new QwtPointArrayData(&v_x[0],&v_y[0],v_y.size()));
    #else
    m_curve->setData(&v_x[0],&v_y[0],v_y.size());
    #endif
    m_plot->replot();
  }
  catch (std::runtime_error& e)
  {
    this->setWindowTitle(e.what());
  }
}



void ribi::QtFunctionPlotterMainDialog::resizeEvent(QResizeEvent *)
{
  OnAnyChange();
}

#ifndef NDEBUG
void ribi::QtFunctionPlotterMainDialog::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting ribi::QtFunctionPlotterMainDialog::Test");
  TRACE("Finished ribi::QtFunctionPlotterMainDialog::Test successfully");
}
#endif
