//---------------------------------------------------------------------------

#ifndef StrukturaOpcjeH
#define StrukturaOpcjeH
#include <vcl.h>
//---------------------------------------------------------------------------
typedef enum {mT,Gs} TJednostki;
namespace PrzestrzenOpcji
{

  enum Zakladki {Wykres,Zapisywanie};
  enum Zapisz   {TemperaturaK=0,TemperaturaC=1,Anizotropia=2,Inne=3};
  //Wartoœci odpowiadaj¹ Indeksom OpDodatDanych GroupBox'a
}
using namespace PrzestrzenOpcji;
struct StructOpcje
{
  int  AktywnaZakladka; // indeks aktywnej zak³adki opcjii
  bool RejestrPrzeczytany;
  enum Separatory {Przecinek=0,Tabulacja=1, Spacja=2, Inne=3 } SeparatorKolumn;
  char ZnakSeparatora;
  enum Symbole {Kolo=0, Kwadrat=1};
  Symbole SymZazn;
  Symbole SymDosw;
  int SymZaznIndeks, SymDoswIndeks;
  int RozmDosw, RozmZazn;
  bool PokazPunkty;
  int NumerPliku;
  bool ZredukowanaLiczbaDanych;
  bool ZapisTemperatury;
  AnsiString TekstInne;
  StructOpcje();
  void CzytajRejestr();
  Separatory DajSeparator();
  void UstSeparator(Separatory);
  Symbole DajSymZazn();
  void UstSymZazn(Symbole);
  Symbole DajSymDosw();
  void UstSymDosw(Symbole);
  int UstRozmDosw(int i);
  int UstRozmZazn(int i);
  int DajRozmDosw();
  int DajRozmZazn();
  Zapisz ZapisywaneDane;
   int czulosc;
   TJednostki Jedn;
};

extern int NumerPliku;
#endif
