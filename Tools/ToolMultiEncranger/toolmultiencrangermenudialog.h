#ifndef TOOLMULTIENCRANGERMENUDIALOG_H
#define TOOLMULTIENCRANGERMENUDIALOG_H

//#include <string>
//#include <vector>
//#include "about.h"
#include "menudialog.h"

namespace ribi {

struct ToolMultiEncrangerMenuDialog : public MenuDialog
{
  About GetAbout() const noexcept;
  Help GetHelp() const noexcept;
  boost::shared_ptr<const Program> GetProgram() const noexcept;
  std::string GetVersion() const noexcept;
  std::vector<std::string> GetVersionHistory() const noexcept;

  private:
  int ExecuteSpecific(const std::vector<std::string>& argv) noexcept;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace ribi

#endif // TOOLMULTIENCRANGERMENUDIALOG_H
