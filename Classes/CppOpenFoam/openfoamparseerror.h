#ifndef OPENFOAMPARSEERROR_H
#define OPENFOAMPARSEERROR_H

#include <stdexcept>

namespace ribi {
namespace foam {

struct ParseError : public std::runtime_error
{
  explicit ParseError(
    const std::string& filename = "",
    const int line = -1);

  const std::string m_filename;
  const int m_line;

  const std::string CreateErrorMsg(
    const std::string& filename,
    const int line) noexcept;
};

} //~namespace foam
} //~namespace ribi

#endif // OPENFOAMPARSEERROR_H
