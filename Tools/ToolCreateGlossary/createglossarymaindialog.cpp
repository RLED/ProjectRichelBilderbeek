//---------------------------------------------------------------------------
/*
CreateGlossary, tool to create my glossaries
Copyright (C) 2011-2012 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/ToolCreateGlossary.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "createglossarymaindialog.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <boost/function.hpp>
#include <boost/xpressive/xpressive.hpp>

#include <QDir>

#include "htmlpage.h"
#include "trace.h"

#pragma GCC diagnostic pop

ribi::CreateGlossaryMainDialog::CreateGlossaryMainDialog()
{
  #ifndef NDEBUG
  Test();
  #endif
  TRACE("Start of CreateGlossaryMainDialog");

  CreatePage("Command-line glossary","ClGlossary.htm","Cl.*\\.htm\\>");
  CreatePage("C++ glossary","CppGlossary.htm","Cpp.*\\.htm\\>");
  CreatePage("Game glossary","GameGlossary.htm","Game.*\\.htm\\>");
  CreatePage("Levend Cluedo glossary","LevendCluedoGlossary.htm","LevendCluedo.*\\.htm\\>");
  CreatePage("Tool glossary","ToolGlossary.htm","Tool.*\\.htm\\>");
  CreatePage("Music glossary","MusicGlossary.htm","(Music|Song|Cd).*\\.htm\\>");
  CreatePage("Sitemap","Sitemap.htm",".*\\.htm\\>");

  TRACE("Finished CreateGlossaryMainDialog successfully");
}

const std::vector<std::string> ribi::CreateGlossaryMainDialog::GetFilesInFolder(const std::string& folder)
{
  QDir dir(folder.c_str());
  dir.setFilter(QDir::Files);
  const QFileInfoList list = dir.entryInfoList();

  //Convert QFileInfoList to std::vector<std::string> of filenames
  std::vector<std::string> v;
  const int size = list.size();
  for (int i = 0; i != size; ++i)
  {
    const std::string file_name = list.at(i).fileName().toStdString();
    v.push_back(file_name);
  }
  return v;
}

const std::vector<std::string> ribi::CreateGlossaryMainDialog::GetFilesInFolder(
  const std::string& folder,
  const std::string& regex_str)
{
  //Get all filenames
  const std::vector<std::string> v = GetFilesInFolder(folder);

  //Create the regex for a correct C++ filename
  const boost::xpressive::sregex regex
    = boost::xpressive::sregex::compile(regex_str);

  //Create the resulting std::vector
  std::vector<std::string> w;

  //Copy all filenames matching the regex in the resulting std::vector
  std::for_each(v.begin(), v.end(),
    [&w,regex](const std::string& s)
    {
      if (boost::xpressive::regex_match(s,regex)) w.push_back(s);
    }
  );

  return w;
}

const std::string ribi::CreateGlossaryMainDialog::GetPath(const std::string& filename)
{
  const int a = filename.rfind("\\",filename.size());
  const int b = filename.rfind("/",filename.size());
  const int i = std::max(a,b);
  assert(i < static_cast<int>(filename.size()));
  return filename.substr(0,i);
}

void ribi::CreateGlossaryMainDialog::CreatePage(
  const std::string& page_name,
  const std::string& page_url,
  const std::string& regex)
{
  const std::vector<std::string> pagenames
    = GetFilesInFolder("/home/richel/ProjectRichelBilderbeek/Projects/RichelbilderbeekNl",regex);

  std::vector<boost::shared_ptr<const HtmlPage> > pages;
  for (const std::string& s: pagenames)
  {
    const std::string full_path
      = std::string("/home/richel/ProjectRichelBilderbeek/")
      + std::string("Projects/RichelbilderbeekNl/")
      + s;
    assert(HtmlPage::IsRegularFile(full_path));
    boost::shared_ptr<const HtmlPage> page {
      new HtmlPage(full_path)
    };
    pages.push_back(page);
  }

  std::sort(pages.begin(),pages.end(),
    [](const boost::shared_ptr<const HtmlPage> lhs,const boost::shared_ptr<const HtmlPage> rhs)
    {
     return *lhs < *rhs;
    }
  );

  std::ofstream f(page_url.c_str());

  f
    << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n"
    << "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
    << "<html xmlns=\"http://www.w3.org/1999/xhtml\" lang=\"en\" xml:lang=\"en\">\n"
    << "<head>\n"
    << "  <meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\"/>\n"
    << "  <title>" << page_name << "</title>\n"
    << "  <meta name=\"description\" content=\"" << page_name << "\"/>\n"
    << "  <meta name=\"keywords\" content=\"" << page_name << " table of content index overview\"/>\n"
    << "  <link rel=\"stylesheet\" href=\"Richelbilderbeek.css\" type=\"text/css\"/>\n"
    << "</head>\n"
    << "<!-- End of head, start of body -->\n"
    << "<body>\n"
    << "<p><a href=\"index.htm\">Go back to Richel Bilderbeek's homepage</a>.</p>\n"
//    << "<p><a href=\"Cpp.htm\">Go back to Richel Bilderbeek's C++ page</a>.</p>\n"
    << "<p>&nbsp;</p>\n"
    << "<p>&nbsp;</p>\n"
    << "<p>&nbsp;</p>\n"
    << "<p>&nbsp;</p>\n"
    << "<p>&nbsp;</p>\n"
    << "<!-- Page header -->\n"
    << "<h1><a href=\"" << page_url << "\">" << page_name << "</a></h1>\n"
    << "<p>&nbsp;</p>\n"
    << "<p>This page is generated by the\n"
    << "<a href=\"CppTools.htm\">tool</a> <a href=\"ToolCreateGlossary.htm\">CreateGlossary</a>.</p>\n"
    << "<p>&nbsp;</p>\n"
    << "<ul>\n";

  for(const boost::shared_ptr<const HtmlPage> page: pages)
  {
    if (page->GetTitle() == "Redirection page"
      || page->GetTitle().empty()
      || page->GetFilename().find("_old.htm") != std::string::npos)
    {
      //continue;
    }
    else
    {
      std::string s
        = "  <li><a href=\""
        + RemovePath(page->GetFilename())
        + "\">"
        + page->GetTitle()
        + "</a></li>";

      s = HtmlPage::ReplaceAll(s,"&","[AMPERSAND]");
      s = HtmlPage::ReplaceAll(s,"[AMPERSAND]","&amp;");

      f << s << '\n';
    }
  }

  TRACE(pagenames.size());

  f
    << "</ul>\n"
    << "<p>&nbsp;</p>\n"
    << "<p>Number of pages: " << pagenames.size() << "</p>\n"
    << "<p>&nbsp;</p>\n"
    << "<p>&nbsp;</p>\n"
    << "<p>&nbsp;</p>\n"
    << "<p>&nbsp;</p>\n"
//    << "<p><a href=\"Cpp.htm\">Go back to Richel Bilderbeek's C++ page</a>.</p>\n"
    << "<p><a href=\"index.htm\">Go back to Richel Bilderbeek's homepage</a>.</p>\n"
    << "<p>&nbsp;</p>\n"
    << "<p><a href=\"http://validator.w3.org/check?uri=referer\"><img src=\"valid-xhtml10.png\" alt=\"Valid XHTML 1.0 Strict\" height=\"31\" width=\"88\" /></a></p>\n"
    << "</body>\n"
    << "</html>\n";
}

const std::string ribi::CreateGlossaryMainDialog::RemovePath(const std::string& filename)
{
  std::vector<std::size_t> v;
  const std::size_t a = filename.rfind("\\",filename.size());
  if (a != std::string::npos) v.push_back(a);
  const std::size_t b = filename.rfind("/",filename.size());
  if (b != std::string::npos) v.push_back(b);
  if (v.empty()) return filename;
  const std::size_t i = *std::max_element(v.begin(),v.end());
  assert(i < filename.size());
  const std::size_t j = i + 1;
  assert(j < filename.size());
  const std::string s = filename.substr(j,filename.size() - j);
  TRACE(s);
  return s;
}

#ifndef NDEBUG
void ribi::CreateGlossaryMainDialog::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  assert(RemovePath("x.txt") == std::string("x.txt"));
  assert(RemovePath("MyFolder/x.txt") == std::string("x.txt"));
  assert(RemovePath("Another/MyFolder/x.txt") == std::string("x.txt"));
  assert(RemovePath("Yet/Another/MyFolder/x.txt") == std::string("x.txt"));
  assert(RemovePath("MyFolder\\x.txt") == std::string("x.txt"));
  assert(RemovePath("Another\\MyFolder\\x.txt") == std::string("x.txt"));
  assert(RemovePath("Yet\\Another\\MyFolder\\x.txt") == std::string("x.txt"));
  assert(RemovePath("Another/MyFolder\\x.txt") == std::string("x.txt"));
  assert(RemovePath("Another\\MyFolder/x.txt") == std::string("x.txt"));
  assert(RemovePath("Yet\\Another/MyFolder\\x.txt") == std::string("x.txt"));
  assert(RemovePath("Yet\\Another\\MyFolder/x.txt") == std::string("x.txt"));
}
#endif
