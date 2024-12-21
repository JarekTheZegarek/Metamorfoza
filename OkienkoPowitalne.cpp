//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "OkienkoPowitalne.h"
#include "metamorfozis_w5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
  TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button1bClick(TObject *Sender)
{
ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button2bClick(TObject *Sender)
{
ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button3bClick(TObject *Sender)
{
ModalResult = mrAbort;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormActivate(TObject *Sender)
{
 //HintWindowClass=  __classid(TFontyDlaHinta);
 Screen->HintFont->Name="Arial";
 Screen->HintFont->Size=10;
 Application->HintColor=clAqua;
 Screen->HintFont->Charset=  EASTEUROPE_CHARSET	;
}
//---------------------------------------------------------------------------

