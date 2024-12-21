//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RamkaOpcje.h"
#include "metamorfozis_w5.h"
#include "Wykres.h"
#include "WykresPunktowy.h"
#include "Punkty.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

using namespace PrzestrzenOpcji;
extern StructOpcje Opcje;

TOknoOpcje *OknoOpcje;
//---------------------------------------------------------------------------
__fastcall TOknoOpcje::TOknoOpcje(TComponent* Owner)
        : TForm(Owner)
{
  Opcje.CzytajRejestr();
  OpDodatDanych->ItemIndex=Opcje.ZapisywaneDane;
  SymbZaznaczony->ItemIndex=Opcje.DajSymZazn();
  SymbDoswiadczalny->ItemIndex=Opcje.DajSymDosw();
  RozmDoswiadczalny->Text=Opcje.DajRozmDosw();
  RozmZaznaczony->Text=Opcje.DajRozmZazn();
  Czulosc->Text=WykresPktForm->Polozenia->DajCzulosc();
  PokazPunkty->Checked=Opcje.PokazPunkty;
  //EditNumerPliku->Text=NumerPliku;
  UpDown1->Position = NumerPliku;
  UpDown1->Max=LiczbaWczytPlikow; // Ustawienie Maksymalnej wartoœci w "przegl¹darce wczytanych plików"
  LiczDanychCheckBox->Checked=Opcje.ZredukowanaLiczbaDanych;
  //ZapisTemperatury->Checked=Opcje.ZapisTemperatury;
  SeparatoryKolumn->ItemIndex=Opcje.DajSeparator();
  if( SeparatoryKolumn->ItemIndex==3) //Je¿eli Wybrano Inne
  {
     EditInneZnaki->Color=clWindow;
     EditInneZnaki->Enabled=true;
     EditInneZnaki->Text=Opcje.TekstInne;
  };
  PageControl1->ActivePageIndex= Opcje.AktywnaZakladka;
  switch(Opcje.Jedn)
        {
           case mT:      Jednostki->ItemIndex=0;
                        break;
           case Gs:     Jednostki->ItemIndex=1;
                        break;
        }
}
//---------------------------------------------------------------------------


void __fastcall TOknoOpcje::ButtonOpcje_OkClick(TObject *Sender)
{
  try{
       try{
      Opcje.UstSymZazn(SymbZaznaczony->ItemIndex);
      Opcje.UstSymDosw(SymbDoswiadczalny->ItemIndex);
      Opcje.UstRozmDosw(RozmDoswiadczalny->Text.ToInt() );
      Opcje.UstRozmZazn(RozmZaznaczony->Text.ToInt());
      Opcje.PokazPunkty=PokazPunkty->Checked;
      Form1->PunktyDoswiadczalne->Down=PokazPunkty->Checked;
      WykresPktForm->Polozenia->UstCzulosc(Czulosc->Text.ToInt());
      NumerPliku=EditNumerPliku->Text.ToInt();
      Opcje.ZredukowanaLiczbaDanych=LiczDanychCheckBox->Checked;
      //Opcje.ZapisTemperatury=ZapisTemperatury->Checked;
      Opcje.UstSeparator(SeparatoryKolumn->ItemIndex);
      if((SeparatoryKolumn->ItemIndex)==3) //Je¿eli Wybrano Inne
      {
         Opcje.TekstInne=EditInneZnaki->Text;
      };

      if(NumerPliku>LiczbaWczytPlikow)
         {
        /* Form1->Memo1->Show();
         Form1->Memo1->Lines->Add("");
         Form1->Memo1->Lines->Add("Plików jest mniej ani¿eli podano w opcji Numer Pliku");
         Form1->Memo1->Lines->Add(AnsiString("Liczba plików jest równa " + IntToStr(LiczbaWczytPlikow)));
         rysunek = false;
         return;*/
         NumerPliku=LiczbaWczytPlikow;
        }
      if(NumerPliku < 1)
        {
       /*  Form1->Memo1->Lines->Add("");
         Form1->Memo1->Lines->Add("Numer Pliku musi byc wiêkszy od zera");
         Form1->Memo1->Lines->Add(AnsiString("Liczba plików jest równa " + IntToStr(LiczbaWczytPlikow)));
         Form1->Memo1->Show();
         rysunek = false;
         return;*/
        NumerPliku=1;
        }
        for(int i=0;i<LiczbaWczytPlikow;i++)
          if( NumerPliku==TablicaNrPlikow[i])
               Form1->UpDown2->Position=i+1;    // Bo Pozycje liczone s¹ od zera a Numery od 1 ????
        delete Form1->Doswiadczalny;     // Ustaw Rodzaje Puntków
        delete Form1->Zaznaczenie;
        switch (SymbDoswiadczalny->ItemIndex)
        {
          case 0:
                  Form1->Doswiadczalny=new Kolo(Opcje.RozmDosw);
                    break;
          case 1:
                  Form1->Doswiadczalny=new Kwadrat(Opcje.RozmDosw);
                    break;
          default: ;
        }
        switch (SymbZaznaczony->ItemIndex)
        {
          case 0:
                  Form1->Zaznaczenie=new Kolo(Opcje.RozmZazn);
                    break;
          case 1:
                  Form1->Zaznaczenie =new Kwadrat(Opcje.RozmZazn);
                    break;
          default: ;
        }
        switch(Jednostki->ItemIndex)
        {
           case 0:      Opcje.Jedn=mT;
                        break;
           case 1:      Opcje.Jedn=Gs;
                        break;
        }
        Opcje.AktywnaZakladka =PageControl1->ActivePageIndex;
        Opcje.ZapisywaneDane= OpDodatDanych->ItemIndex;
        extern ObszarPunkty::TListaPunktow ZebranePunkty;
        ObszarPunkty::Punkt* Pkt;
        int NrPlik;
        ZbiorTablic* Akt;
        double Wart;
        while((Pkt=ZebranePunkty.DajNastepny() ) != 0 )
        {
                NrPlik=Pkt->DajNumerPliku();
                Akt=DajZb(NrPlik);
                Wart=Konwertuj(Akt->DajWartosc(NrPlik));
                Pkt->UstawWartosc(Wart);
        }


      }
      catch (EConvertError& d) //EConvertError
      {
       Application->MessageBox("UWAGA"," Opcja nie zawiera w³aœciwego formatu liczby ",MB_OK);
       NumerPliku=1;
       return;
      }
  }
  __finally  {
 // Invalidate();
  ModalResult=mrOk;
 // Close();
 }



}
//---------------------------------------------------------------------------
void __fastcall TOknoOpcje::ButtonAnuluj_OpcjeClick(TObject *Sender)
{
  ModalResult=mrCancel;
 // Close();
}
//---------------------------------------------------------------------------
void __fastcall TOknoOpcje::ButtonPomoc_OpcjeClick(TObject *Sender)
{
/*if( */
   Application->MessageBox("Niestety Pomoc w Fazie Przygotowañ. Ale jest uwzglêdniona",
     "Informacja Optymistyczna",MB_OK);
     //==MB_OK)
Application->MessageBox("Realzacja uwzglêdniona w nastêpnym planie 5-cio letnim",
     "Informacja Niezbyt Optymistyczna",MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TOknoOpcje::SeparatoryKolumnClick(TObject *Sender)
{
 if(SeparatoryKolumn->ItemIndex==3)
 {      EditInneZnaki->Color=clWindow;
        EditInneZnaki->Enabled=true;
 }
 else
 {
        EditInneZnaki->Enabled=false;
        EditInneZnaki->Color=clInactiveCaption;
 }
}
//---------------------------------------------------------------------------
