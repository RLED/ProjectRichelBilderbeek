//---------------------------------------------------------------------------
/*
GTST, Game Theory Server
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
//From http://www.richelbilderbeek.nl/ProjectGtst.htm
//---------------------------------------------------------------------------
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <boost/filesystem.hpp>

#include "all_participantdialogstates.h"
#include "all_participantstates.h"
#include "all_serverstates.h"
#include "groupfinished.h"
#include "grouploggedin.h"
#include "groupnotloggedin.h"
#include "groups.h"
#include "logfile.h"
#include "participant.h"
#include "participantdialog.h"
#include "participantdialogstates.h"
#include "state.h"
#include "stopwatch.h"
#include "server.h"
#include "trace.h"
#include "wtserverpusher.h"

ribi::gtst::ParticipantDialogState::ParticipantDialogState(
  ParticipantDialog * const dialog,
  Server * const server)
  : m_dialog(dialog),
    m_server(server)
{
  assert(m_dialog);
  assert(m_server);
}

const std::string ribi::gtst::ParticipantDialogState::CurrencyToStr(const double value)
{
  std::stringstream s;
  s << std::setprecision(2) << std::fixed;
  s << value;
  return s.str();
}

///FileToVector reads a file and converts it to a std::vector<std::string>
///From http://www.richelbilderbeek.nl/CppFileToVector.htm
const std::vector<std::string> ribi::gtst::ParticipantDialogState::FileToVector(const std::string& filename)
{
  assert(boost::filesystem::exists(filename));
  std::vector<std::string> v;
  std::ifstream in(filename.c_str());
  std::string s;
  for (int i=0; !in.eof(); ++i)
  {
    std::getline(in,s);
    v.push_back(s);
  }
  return v;
}

ribi::gtst::ParticipantDialog * ribi::gtst::ParticipantDialogState::GetDialog()
{
  assert(m_dialog);
  return m_dialog;
}

const ribi::gtst::ParticipantDialog * ribi::gtst::ParticipantDialogState::GetDialog() const
{
  assert(m_dialog);
  return m_dialog;
}

void ribi::gtst::ParticipantDialogState::Logout()
{
  m_server->NotifyLogout(GetDialog()->GetParticipant());
  WtServerPusher::GetInstance()->Post();
}

///Respons to the Participant his/her ParticipantState, which
///can be changed by the Server
void ribi::gtst::ParticipantDialogState::RespondToParticipant()
{
  if (!GetDialog()->CanGetParticipant())
  {
    return;
  }

  assert(GetDialog());
  assert(GetDialog()->GetParticipant());
  assert(GetDialog()->GetParticipant()->GetState());

  const ParticipantState * const state = this->GetDialog()->GetParticipant()->GetState();

  if (dynamic_cast<const ParticipantStateAssignPayoff*>(state))
  {
    m_dialog->SetState(m_dialog->GetStates()->GetStateAssignPayoff());
  }
  else if (dynamic_cast<const ParticipantStateChat*>(state))
  {
    m_dialog->SetState(m_dialog->GetStates()->GetStateChat());
  }
  else if (dynamic_cast<const ParticipantStateChooseAction*>(state))
  {
    m_dialog->SetState(m_dialog->GetStates()->GetStateChooseAction());
  }
  else if (dynamic_cast<const ParticipantStateFinished*>(state))
  {
    m_dialog->SetState(m_dialog->GetStates()->GetStateFinished());
  }
  else if (dynamic_cast<const ParticipantStateGroupAssign*>(state))
  {
    m_dialog->SetState(m_dialog->GetStates()->GetStateGroupAssign());
  }
  else if (dynamic_cast<const ParticipantStateGroupReAssign*>(state))
  {
    m_dialog->SetState(m_dialog->GetStates()->GetStateGroupReAssign());
  }
  else if (dynamic_cast<const ParticipantStateLoggedIn*>(state))
  {
    m_dialog->SetState(m_dialog->GetStates()->GetStateLoggedIn());
  }
  else if (dynamic_cast<const ParticipantStateNotLoggedIn*>(state))
  {
    m_dialog->SetState(m_dialog->GetStates()->GetStateNotLoggedIn());
  }
  else if (dynamic_cast<const ParticipantStateQuiz*>(state))
  {
    m_dialog->SetState(m_dialog->GetStates()->GetStateQuiz());
  }
  else if (dynamic_cast<const ParticipantStateViewResultsGroup*>(state))
  {
    m_dialog->SetState(m_dialog->GetStates()->GetStateViewResultsGroup());
  }
  else if (dynamic_cast<const ParticipantStateViewResultsVoting*>(state))
  {
    m_dialog->SetState(m_dialog->GetStates()->GetStateViewResultsVoting());
  }
  else if (dynamic_cast<const ParticipantStateVoting*>(state))
  {
    m_dialog->SetState(m_dialog->GetStates()->GetStateVoting());
  }
  else
  {
    const std::string error
      = std::string("Unimplemented state: ")
      + state->ToStr();
    std::cerr << error << '\n';
    std::clog << error << '\n';
    assert("Should not get here, there must be an unimplemented state");
    throw std::logic_error(
      "Should not get here (void ribi::gtst::ParticipantDialogState::RespondToParticipant), there must be an unimplemented state");
  }
  #define DEBUG_TEMP_2367452375825442365723832
  #ifdef  DEBUG_TEMP_2367452375825442365723832
  if(GetDialog()->GetStates()->GetCurrentState()->ToStr()
    != GetDialog()->GetParticipant()->GetState()->ToStr())
  {
    TRACE(GetDialog()->GetStates()->GetCurrentState()->ToStr());
    TRACE(GetDialog()->GetParticipant()->GetState()->ToStr());
  }
  #endif
  //assert(this->GetDialog()->GetStates()->GetCurrentState()->ToStr()
  //  == GetDialog()->GetParticipant()->GetState()->ToStr());

}

