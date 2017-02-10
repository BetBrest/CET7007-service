//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AboutProgram.h"
#define Version "1.0.1"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
        : TForm(Owner)
{

 Form2->Label1->Caption= Form2->Label1->Caption + Version;
}
//---------------------------------------------------------------------------
