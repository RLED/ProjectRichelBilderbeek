//#include own header file as first substantive line of code, from:
// * John Lakos. Large-Scale C++ Software Design. 1996. ISBN: 0-201-63362-0. Section 3.2, page 110
#include "steadystatekalmanfiltercalculationelements.h"

#include <cassert>
#include "kalmanfiltercalculationelementsfactory.h"

ribi::SteadyStateKalmanFilterCalculationElements::SteadyStateKalmanFilterCalculationElements(
  const boost::numeric::ublas::vector<double>& measurement,
  const boost::numeric::ublas::vector<double>& predicted_state,
  const boost::numeric::ublas::vector<double>& previous_state_estimate,
  const boost::numeric::ublas::vector<double>& updated_state)

  : KalmanFilterCalculationElements(
      measurement,
      predicted_state,
      previous_state_estimate,
      updated_state)
{
  //... nothing to check
}

const boost::shared_ptr<ribi::KalmanFilterCalculationElements> ribi::SteadyStateKalmanFilterCalculationElements::Clone() const
{
  const boost::shared_ptr<KalmanFilterCalculationElements> p {
    new SteadyStateKalmanFilterCalculationElements(
      //Base
      this->GetMeasurement(),
      this->GetPredictedState(),
      this->GetPreviousState(),
      this->GetUpdatedState()
      //Derived class
      //... no additional data members
    )
  };
  assert(p);
  assert(p->GetType() == this->GetType());
  return p;
}

bool ribi::SteadyStateKalmanFilterCalculationElements::IsComplete() const
{
  const std::size_t sz = GetMeasurement().size();
  return
       sz != 0
    && sz == GetMeasurement().size()
    && sz == GetPreviousState().size()
    && sz == GetUpdatedState().size();
}
