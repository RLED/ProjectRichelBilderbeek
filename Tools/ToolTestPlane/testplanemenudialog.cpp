#include "testplanemenudialog.h"

#include <cassert>
#include <iostream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <boost/lexical_cast.hpp>

#include "fileio.h"
#include "fparser.hh"
#include "geometry.h"
#include "plane.h"
#include "trace.h"
#pragma GCC diagnostic pop

ribi::TestPlaneMenuDialog::TestPlaneMenuDialog()
{
  #ifndef NDEBUG
  Test();
  #endif
}

int ribi::TestPlaneMenuDialog::ExecuteSpecific(const std::vector<std::string>& argv) noexcept
{
  const int argc = static_cast<int>(argv.size());
  if (argc == 1)
  {
    std::cout << GetHelp() << '\n';
    return 1;
  }
  /*
  if (argc == 5)
  {
    std::stringstream s;
    s << "Construct plane from coefficients" << '\n';
    try
    {
      const Plane p(
        boost::lexical_cast<double>(argv[1]),
        boost::lexical_cast<double>(argv[2]),
        boost::lexical_cast<double>(argv[3]),
        boost::lexical_cast<double>(argv[4])
      );
    }
    catch (std::logic_error& e)
    {
      s << "Exception: " << e.what() << '\n';
    }
    std::cout << s.str() << '\n';
  }
  else
  */
  if (argc == 10)
  {
    std::stringstream s;
    s << "Construct plane from three points" << '\n';

    boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> p1(
      boost::lexical_cast<double>(argv[1]),
      boost::lexical_cast<double>(argv[2]),
      boost::lexical_cast<double>(argv[3])
    );

    s << "Point 1: " << Geometry().ToStr(p1) << '\n';

    boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> p2(
      boost::lexical_cast<double>(argv[4]),
      boost::lexical_cast<double>(argv[5]),
      boost::lexical_cast<double>(argv[6])
    );

    s << "Point 2: " << Geometry().ToStr(p2) << '\n';

    boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> p3(
      boost::lexical_cast<double>(argv[7]),
      boost::lexical_cast<double>(argv[8]),
      boost::lexical_cast<double>(argv[9])
    );

    s << "Point 3: " << Geometry().ToStr(p3) << '\n';

    const ribi::Plane plane(p1,p2,p3);
    try
    {
      s << "Function (X): " << plane.ToFunctionX() << '\n'
        << "Coefficients (X): " << '\n'
        << " - A: " << plane.GetCoefficientsX()[0] << '\n'
        << " - B: " << plane.GetCoefficientsX()[1] << '\n'
        << " - C: " << plane.GetCoefficientsX()[2] << '\n'
        << " - D: " << plane.GetCoefficientsX()[3] << '\n'
      ;
    }
    catch (std::logic_error& e)
    {
      s << "Exception: " << e.what() << '\n';
    }

    try
    {
      s << "Function (Y): " << plane.ToFunctionY() << '\n'
        << "Coefficients (Y): " << '\n'
        << " - A: " << plane.GetCoefficientsY()[0] << '\n'
        << " - B: " << plane.GetCoefficientsY()[1] << '\n'
        << " - C: " << plane.GetCoefficientsY()[2] << '\n'
        << " - D: " << plane.GetCoefficientsY()[3] << '\n'
      ;
    }
    catch (std::logic_error& e)
    {
      s << "Exception: " << e.what() << '\n';
    }

    try
    {
      s << "Function (Z): " << plane.ToFunctionZ() << '\n'
        << "Coefficients (Z): " << '\n'
        << " - A: " << plane.GetCoefficientsZ()[0] << '\n'
        << " - B: " << plane.GetCoefficientsZ()[1] << '\n'
        << " - C: " << plane.GetCoefficientsZ()[2] << '\n'
        << " - D: " << plane.GetCoefficientsZ()[3] << '\n'
      ;
    }
    catch (std::logic_error& e)
    {
      s << "Exception: " << e.what() << '\n';
    }
    std::cout << s.str() << '\n';
  }


  return 0;
}

ribi::About ribi::TestPlaneMenuDialog::GetAbout() const noexcept
{
  About a(
    "Richel Bilderbeek",
    "TestPlane",
    "tests the Plane class",
    "the 7th of March 2014",
    "2014",
    "http://www.richelbilderbeek.nl/ToolTestPlane.htm",
    GetVersion(),
    GetVersionHistory());
  a.AddLibrary("FileIo version: " + fileio::FileIo().GetVersion());
  a.AddLibrary("Plane version: "
    + Plane(
      Plane::Coordinat3D(0.0,0.0,0.0),
      Plane::Coordinat3D(1.0,0.0,0.0),
      Plane::Coordinat3D(0.0,1.0,0.0)
    ).GetVersion()
  );
  a.AddLibrary("Geometry version: " + Geometry().GetVersion());
  a.AddLibrary("Warp's FunctionParser version: 4.4.3");
  return a;
}

ribi::Help ribi::TestPlaneMenuDialog::GetHelp() const noexcept
{
  return ribi::Help(
    this->GetAbout().GetFileTitle(),
    this->GetAbout().GetFileDescription(),
    {

    },
    {

    }
  );
}

boost::shared_ptr<const ribi::Program> ribi::TestPlaneMenuDialog::GetProgram() const noexcept
{
  boost::shared_ptr<const ribi::Program> p {
    new ribi::ProgramTestPlane
  };
  assert(p);
  return p;
}

std::string ribi::TestPlaneMenuDialog::GetVersion() const noexcept
{
  return "1.0";
}

std::vector<std::string> ribi::TestPlaneMenuDialog::GetVersionHistory() const noexcept
{
  return {
    "2014-03-07: version 1.0: initial version"
  };
}

#ifndef NDEBUG
void ribi::TestPlaneMenuDialog::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting ribi::TestPlaneMenuDialog::Test");
  TestPlaneMenuDialog d;
  d.Execute( {"TestPlane" } );
  d.Execute( {"TestPlane","30.0","-48.0","17.0","15.0"} );
  d.Execute(
    {
      "TestPlane",
      "1.0","1.0","10.0",
      "1.0","2.0","13.0",
      "2.0","1.0","12.0"
    }
  );
  TRACE("Finished ribi::TestPlaneMenuDialog::Test successfully");
}


#endif
