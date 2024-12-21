//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitPrzelicznik.h"
#include "termopara.h"
extern Term termopara;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPrzelicznik *Przelicznik;
//---------------------------------------------------------------------------
__fastcall TPrzelicznik::TPrzelicznik(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPrzelicznik::FormCreate(TObject *Sender)
{
      OpcjaClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TPrzelicznik::OpcjaClick(TObject *Sender)
{
  switch(Opcja->ItemIndex)
  {
   case 0:      StrGrTabelka->Cells[1][0]=" T [K] ";
                StrGrTabelka->Cells[2][0]=" T [C] ";
                StrGrTabelka->Cells[3][0]=" U [mV] ";
                break;
   case 1:      StrGrTabelka->Cells[1][0]=" U [mV] ";
                StrGrTabelka->Cells[2][0]=" T [C] ";
                StrGrTabelka->Cells[3][0]=" T [K] ";
                break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TPrzelicznik::PrzeliczClick(TObject *Sender)
{
    AnsiString Fo;
    int i=1;
    while( StrGrTabelka->Cells[1][i]!="" || StrGrTabelka->Cells[2][i]!="")
    {
     switch(Opcja->ItemIndex)
     {
      case 0:
             if(StrGrTabelka->Cells[2][i]=="")
                        StrGrTabelka->Cells[2][i]=Fo.FormatFloat("0.####",  StrGrTabelka->Cells[1][i].ToDouble()-273.15);
             else
                        StrGrTabelka->Cells[1][i]=Fo.FormatFloat("0.####",StrGrTabelka->Cells[2][i].ToDouble()+273.15);
             StrGrTabelka->Cells[3][i]= Fo.FormatFloat("0.####",termopara.CelsjuszNap(StrGrTabelka->Cells[2][i].ToDouble()));
             break;
      case 1: if(StrGrTabelka->Cells[1][i]=="")
              {
                StrGrTabelka->Cells[2][i]="";
                StrGrTabelka->Cells[3][i]="";
                break;
              }
              StrGrTabelka->Cells[2][i]=Fo.FormatFloat("0.####", termopara.TempCelsjusz( StrGrTabelka->Cells[1][i].ToDouble()));
              StrGrTabelka->Cells[3][i]=Fo.FormatFloat("0.####",StrGrTabelka->Cells[2][i].ToDouble()+273.15);
              break;
     }
     i++;
     if (i>StrGrTabelka->RowCount) break;
    }
}
//---------------------------------------------------------------------------





void __fastcall TPrzelicznik::StrGrTabelkaSetEditText(TObject *Sender,
      int ACol, int ARow, const AnsiString Value)
{
        for(int i=1;i<4;i++)
        {
          if(i==ACol) continue;
          StrGrTabelka->Cells[i][ARow]="";
        }
}
//---------------------------------------------------------------------------

