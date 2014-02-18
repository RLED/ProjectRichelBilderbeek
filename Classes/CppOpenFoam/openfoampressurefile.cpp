#include "openfoampressurefile.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>

#include <QFile>

#include "fileio.h"
#include "filename.h"
#include "openfoamheader.h"
#include "trace.h"
#pragma GCC diagnostic pop


ribi::foam::PressureFile::PressureFile(
  const Header header
)
  : m_header{header}
{
  #ifndef NDEBUG
  Test();
  #endif
}

const ribi::foam::Header ribi::foam::PressureFile::GetDefaultHeader() noexcept
{
  return Header("volScalarField","0","","p");
}

#ifndef NDEBUG
void ribi::foam::PressureFile::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting ribi::foam::PressureFile::Test");
  TRACE("Finished ribi::foam::PressureFile successfully");
}
#endif

std::ostream& ribi::foam::operator<<(std::ostream& os, const PressureFile& f)
{
  os
    << f.GetHeader() << '\n'
    << "" << '\n'
    << "dimensions ["
      << f.m_dimensions[0] << " "
      << f.m_dimensions[1] << " "
      << f.m_dimensions[2] << " "
      << f.m_dimensions[3] << " "
      << f.m_dimensions[4] << " "
      << f.m_dimensions[5] << " "
      << f.m_dimensions[6]
      << "];" << '\n'
    << "" << '\n'
    << "internalField " << f.m_internal_field << ";\n"
    << "" << '\n'
    << "boundaryField" << '\n'
    << "{" << '\n'
    << f.m_boundary_field << '\n'
    /*
    << "  inlet" << '\n'
    << "  {" << '\n'
    << "    type uniformFixedValue;" << '\n'
    << "    uniformValue tableFile;" << '\n'
    << "    tableFileCoeffs" << '\n'
    << "    {" << '\n'
    << "      fileName  \"pIn\"" << '\n'
    << "      outOfBounds  clamp;" << '\n'
    << "    }" << '\n'
    << "  }" << '\n'
    << "  " << '\n'
    << "  outlet" << '\n'
    << "  {" << '\n'
    << "    type    uniformFixedValue;" << '\n'
    << "    uniformValue tableFile;" << '\n'
    << "    tableFileCoeffs" << '\n'
    << "    {" << '\n'
    << "    fileName  \"pOut\"" << '\n'
    << "    outOfBounds  clamp;" << '\n'
    << "    }" << '\n'
    << "  }" << '\n'
    << "  " << '\n'
    << "  side_CW" << '\n'
    << "  {" << '\n'
    << "    type zeroGradient;" << '\n'
    << "  }" << '\n'
    << "  " << '\n'
    << "  side_CCW" << '\n'
    << "  {" << '\n'
    << "    type zeroGradient;" << '\n'
    << "  }" << '\n'
    << "  " << '\n'
    << "  inside" << '\n'
    << "  {" << '\n'
    << "    type patch;" << '\n'
    << "  }" << '\n'
    << "  " << '\n'
    << "  outside" << '\n'
    << "  {" << '\n'
    << "    type zeroGradient;" << '\n'
    << "  }" << '\n'
    */
    << "}" << '\n'
  ;
  return os;
}
