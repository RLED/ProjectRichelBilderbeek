//---------------------------------------------------------------------------
/*
WtSafeTimer, forward-declaration-safe Wt::WTimer
Copyright (C) 2011 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppWtSafeTimer.htm
//---------------------------------------------------------------------------
#ifndef WTSAFETIMER_H
#define WTSAFETIMER_H
//---------------------------------------------------------------------------
#include <string>
#include <vector>
//---------------------------------------------------------------------------
#include <boost/checked_delete.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
//---------------------------------------------------------------------------
namespace Wt
{
  struct WTimer;
}
//---------------------------------------------------------------------------
#include <Wt/WObject>
#include <Wt/WEvent>
//---------------------------------------------------------------------------
///WtSafeTimer is hopefully an even safer timer than Wt::WTimer
struct WtSafeTimer : public boost::noncopyable, Wt::WObject
{
  ///Construct a WtSafeTimer from its firing time
  WtSafeTimer(const int time_msec);

  ///Get the WtSafeTimer's current version
  static const std::string GetVersion();

  ///Get the WtSafeTimer's version history
  static const std::vector<std::string> GetVersionHistory();

  ///The signal emitted every tick
  Wt::EventSignal<Wt::WMouseEvent>& timeout();

  private:
  ///\brief
  ///Only allow a Boost smart pointer to delete WtSafeTimer
  ///
  ///This prevents the following trouble,
  ///cited from http://www.boost.org/libs/utility/checked_delete.html:
  ///The C++ Standard allows, in 5.3.5/5, pointers to incomplete
  ///class types to be deleted with a delete-expression.
  ///When the class has a non-trivial destructor, or a class-specific operator
  ///delete, the behavior is undefined. Some compilers issue a warning when an
  ///incomplete type is deleted, but unfortunately, not all do, and programmers
  ///sometimes ignore or disable warnings.
  ~WtSafeTimer();
  ///Only allow a Boost smart pointer to delete WtSafeTimer
  //Template syntax from Herb Sutter. Exceptional C++ style. 2005. ISBN: 0-201-76042-8. Item 8: 'Befriending templates'.
  friend void boost::checked_delete<>(WtSafeTimer*);

  ///Internal ticking method
  void OnTimer();

  ///The fortified Wt::WTimer
  boost::scoped_ptr<Wt::WTimer> m_timer;
};
//---------------------------------------------------------------------------
#endif // WTSAFETIMER_H
