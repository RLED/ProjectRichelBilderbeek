#include "qttooltestapproximatormaindialog.h"

#include <cassert>

#include <qwt_legend.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_zoomer.h>

#if QWT_VERSION >= 0x060000
#include <qwt_point_data.h>
#endif

#include "ui_qttooltestapproximatormaindialog.h"

QtToolTestApproximatorMainDialog::QtToolTestApproximatorMainDialog(QWidget *parent) :
  QtHideAndShowDialog(parent),
  ui(new Ui::QtToolTestApproximatorMainDialog),
  m_approximator(),
  m_curve_approximation(new QwtPlotCurve),
  m_curve_values(new QwtPlotCurve),
  m_plot(new QwtPlot)
{
  ui->setupUi(this);

  //Set up the plot
  assert(m_plot);
  m_plot->setTitle("Approximator");
  m_plot->setAxisTitle(QwtPlot::xBottom,"Key");
  m_plot->setAxisTitle(QwtPlot::yLeft,"Value");
  #ifdef _WIN32
  m_plot->setCanvasBackground(QBrush(QColor(255,255,255)));
  #else
  plot->setCanvasBackground(QColor(255,255,255));
  #endif

  //Create plots
  assert(m_curve_values);
  m_curve_values->setTitle("Points");
  m_curve_values->attach(m_plot.get());
  m_curve_values->setStyle(QwtPlotCurve::Dots);
  m_curve_values->setPen(QPen(QColor(255,0,0),5));

  assert(m_curve_approximation);
  m_curve_approximation->setTitle("Approximation");
  m_curve_approximation->attach(m_plot.get());
  m_curve_approximation->setStyle(QwtPlotCurve::Dots);
  m_curve_approximation->setPen(QPen(QColor(0,0,255),3));

  //Add grid
  {
    QwtPlotGrid * const grid = new QwtPlotGrid;
    grid->setPen(QPen(QColor(128,128,128)));
    grid->attach(m_plot.get());
  }
  //Add zoomer
  {
    new QwtPlotZoomer(m_plot->canvas());
  }
  {
    // legend
    QwtLegend * const legend = new QwtLegend;
    legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
    m_plot->insertLegend(legend, QwtPlot::RightLegend);
  }
  assert(ui->verticalLayout->layout());
  ui->verticalLayout->layout()->addWidget(m_plot.get());

  m_plot->setAxisScale(
    QwtPlot::xBottom,
    static_cast<double>(ui->box_int_x->minimum()),
    static_cast<double>(ui->box_int_x->maximum())
  );
  m_plot->setAxisScale(
    QwtPlot::yLeft,
    static_cast<double>(ui->box_double_y->minimum()),
    static_cast<double>(ui->box_double_y->maximum())
  );

  //Add some nice testing values
  ui->box_int_x->setValue(ui->box_int_x->minimum() / 2);
  ui->box_double_y->setValue(ui->box_double_y->maximum() / 2.0);
  on_button_clicked();

  ui->box_int_x->setValue(ui->box_int_x->minimum() / 4);
  ui->box_double_y->setValue(ui->box_double_y->minimum() / 2.0);
  on_button_clicked();

  ui->box_int_x->setValue(ui->box_int_x->maximum() / 4);
  ui->box_double_y->setValue(ui->box_double_y->maximum() / 2.0);
  on_button_clicked();

  ui->box_int_x->setValue(ui->box_int_x->maximum() / 2);
  ui->box_double_y->setValue(ui->box_double_y->minimum() / 2.0);
  on_button_clicked();

  ui->box_int_x->setValue(0);
  ui->box_double_y->setValue(0.0);

  TRACE_FUNC();
}

QtToolTestApproximatorMainDialog::~QtToolTestApproximatorMainDialog()
{
  delete ui;
}

void QtToolTestApproximatorMainDialog::on_button_clicked()
{
  const int x = ui->box_int_x->value();
  const double y = ui->box_double_y->value();
  assert(m_approximator.CanAdd(x,y)
    && "Can only click the button when the pair can be added");

  m_approximator.Add(x,y);
  Plot();
}

void QtToolTestApproximatorMainDialog::on_box_int_x_valueChanged(int)
{
  const int x = ui->box_int_x->value();
  const double y = ui->box_double_y->value();
  ui->button->setEnabled( m_approximator.CanAdd(x,y) );
}

void QtToolTestApproximatorMainDialog::on_box_double_y_valueChanged(double)
{
  const int x = ui->box_int_x->value();
  const double y = ui->box_double_y->value();
  ui->button->setEnabled( m_approximator.CanAdd(x,y) );
}

void QtToolTestApproximatorMainDialog::Plot()
{
  //Plot approximation
  {
    std::vector<double> xs;
    std::vector<double> ys;
    for (const auto p: m_approximator.Get())
    {
      xs.push_back( static_cast<double>(p.first) );
      ys.push_back( static_cast<double>(p.second) );
    }
    assert(m_curve_values);
    #if QWT_VERSION >= 0x060000
    m_curve_values->setData(new QwtPointArrayData(&xs[0],&ys[0],xs.size()));
    #else
    m_curve_values->setData(&xs[0],&y[0],xs.size());
    #endif
  }

  const double min_x = static_cast<double>(ui->box_int_x->minimum());
  const double max_x = static_cast<double>(ui->box_int_x->maximum());

  //Plot approximation
  {
    std::vector<double> xs;
    std::vector<double> ys;
    for (double t=min_x; t < max_x; t+=0.5)
    {
      try
      {
        const double x = static_cast<double>(t);
        const double y = m_approximator.Approximate(x);
        ys.push_back(y);
        xs.push_back(x);
      }
      catch (ExceptionNoExtrapolation<Approximator_t::key_type>& e)
      {

      }
    }
    assert(m_curve_approximation);
    #if QWT_VERSION >= 0x060000
    m_curve_approximation->setData(new QwtPointArrayData(&xs[0],&ys[0],xs.size()));
    #else
    m_curve_approximation->setData(&xs[0],&y[0],xs.size());
    #endif
  }
  assert(m_plot);
  m_plot->replot();
}