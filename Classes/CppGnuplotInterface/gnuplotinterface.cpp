//---------------------------------------------------------------------------
//
//  Gnuplot - a C++ class interface with Gnuplot
//  V4.0
//
//  Created by Richel Bilderbeek on Fri Jun 10 2005.
//  Open source.
//
//  Richel Bilderbeek
//  Website: http://www.richelbilderbeek.nl
//
//  V4.0: 19th March 2008: Conformized code
//  V3.0: 23th September 2006: Implemented suggestion by Christoph Schmalz.
//                             According to Christoph it should work under Windows now.
//                             Made the code 100% Standard C++.
//  V2.0: 12th September 2005: Got program to compile under Windows environment.
//                             Program is found NOT to run under Windows.
//                             Somebody has an idea how to open Gnuplot.exe ???
//  V1.0: 10th June 2005: first version developed on Macintosh
//
//
//  This class functions well at a Macintosh. Under Windows, the pipe to the
//  Gnuplot executable (either MS-DOS or Windows executable) will not open.
//  If you have an idea to solve this, please E-mail me.
//
//
//  Feel free to contact me when:
//  - YOU can get the program to run under Windows (and normally it wouldn't)
//  - you encounter a compile error
//  - you have made additional functionality
//  - you have outsmarted me on this interface.
//
//----------------------------------------------------------------------------------
#include "UnitGnuplotInterface.h"
//----------------------------------------------------------------------------------
//#define EXTENSIVE_LOG
//----------------------------------------------------------------------------------
int Gnuplot::mTempFileNumber = 0;
//----------------------------------------------------------------------------------
Gnuplot::Gnuplot() :
	mGnuPipe(0),
	mTerminal("aqua"),
	mStyle("lines"),
	mCurrentWindowNumber(0)
{
  const std::string fileName = ReadFileName();
  if (FileExists(fileName)==false)
  {
    std::cout
      << "\nGnuplot file '" << fileName << "' does not exist."
      << "\nTherefore, change the line in 'GnuplotLocation.txt"
      << "\nto your Gnuplot location (e.g. 'c:/gnuplot/gnuplot')"
      << "\nThis error is generated in line " << __LINE__
      << "\nof file " << __FILE__ << std::endl
      << "\n\nPress key to terminate program." << std::endl;
    std::cin.get();
    std::exit(1);
  }
  std::cout
    << "Gnuplot executable found at '" << fileName << "'."
    << "\nOpening pipe to Gnuplot" << std::endl;
  mGnuPipe = std::_popen((fileName+" -persist").c_str(),"w"); //Changed 'rw' to 'w'
  if (mGnuPipe==0)
  {
    std::cout << "Couldn't open connection to gnuplot" << std::endl;
    std::cin.get();
    std::exit(1);
  }
  SetLineStyles();
  AddWindow();
  ChangeWindow(0);
}
//----------------------------------------------------------------------------------
///This reads the location of Gnuplot from the config file called GnuplotLocation.txt
///If it cannot find this file, it is created with a default value
const std::string Gnuplot::ReadFileName() const
{
  const std::string fileName = "GnuplotLocation.txt";
  std::string returnFileName;
  if (FileExists(fileName)==true)
  {
    std::ifstream file(fileName.c_str());
    assert(file.is_open()==true);
    file >> returnFileName;
    file.close();
  }
  else
  {
    std::ofstream file(fileName.c_str());
    assert(file.is_open()==true);
    returnFileName ="/sw/bin/gnuplot";
    file << returnFileName << '\n';
    file.close();
  }
  return returnFileName;
}
//----------------------------------------------------------------------------------
Gnuplot::~Gnuplot()
{
  const int nWindows = mWindowData.size();
  for(int i=0; i!=nWindows; ++i)
  {
    mCurrentWindowNumber = i;
    EmptyPlot();
  }
  if (_pclose(mGnuPipe) == -1) assert(!"Problem closing communication to Gnuplot");
}
//----------------------------------------------------------------------------------
void Gnuplot::EmptyPlot()
{
  const int nPlots = mWindowData[mCurrentWindowNumber].tempFileName.size();
  for (int i=0; i!=nPlots; ++i)
  {
    Dot("Removing file: " + mWindowData[mCurrentWindowNumber].tempFileName[i]);
    std::remove(mWindowData[mCurrentWindowNumber].tempFileName[i].c_str());
  }
}
//----------------------------------------------------------------------------------
void Gnuplot::SetTerminal(const GnuplotTerminal& terminal)
{
  switch(terminal)
  {
    case terminalAqua : mTerminal = "aqua"; break;
    case terminalX11  : mTerminal = "x11" ; break;
    default: assert(!"Unknown GnuplotTerminal");
  }
}
//----------------------------------------------------------------------------------
void Gnuplot::SetStyle(const GnuplotStyle& style)
{
  switch(style)
  {
    case styleLines        : mStyle = "lines"; break;
    case stylePoints       : mStyle = "points"; break;
    case styleLinespoints  : mStyle = "linespoints"; break;
    case styleImpulses     : mStyle = "impulses"; break;
    case styleDots         : mStyle = "dots"; break;
    case styleSteps        : mStyle = "steps"; break;
    case styleErrorbars    : mStyle = "errorbars"; break;
    case styleBoxes        : mStyle = "boxes"; break;
    case styleBoxerrorbars : mStyle = "boxerrorbars"; break;
    default: assert(!"Unknown GnuplotStyle");
  }
}
//----------------------------------------------------------------------------------
void Gnuplot::Execute(const std::string& cmdstr) const
{
  Dot("Command sent to Gnuplot: " + static_cast<std::string>(cmdstr));
  fputs((cmdstr+"\n").c_str(),mGnuPipe);
  fflush(mGnuPipe);

  Sleep(1.0); //Setting this sleep time too low results in errors
}
//----------------------------------------------------------------------------------
void Gnuplot::SetYlabel(const std::string& label)
{
  const std::string command = "set ylabel \""+label+"\"";
  Execute(command.c_str());
}
//----------------------------------------------------------------------------------
void Gnuplot::SetXlabel(const std::string& label)
{
  const std::string command = "set xlabel \""+label+"\"";
  Execute(command.c_str());
}
//----------------------------------------------------------------------------------
///Change to plotting windows to 'windowNumber'. If this window number is beyond
///the amount of windows created, a new window will be created and that window
///will be set as the plotting window
void Gnuplot::ChangeWindow(const int windowNumber)
{
  if (windowNumber==mCurrentWindowNumber) return;
  if (windowNumber>= static_cast<int>(mWindowData.size()))
  {
    AddWindow();
    mCurrentWindowNumber = mWindowData.size()-1;
  }
  else
  {
    mCurrentWindowNumber=windowNumber;
  }
  const std::string myCommand = "set terminal "+mTerminal+" "+Itoa(mCurrentWindowNumber+1);
  Execute(myCommand.c_str());
}
//----------------------------------------------------------------------------------
///Creates an extra 'nWindows' display windows.
void Gnuplot::AddWindow(const int nWindows)
{
  for (int i=0; i!=nWindows; ++i)
  {
    WindowData temp;
    mWindowData.push_back(temp);
  }
}
//----------------------------------------------------------------------------------
void Gnuplot::Plot(const std::string &equation, const std::string &title)
{
  std::string myCommand;
  if (mWindowData[mCurrentWindowNumber].tempFileName.size() > 0)
    myCommand = "replot " + equation + " title \"" + title + "\" with " + mStyle;
  else
    myCommand = "plot " + equation + " title \"" + title + "\" with " + mStyle;

  Execute(myCommand.c_str());
}
//----------------------------------------------------------------------------------
void Gnuplot::Plot(const std::vector<double>& x, const std::string &title)
{
  const std::string tempFileName = CreateTempFileName();
  CreateTempFile(tempFileName,x);
  PlotTempFile2D(tempFileName,title);
}
//----------------------------------------------------------------------------------
void Gnuplot::Plot(const std::vector<double>& x, const std::vector<double>& y, const std::string &title)
{
  const std::string tempFileName = CreateTempFileName();
  CreateTempFile(tempFileName,x,y);
  PlotTempFile2D(tempFileName,title);
}
//----------------------------------------------------------------------------------
void Gnuplot::Plot(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& z, const std::string &title)
{
  const std::string tempFileName = CreateTempFileName();
  CreateTempFile(tempFileName,x,y,z);
  PlotTempFile3D(tempFileName,title);
}
//----------------------------------------------------------------------------------
void Gnuplot::PlotTempFile2D(const std::string& tempFileName, const std::string& title)
{
  std::string myCommand;
  const int nSeries = mWindowData[mCurrentWindowNumber].tempFileName.size();
  //Linestyles are nice, but don't work on my terminal...
  ///if (nSeries>12) std::cout << "MAKE MORE LINESTYLES!!!" << std::endl;
  //if ( nSeries == 0)
  //	myCommand = "plot \"" + tempFileName + "\" ls "+Itoa(nSerie)+" title \"" + title + "\" with " + mStyle;
  //else
  //	myCommand = "replot \"" + tempFileName + "\" ls "+Itoa(nSerie)+" title \"" + title + "\" with " + mStyle;

  if ( nSeries == 0)
    myCommand = "plot \"" + tempFileName + "\" title \"" + title + "\" with " + mStyle;
  else
    myCommand = "replot \"" + tempFileName + "\" title \"" + title + "\" with " + mStyle;

  Execute(myCommand.c_str());
  Sleep(1.0); //Setting this sleep time too low results in errors
  mWindowData[mCurrentWindowNumber].tempFileName.push_back(tempFileName);

}
//----------------------------------------------------------------------------------
void Gnuplot::PlotTempFile3D(const std::string& tempFileName, const std::string& title)
{
  //Does not work on my computer. Should be something like this...
  Execute("set pm3d");
  std::string myCommand;
  if (mWindowData[mCurrentWindowNumber].tempFileName.size() == 0)
    myCommand = "splot \"" + tempFileName + "\" title \"" + title + "\" with pm3d palette";
  else
    myCommand = "replot \"" + tempFileName + "\" title \"" + title + "\" with pm3d palette";

  Execute(myCommand.c_str());

  Sleep(1.0); //Setting this sleep time too low results in errors
  mWindowData[mCurrentWindowNumber].tempFileName.push_back(tempFileName);
}
//----------------------------------------------------------------------------------
const std::string Gnuplot::CreateTempFileName()
{
  ++mTempFileNumber;
  const std::string tempFileName = "GnuplotTemp"+Itoa(mTempFileNumber);
  Dot("Created temp file '" + tempFileName);
  return(tempFileName);
}
//----------------------------------------------------------------------------------
void Gnuplot::CreateTempFile(const std::string& tempFileName, const std::vector<double>& x)
{
  std::ofstream file(tempFileName.c_str());
  assert(file.is_open()==true);
  const int size = x.size();
  for (int i = 0; i != size; i++) file << x[i] << std::endl;
  file.close();
}
//----------------------------------------------------------------------------------
void Gnuplot::CreateTempFile(const std::string& tempFileName, const std::vector<double>& x, const std::vector<double>& y)
{
  assert(x.size()==y.size());

  std::ofstream file(tempFileName.c_str());
  assert(file.is_open()==true);
  const int size = x.size();
  for (int i = 0; i != size; i++) file << x[i] << " " << y[i] << std::endl;
  file.close();
}
//----------------------------------------------------------------------------------
void Gnuplot::CreateTempFile(const std::string& tempFileName, const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& z)
{
  assert(x.size()==y.size());
  assert(y.size()==z.size());

  std::ofstream file(tempFileName.c_str());
  assert(file.is_open()==true);
  const int size = x.size();
  for (int i = 0; i != size; i++) file << x[i] << " " << y[i] << " " << z[i] << std::endl;
  file.close();
}
//----------------------------------------------------------------------------------
void Gnuplot::SetLineStyles()
{
  //Does not work on all terminals. Like mine... :-(
  //Execute("set style line  1 lt pal frac 0.0 lw 2");
  //Execute("set style line  2 lt pal frac 0.0 lw 4");
  //Execute("set style line  3 lt pal frac 0.2 lw 2");
  //Execute("set style line  4 lt pal frac 0.2 lw 4");
  //Execute("set style line  5 lt pal frac 0.4 lw 2");
  //Execute("set style line  6 lt pal frac 0.4 lw 4");
  //Execute("set style line  7 lt pal frac 0.6 lw 2");
  //Execute("set style line  8 lt pal frac 0.6 lw 4");
  //Execute("set style line  9 lt pal frac 0.8 lw 2");
  //Execute("set style line 10 lt pal frac 0.8 lw 4");
  //Execute("set style line 11 lt pal frac 1.0 lw 2");
  //Execute("set style line 12 lt pal frac 1.0 lw 4");
}
//----------------------------------------------------------------------------------
///Logging when in developmental phase
void Gnuplot::Dot(const std::string& debugString) const
{
  #ifdef EXTENSIVE_LOG
  std::cout << "DOT: " << debugString << std::endl;
  #endif
}
//----------------------------------------------------------------------------------
///Wait for myTime seconds
void Gnuplot::Sleep(const double myTime) const
{
  std::clock_t timeStart = std::clock();
  std::clock_t timeEnd   = std::clock();
  while ((std::difftime(timeEnd,timeStart) / CLK_TCK) < myTime)
  {
    //Wait...
  }
}
//----------------------------------------------------------------------------------

///Converts an integer to a string
const std::string Gnuplot::Itoa(const int number) const
{
  std::ostringstream o;
  if (!(o << number)) return "ERROR";
  return o.str();
}
//----------------------------------------------------------------------------------
///Converts a double to a string
const std::string Gnuplot::Ftoa(const double number) const
{
  std::ostringstream o;
  if (!(o << number)) return "ERROR";
  return o.str();
}
//----------------------------------------------------------------------------------
///Checks if a file exists
const bool Gnuplot::FileExists(const std::string& fileName) const
{
  std::fstream fin;
  fin.open(fileName.c_str(),std::ios::in);
  if( fin.is_open() )
  {
    fin.close();
    return true;
  }
  fin.close();
  return false;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

