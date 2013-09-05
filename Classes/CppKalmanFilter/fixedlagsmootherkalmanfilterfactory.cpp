//#include own header file as first substantive line of code, from:
// * John Lakos. Large-Scale C++ Software Design. 1996. ISBN: 0-201-63362-0. Section 3.2, page 110
#include "fixedlagsmootherkalmanfilterfactory.h"

#include <cassert>

const boost::shared_ptr<ribi::FixedLagSmootherKalmanFilter> ribi::FixedLagSmootherKalmanFilterFactory::Create(
  const boost::shared_ptr<const KalmanFilterParameters>& parameters)
{
  assert(parameters);
  assert(parameters->GetType() == KalmanFilterType::fixed_lag_smoother);

  const boost::shared_ptr<FixedLagSmootherKalmanFilterCalculationElements> calculation {
    new FixedLagSmootherKalmanFilterCalculationElements
  };

  assert(calculation);

  const boost::shared_ptr<FixedLagSmootherKalmanFilter> kalman_filter {
    new FixedLagSmootherKalmanFilter(calculation,parameters)
  };
  assert(kalman_filter);
  assert(kalman_filter->GetType() == KalmanFilterType::fixed_lag_smoother);
  return kalman_filter;
}


