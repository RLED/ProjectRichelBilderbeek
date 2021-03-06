//---------------------------------------------------------------------------
/*
TestLed, tool to test the Led class
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
//From http://www.richelbilderbeek.nl/ToolTestLed.htm
//---------------------------------------------------------------------------
#include <Wt/WApplication>
#include <Wt/WEnvironment>
//---------------------------------------------------------------------------
#include "trace.h"
#include "wtautoconfig.h"
#include "wtsimmysterymachinemenudialog.h"
//---------------------------------------------------------------------------
struct WtApplication : public Wt::WApplication
{
  WtApplication(const Wt::WEnvironment& env)
    : Wt::WApplication(env)
  {
    this->setTitle("SimMysteryMachine");
    this->useStyleSheet("wt.css");
    root()->addWidget(new ribi::WtSimMysteryMachineMenuDialog);
  }
};
//---------------------------------------------------------------------------
Wt::WApplication *createApplication(
  const Wt::WEnvironment& env)
{
  return new WtApplication(env);
}
//---------------------------------------------------------------------------
int main(int argc, char **argv)
{
  START_TRACE();
  ribi::WtAutoConfig::SaveDefaultStylesheet();
  ribi::WtAutoConfig a(argc,argv,createApplication);
  return a.Run();
}
//---------------------------------------------------------------------------
