//---------------------------------------------------------------------------
/*
RandomCode, tool to generate random C++ code
Copyright (C) 2010-2011  Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/ToolRandomCode.htm
//---------------------------------------------------------------------------
#include <boost/foreach.hpp>
//---------------------------------------------------------------------------
#include <Wt/WBreak>
#include <Wt/WPushButton>
#include <Wt/WTextArea>
//---------------------------------------------------------------------------
#include "wtrandomcodegeneratedialog.h"
#include "randomcode.h"
//---------------------------------------------------------------------------
ribi::WtRandomCodeGenerateDialog::WtRandomCodeGenerateDialog()
  : m_button_generate(new Wt::WPushButton),
    m_text_area(new Wt::WTextArea)
{
  this->setContentAlignment(Wt::AlignCenter);
  this->addWidget(m_text_area);
  this->addWidget(new Wt::WBreak());
  this->addWidget(m_button_generate);
  m_text_area->setMinimumSize(800,400);

  m_button_generate->setText("Generate");

  m_button_generate->clicked().connect(
    this, &ribi::WtRandomCodeGenerateDialog::OnGenerateClick);

  OnGenerateClick();
}
//---------------------------------------------------------------------------
void ribi::WtRandomCodeGenerateDialog::OnGenerateClick()
{
  const std::vector<std::string> v = RandomCode::CreateRandomCode();
  std::string s;
  BOOST_FOREACH(const std::string& line,v)
  {
    s += line + '\n';
  }
  m_text_area->setText(s);
}
//---------------------------------------------------------------------------
