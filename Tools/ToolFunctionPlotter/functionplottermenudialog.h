#ifndef FUNCTIONPLOTTERMENUDIALOG_H
#define FUNCTIONPLOTTERMENUDIALOG_H

#include <string>
#include <vector>

#include "menudialog.h"

namespace ribi {

///GUI independent SurfacePlotter menu dialog
struct FunctionPlotterMenuDialog : public MenuDialog
{
  const About GetAbout() const noexcept;
  const Help GetHelp() const noexcept;
  const boost::shared_ptr<const Program> GetProgram() const noexcept;
  const std::string GetVersion() const noexcept;
  const std::vector<std::string> GetVersionHistory() const noexcept;

  private:
  int ExecuteSpecific(const std::vector<std::string>& argv) noexcept;

  //static const About GetAbout();
  //static const std::string GetVersion();
  //static const std::vector<std::string> GetVersionHistory();
};

} //namespace ribi

#endif // FUNCTIONPLOTTERMENUDIALOG_H
