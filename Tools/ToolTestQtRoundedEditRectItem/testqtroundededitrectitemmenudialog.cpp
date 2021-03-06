#include "testqtroundededitrectitemmenudialog.h"

#include <cassert>
#include <iostream>

#include "trace.h"

int ribi::TestQtRoundedEditRectItemMenuDialog::ExecuteSpecific(const std::vector<std::string>& argv) noexcept
{
  #ifndef NDEBUG
  Test();
  #endif
  const int argc = static_cast<int>(argv.size());
  if (argc == 1)
  {
    std::cout << GetHelp() << '\n';
    return 1;
  }
  assert(!"TODO");
  return 1;
}

ribi::About ribi::TestQtRoundedEditRectItemMenuDialog::GetAbout() const noexcept
{
  About a(
    "Richel Bilderbeek",
    "TestQtRoundedEditRectItemWidget",
    "tests QtRoundedEditRectItemWidget",
    "the 31st of December 2012",
    "2012-2014",
    "http://www.richelbilderbeek.nl/ToolTestQtRoundedEditRectItemWidget.htm",
    GetVersion(),
    GetVersionHistory());
  //a.AddLibrary("ProFile version: " + ProFile::GetVersion());
  a.AddLibrary("Trace version: " + Trace::GetVersion());
  return a;
}

ribi::Help ribi::TestQtRoundedEditRectItemMenuDialog::GetHelp() const noexcept
{
  return Help(
    this->GetAbout().GetFileTitle(),
    this->GetAbout().GetFileDescription(),
    {

    },
    {

    }
  );
}

boost::shared_ptr<const ribi::Program> ribi::TestQtRoundedEditRectItemMenuDialog::GetProgram() const noexcept
{
  const boost::shared_ptr<const Program> p {
    new ProgramTestQtRoundedEditRectItem
  };
  assert(p);
  return p;
}

std::string ribi::TestQtRoundedEditRectItemMenuDialog::GetVersion() const noexcept
{
  return "1.2";
}

std::vector<std::string> ribi::TestQtRoundedEditRectItemMenuDialog::GetVersionHistory() const noexcept
{
  return {
    "2012-12-21: version 1.0: initial version",
    "2012-12-31: version 1.1: added menu",
    "2013-11-05: version 1.2: conformized for ProjectRichelBilderbeekConsole"
  };
}

#ifndef NDEBUG
void ribi::TestQtRoundedEditRectItemMenuDialog::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting ribi::TestQtRoundedEditRectItemMenuDialog::Test");
  TRACE("Finished ribi::TestQtRoundedEditRectItemMenuDialog::Test successfully");
}
#endif
