#ifndef QTSTATEOBSERVERMAINDIALOG_H
#define QTSTATEOBSERVERMAINDIALOG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

#include <boost/shared_ptr.hpp>

#include "qthideandshowdialog.h"
#include "stateobserverfwd.h"

#pragma GCC diagnostic pop

//struct LsqFilter;
//struct SlsqFilter;
//struct SymmetricLsqFilter;
struct QwtPlotCurve;

namespace Ui {
  class QtStateObserverMainDialog;
}

namespace ribi {

class QtStateObserverMainDialog : public QtHideAndShowDialog
{
  Q_OBJECT
  
public:
  QtStateObserverMainDialog(const QtStateObserverMainDialog&) = delete;
  QtStateObserverMainDialog& operator=(const QtStateObserverMainDialog&) = delete;

  explicit QtStateObserverMainDialog(QWidget *parent = 0);
  ~QtStateObserverMainDialog();
  
private slots:
  const boost::shared_ptr<AlphaFilter> CreateAlphaFilter() const;
  const boost::shared_ptr<AlphaBetaFilter> CreateAlphaBetaFilter() const;
  const boost::shared_ptr<AlphaBetaGammaFilter> CreateAlphaBetaGammaFilter() const;
  const boost::shared_ptr<IntegerAlphaFilter> CreateLsqFilter() const;
  const boost::shared_ptr<IntegerSymmetricalAlphaFilter> CreateSlsqFilter() const;
  const boost::shared_ptr<MultiAlphaFilter> CreateMultiAlphaFilter() const;
  const boost::shared_ptr<MultiIntegerStateObserver> CreateMiso() const;
  double CreateDt() const { return 1.0; }

  void Run();

  void on_button_rerun_clicked();


private:
  Ui::QtStateObserverMainDialog *ui;

  QwtPlotCurve * const m_curve_inputs;
  QwtPlotCurve * const m_curve_outputs_alpha;
  QwtPlotCurve * const m_curve_outputs_alpha_beta;
  QwtPlotCurve * const m_curve_outputs_alpha_beta_gamma;
  QwtPlotCurve * const m_curve_outputs_lsq;
  QwtPlotCurve * const m_curve_outputs_slsq;
  QwtPlotCurve * const m_curve_outputs_ma;
  QwtPlotCurve * const m_curve_outputs_miso;
};

} //~namespace ribi

#endif // QTSTATEOBSERVERMAINDIALOG_H
