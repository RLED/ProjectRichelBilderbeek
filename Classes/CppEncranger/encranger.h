//---------------------------------------------------------------------------
/*
Encranger, encryption and de-encryption class
Copyright (C) 2010-2014 Richel Bilderbeek

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
// From http://www.richelbilderbeek.nl/ToolMultiEncranger.htm
//---------------------------------------------------------------------------
#ifndef ENCRANGER_H
#define ENCRANGER_H

#include <string>
#include <vector>

namespace ribi {

//Encranger stands for 'ENCryption by RAndom Number GEneratoR'
struct Encranger
{
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Weffc++"
  //From http://www.richelbilderbeek.nl/CppIncrease.htm
  struct Increase : public std::unary_function<void,int>
  {
    explicit Increase(const int init_x = 0) noexcept : m_x(init_x) {}
    void operator()(int& x) noexcept
    {
      x = m_x;
      ++m_x;
    }

    private:
    int m_x;
  };
  #pragma GCC diagnostic pop

  Encranger(const int key);
  std::string Encrypt(std::string s) const noexcept;
  std::string Deencrypt(std::string s) const noexcept;

  ///Obtain this class its version
  static std::string GetVersion() noexcept;

  ///Obtain this class its version history
  static std::vector<std::string> GetVersionHistory() noexcept;

  private:

  const std::vector<char> characters;
  const std::vector<int>  table;

  const std::vector<int> CreateTable(const int key, const unsigned int sz) const;
  static const std::vector<int> CreateTestKeys() noexcept;
  static const std::vector<char> CreateCharacters() noexcept;

  char Encrypt(const char c, const int d) const noexcept;
  char Deencrypt(const char c, const int d) const noexcept;
  int GetIndex(const char c) const noexcept;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace ribi

#endif // ENCRANGER_H
