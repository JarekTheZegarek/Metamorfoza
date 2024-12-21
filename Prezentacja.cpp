//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Prezentacja.h"
#include "metamorfozis_w5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormularzOpcjiPrezentacji *FormularzOpcjiPrezentacji;
//---------------------------------------------------------------------------
__fastcall TFormularzOpcjiPrezentacji::TFormularzOpcjiPrezentacji(TComponent* Owner)
        : TForm(Owner)
{
  Timer1->Interval=UpDown1->Position*10;
}
//---------------------------------------------------------------------------
void __fastcall TFormularzOpcjiPrezentacji::Timer1Timer(TObject *Sender)
{
    TShiftState S1;
    Timer1->Interval=UpDown1->Position*10;
    Form1->FormKeyDown(Sender, VK_RIGHT, S1);
}
//---------------------------------------------------------------------------
void __fastcall TFormularzOpcjiPrezentacji::Button1Click(TObject *Sender)
{
   if(CheckBox1->Checked)
        Form1->LabelDane->Visible=true;
   else
        Form1->LabelDane->Visible=false;

   Close();
}
//---------------------------------------------------------------------------

