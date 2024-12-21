//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "StrukturaOpcje.h"
#include "metamorfozis_w5.h"
#include "Rejestr.h"



StructOpcje Opcje;

StructOpcje::StructOpcje()
{
  ZapisywaneDane=Inne;
  AktywnaZakladka=1;
  RejestrPrzeczytany=false;
  SymZaznIndeks=SymZazn=Kwadrat;
  SymDoswIndeks=SymDosw=Kolo;
  RozmDosw=2;
  RozmZazn=4;
  ZredukowanaLiczbaDanych=false;
  ZapisTemperatury=false;
  PokazPunkty=false;
  NumerPliku=::NumerPliku;
}


void StructOpcje::CzytajRejestr()
{
  if(RejestrPrzeczytany) return;

  char Bufor[21];
  rejestr->open();
  if (rejestr->ValueExists("Separator Kolumn"))  // Wczytanie zapamiêtanego separatora kolumn
  {
    //strcpy(Bufor,rejestr->ReadString("Separator Kolumn").c_str());
    strcpy(Bufor,rejestr->odczytStr("Separator Kolumn").c_str());
    //int NumerSeparatora = rejestr->ReadInteger("NrSep");
    int NumerSeparatora = rejestr->odczytInt("NrSep");
    UstSeparator(NumerSeparatora);
    if(NumerSeparatora==Inne)
    //   TekstInne=rejestr->ReadString("EditInne");
    //TekstInne=rejestr->ReadString("Separator Kolumn");
            TekstInne=rejestr->odczytStr("Separator Kolumn");
    else
    {
      ZnakSeparatora=Bufor[0];
    }
  }
        rejestr->open();
	if(rejestr->ValueExists("Aktywna Zak³adka"))
		// AktywnaZakladka=rejestr->ReadInteger("Aktywna Zak³adka");
		AktywnaZakladka=rejestr->odczytInt("Aktywna Zak³adka");
        rejestr->open();
	if(rejestr->ValueExists("Indeks Dodatkowych Danych"))
		 //ZapisywaneDane=rejestr->ReadInteger("Indeks Dodatkowych Danych");
		 ZapisywaneDane=rejestr->odczytInt("Indeks Dodatkowych Danych");

  RejestrPrzeczytany=true;
}


int StructOpcje::UstRozmDosw(int i)
{
  if (i<2) i=2;
  else if (i>20) i=20;
  return RozmDosw=i;
}

int StructOpcje::UstRozmZazn(int i)
{
  if (i<2) i=2;
  else if (i>20) i=20;
  return RozmZazn=i;
}

int StructOpcje::DajRozmDosw()
{
  return RozmDosw;
}

int StructOpcje::DajRozmZazn()
{
  return RozmZazn;
}


StructOpcje::Separatory StructOpcje::DajSeparator()
       {
         return SeparatorKolumn;
       }

void StructOpcje::UstSeparator(Separatory S)
{
  SeparatorKolumn=S;
  switch (SeparatorKolumn)
   {
     case Przecinek: ZnakSeparatora=',' ;
                        break;
     case Tabulacja: ZnakSeparatora='\t';
                        break;
     case Spacja: ZnakSeparatora=' ';
                        break;
     default:  ZnakSeparatora=',' ;
                        break;
   }
}

void StructOpcje::UstSymZazn(Symbole S)
{
  SymZazn=S;
}

StructOpcje::Symbole StructOpcje::DajSymZazn()
{
  return SymZazn;
}

StructOpcje::Symbole StructOpcje::DajSymDosw()
{
  return SymDosw;
};
  void StructOpcje::UstSymDosw(Symbole S)
  {
    SymDosw = S;
  };
//---------------------------------------------------------------------------



