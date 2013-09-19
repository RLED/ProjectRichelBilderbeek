//---------------------------------------------------------------------------
/*
  Maziak, a simple maze game
  Copyright (C) 2007  Rich�l Bilderbeek

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
// From http://www.richelbilderbeek.nl
//---------------------------------------------------------------------------
#ifndef UnitFormMaziakGameOverH
#define UnitFormMaziakGameOverH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TFormMaziakGameOver : public TForm
{
__published:	// IDE-managed Components
        TImage *ImageGrave;
        TTimer *Timer1;
        TImage *ImageTop;
        TImage *ImageBottom;
        void __fastcall OnAnyClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
  bool mCanClose;
public:		// User declarations
        __fastcall TFormMaziakGameOver(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMaziakGameOver *FormMaziakGameOver;
//---------------------------------------------------------------------------
void DrawGrave(const TImage * image);

#endif