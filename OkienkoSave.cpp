//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "OkienkoSave.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TSaves *Saves;
//---------------------------------------------------------------------------
__fastcall TSaves::TSaves(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSaves::DriveComboBox1Change(TObject *Sender)
{
  Katalogi->Drive=DriveComboBox1->Drive;      
}

/*
void __fastcall TForm1::Button1Click(TObject *Sender)

{
  ForceDirectories("c:\temp\mydir");
}

void __fastcall TForm1::FormCreate(TObject *Sender)

{
  DirectoryListBox1->Directory = "c:\temp";
}

void __fastcall TForm1::Button2Click(TObject *Sender)

{
  DirectoryListBox1->Update();
}

*/
//---------------------------------------------------------------------------