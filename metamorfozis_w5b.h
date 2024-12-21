#ifndef metamorfozis_w5bH
#define metamorfozis_w5bH
#define odpluskwianie

#include <fstream.h>
#include <iomanip.h>
#include <except.h>
#include <values.h>
#include <math.h>
#include <vcl\StdCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Classes.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\Menus.hpp>
#include <vcl\Dialogs.hpp>
#include <Forms.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include <ActnList.hpp>
#include <sys\stat.h>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Classes.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\Menus.hpp>
#include <vcl\Dialogs.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Classes.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\Menus.hpp>
#include <vcl\Dialogs.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Classes.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\Menus.hpp>
#include <vcl\Dialogs.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Classes.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\Menus.hpp>
#include <vcl\Dialogs.hpp>
#include <ComCtrls.hpp>
//#include <strstream.h>
#include <strstrea.h>
#include <vcl\registry.hpp>
#include "termopara.h"
#include "Wykres.h"
#include "StrukturaOpcje.h"
//#include "Rejestr.h";

//#include "Unit1.h"//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class TJTryby;
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu1;
        TMenuItem *Pliki;
        TMenuItem *Wczytaj1;
        TOpenDialog *OpenDialog1;
        TMenuItem *N1;
        TMenuItem *Koniec1;
        TSaveDialog *SaveDialog1;
        TMenuItem *Zapisz1;
        TMenuItem *Zbiorczy1;
        TMenuItem *PlikizKropk1;
        TMenuItem *Opcje1;
        TMenuItem *Linie;
        TStatusBar *StatusBar1;
        TMenuItem *Wypisz1;
        TMenuItem *Wykres1;
        TMenuItem *UsuSzpilki1;
        TMenuItem *AktualnaLinia1;
        TMenuItem *WszystkieLinie1;
        TSaveDialog *SaveDialog2;
        TMenuItem *Polecenia1;
        TMenuItem *Sprawdzanie1;
        TMenuItem *Wypisz2;
        TMenuItem *Termopara1;
        TMenuItem *EksportujListy1;
        TSaveDialog *EksportDialog;
        TMemo *Memo1;
        TScrollBox *ScrollBox1;
        TToolBar *ToolBar1;
        TUpDown *UpDown2;
        TToolButton *ToolButton1;
        TImageList *ListaIkon;
        TToolButton *PrzycWstPunkt;
        TToolButton *ToolButton3;
        TToolButton *ToolButton4;
        TToolButton *ToolButton5;
        TActionList *ActionList1;
        TAction *Wykres;
        TToolButton *ToolButton6;
        TToolButton *PrzycWstSzer;
        TPaintBox *Image1;
        TMenuItem *WykresPunkty1;
        TAction *WykresPkt;
        TToolButton *ToolButton2;
        TToolButton *ToolButton7;
        TToolButton *ToolButton8;
        TAction *ActionWczytaj;
        TMenuItem *Nazwy1;
        TToolButton *ToolButton9;
        TToolButton *PunktyDoswiadczalne;
        TToolButton *btnZapisSesji;
        TSaveDialog *SaveSesji;
        TOpenDialog *OpenSesje;
        TMenuItem *OtwarcieSesji;
        TToolButton *ToolButton10;
        TAction *OtwSesji;
        TToolButton *PrzycPoziomZaznaczania;
        TMenuItem *ZapisSesjiMenu;
        TToolButton *ToolButtonScalaGlobalLocal;
        TLabel *LabelDane;
        TToolButton *ToolButton11;
        TMenuItem *Etykieta;
        TFontDialog *FontDialog1;
        TToolButton *ToolButton12;
        TToolButton *ToolButton13;
        TMenuItem *Kszta1;
        TMenuItem *Zarejestruj1;
        void __fastcall Wczytaj1Click(TObject *Sender);
        void __fastcall Koniec1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Zbiorczy1Click(TObject *Sender);
        void __fastcall PlikizKropk1Click(TObject *Sender);
        void __fastcall Opcje1Click(TObject *Sender);
        //void __fastcall Panel1Exit(TObject *Sender);
        void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall Wypisz1Click(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall Wykres1Click(TObject *Sender);
        void __fastcall AktualnaLinia1Click(TObject *Sender);
        void __fastcall WszystkieLinie1Click(TObject *Sender);
        void __fastcall UpDown2Click(TObject *Sender, TUDBtnType Button);
        void __fastcall Wypisz2Click(TObject *Sender);
        void __fastcall Termopara1Click(TObject *Sender);
        void __fastcall EksportujListy1Click(TObject *Sender);
        void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift,
          int WheelDelta, TPoint &MousePos, bool &Handled);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ToolButton1Click(TObject *Sender);
        void __fastcall PrzycWstPunktClick(TObject *Sender);
        void __fastcall ToolButton4Click(TObject *Sender);
        void __fastcall ToolButton5Click(TObject *Sender);
        void __fastcall ToolButton6Click(TObject *Sender);
        void __fastcall PrzycWstSzerClick(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall WykresPunkty1Click(TObject *Sender);
        void __fastcall Nazwy1Click(TObject *Sender);
        void __fastcall ToolButton9Click(TObject *Sender);
        void __fastcall PunktyDoswiadczalneClick(TObject *Sender);
        void __fastcall btnZapisSesjiClick(TObject *Sender);
        void __fastcall OtwarcieSesjiClick(TObject *Sender);
        void __fastcall PrzycPoziomZaznaczaniaMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButtonScalaGlobalLocalClick(TObject *Sender);
        void __fastcall LabelDaneMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
		void __fastcall Image1DragOver(TObject *Sender, TObject *Source,
          int X, int Y, TDragState State, bool &Accept);
        void __fastcall ToolButton11Click(TObject *Sender);
        void __fastcall ToolButton12Click(TObject *Sender);
        void __fastcall ToolButton13Click(TObject *Sender);
        void __fastcall Image1EndDrag(TObject *Sender, TObject *Target,
          int X, int Y);
        void __fastcall Kszta1Click(TObject *Sender);
        void __fastcall Zarejestruj1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
		void Wczytaj();
		bool Sprawdz();
		void WczytajMLA();
		void WczytajDAT();
		void WczytajEPR();
		void RysujPoziomZaznaczania(int=0);
        Symbol *Doswiadczalny;
        Symbol *Zaznaczenie;
        int RozmDosw, RozmZazn;
        Kwadrat SymKwadrat;
        Kolo SymbolKola;
        char ZnakKolumny[20];
        int NumerSeparatora;
        Graphics::TBitmap *MBit;
        __fastcall TForm1(TComponent* Owner);
        void __fastcall rysuj(TObject *Sender);
        friend class TJTryby;
        TListBox* ListBox1;
        TPanel* Panelek;
        int X0,Y0;  // Dane dla LabelDane do przesuwania etykiety z Temperaturami
        

};

extern TForm1* Form1;
/*
class TFontyDlaHinta:public THintWindow  // Klasa stworzona po to aby dobrze dzialaly podpowiedzi
{
  TFontyDlaHinta():THintWindow(Form1->Handle)
        {
                Font->Charset=EASTEUROPE_CHARSET;
                Font->Size=14;
        }
};
*/

// **************************************************************
// ******************  Deklaracjie globalne *********************
enum EnTryby {ZaznaczaniePolRezonansowych,ZaznaczanieSzerokosci,Nieokreslony };

class TJTryby
{
   EnTryby  AktTryb;
public:
   TJTryby() {AktTryb=Nieokreslony;}
   EnTryby Ustaw(EnTryby Akt)
     {
            switch(Akt)
            {
              case ZaznaczaniePolRezonansowych:
                if( AktTryb==ZaznaczanieSzerokosci)
                       Form1->PrzycWstSzer->Down=false;
                break;
              case ZaznaczanieSzerokosci:
                if( AktTryb==ZaznaczaniePolRezonansowych)
                       Form1->PrzycWstPunkt->Down=false;
                break;
           }
           AktTryb=Akt;
           Form1->Invalidate();
           return AktTryb;
          }

   EnTryby DajTryb() {return AktTryb;}

   TJTryby& operator=(EnTryby T)
                 {Ustaw(T); return *this;}

    operator EnTryby() {return AktTryb;}
//   friend bool operator==(EnTryby& T,TJTryby& KlT);
//   friend bool operator==(TJTryby& KlT,EnTryby& T);

};

extern TJTryby Tryb;
 /*
   bool operator==(EnTryby& T,TJTryby& KlT)
    {
      if(T==Klt.AktTryb)
          return true;
      else
          return false;
    }


 bool operator==(TJTryby& TlT, EnTryby& T)
 {
    if(T==Klt.AktTryb)
          return true;
      else
          return false;
 }
   */
 //Klasa implementuje listê punktów
// zaznaczonych na wykresie dla pojedynczej
// linii
class Punkty
{
 Punkty* Znajdz(int);
public:
 int indeks;  //indeks zaznaczonego pola
 Punkty* poprzedni;
 Punkty* nastepny;
 bool Kasuj(int);
 int UsunWszystkie();
 bool Dodaj(int);
  Punkty();
};

Punkty::Punkty()
{
 poprzedni = NULL;
 nastepny  = NULL;
 indeks = -1;     // Mo¿na testowaæ czy wstawiono indeks tablicy
}

bool Punkty::Dodaj(int ind)
{
  Punkty* tym = this, *Nowy=0;
  Punkty* TymPop=this;
  while(tym->nastepny)
  {
    TymPop=tym;
    tym = tym->nastepny;
    if (tym->indeks==ind)
       return false;     //Je¿eli wstawiono ju¿ punkt pod takim X-sem o takich wspó³rzêdnych
    if (ind < tym->indeks)
    {  // Wstawiamy w posortowan¹ rosn¹co listê
       Nowy=new Punkty;
        TymPop->nastepny = Nowy;
        Nowy->poprzedni = TymPop;
        Nowy->indeks = ind;
        Nowy->nastepny = tym;
        tym->poprzedni = Nowy;
        return true;
    }
  }
  Nowy = new Punkty;    // Wstawianie na koniec listy
  tym->nastepny = Nowy;
  Nowy->poprzedni = tym;
  Nowy->indeks = ind;
  Nowy->nastepny = 0;
  return true;
}

Punkty* Punkty::Znajdz(int ind) // Funkcja szuka najblizszego indeksu
                                // Je¿eli nie znaleziono zwraca NULL
{
  Punkty* tym = nastepny;  //Zaczynamy zawsze od g³owy listy
  Punkty* wynik=NULL;
  int min= MaxInt;//MAXINT;
  while(tym)
  {
   if(abs(ind - tym->indeks)< min)
   {
     min = abs(ind - tym->indeks);
     wynik = tym;
   }
   tym = tym->nastepny;
  }
  return wynik;
}

bool Punkty::Kasuj(int ind) //Usuwa zaznaczony punkt
                                // Jezeli Punkt=NULL zwraca 0

{
/* TODO : K³opoty z linijkami z  W Punkty::Kasuj */
  if(!nastepny)     //Je¿eli lista Punktów Pusta WyjdŸ
         return false;
  Punkty* Punkt, *Pop=0,*Nast=0;
  Punkt = Znajdz(ind);
  if(Punkt)
  {
   /*if(Punkt->nastepny) Punkt->nastepny->poprzedni=Punkt->poprzedni;
   Punkt->poprzedni->nastepny=Punkt->nastepny;
   ind = Punkt->indeks; */
   Nast=Punkt->nastepny;
   Pop=Punkt->poprzedni;
   Pop->nastepny=Nast;
   if(Nast) Nast->poprzedni=Pop;
   delete Punkt;
   return true;
  }
  return false; //  Je¿eli Nie Znaleziono ¯adnego Punktu
}

int Punkty::UsunWszystkie()
{
   if(!nastepny)     //Je¿eli lista Punktów Pusta WyjdŸ
         return 0;
  int LiczUsun=0;
  Punkty* P=nastepny, *Pop=0;
  while(P->nastepny)
      P=P->nastepny;  // Ustaw siê na koniec listy
  while(P->poprzedni)
  {
    Pop=P->poprzedni;
    delete P;
    LiczUsun++;
    P=Pop;
    P->nastepny=0;
  }
  return LiczUsun;
}

extern /*PACKAGE*/ TForm1 *Form1;
// **** Dane potrzebne do konwersji Real -> Double
typedef unsigned char real[6];  //typ real bez znaku
//real *liczba;
double RealToDouble(real Liczba); // Deklaracja zapowaiadaj¹ca - definicja w Funkcje.cpp
//*********



class TabAsocInt
{
  double* Y;              // Wartoœci sygna³u EPR
  int AktNumer;          // Lokalny Numer pliku tzn.dla danego zakresu
  int NumerPliku;        // unikalny numer pliku, inny dla wszystkich plików wczytanych
  double Wartosc;        // Np. temperatura, lub k¹t dla anizotropii
  TabAsocInt* Nast;
  Punkty* Szer;       //Przechowuje indeksy Œrodków Linii
  time_t Czas;
  int IndMax,IndMin;   // Indeksy Masymalnego i Minimalnego Punktu;
  float MaxY,MinY;

  bool zarejestrowany;  // Czy wartoœci dla tego pliku zosta³y zarejestowane
public:
  int dBMin,dBMax;
  TabAsocInt* DajAkt(int Akt)
              {
                for(TabAsocInt* tym=this;tym!=NULL;tym=tym->Nast)
                                   if(tym->NumerPliku==Akt) return tym;
                return 0; // w wypadku niepowodzenia
              }

  double DajWartosc(int Akt) {for(TabAsocInt* tym=this;tym!=NULL;tym=tym->Nast)
                                   if(tym->NumerPliku==Akt) return tym->Wartosc;
                              return -2000;} // w wypadku niepowodzenia.

  TabAsocInt() {AktNumer=-1;NumerPliku=-1;Nast=NULL;zarejestrowany = false;
                 Szer=new Punkty;}
  TabAsocInt(int Akt,int Plik)
        {Nast = new TabAsocInt;Nast->AktNumer=Akt; Nast->NumerPliku=Plik;
                zarejestrowany=false; Szer=new Punkty;}
  double* Dodaj(int Akt, int Plik, AnsiString Nazwa, int rozm);
  double* DodajDat(int Akt, int Plik, AnsiString Nazwa, int LWierszy, int Rozmiar);
  double* DodajEPR(int Akt, int Plik, AnsiString Nazwa, int LWierszy, int Rozmiar);
  int SzukajNrPliku(int Akt) { for(TabAsocInt* tym=this;tym!=NULL;tym=tym->Nast)
                                   if(tym->AktNumer==Akt) return tym->NumerPliku;
                               return -2;  // w wypadku nie powodzenia
                             }
  int SzukajAktNumer(int Plik) { for(TabAsocInt* tym=this;tym!=NULL;tym=tym->Nast)
                                   if(tym->NumerPliku==Plik) return tym->AktNumer;
                               return -1;  // w wypadku nie powodzenia
                             }
  void Zwolnij();
  double* WartosciY(int Akt) {  for(TabAsocInt* tym=this;tym!=NULL;tym=tym->Nast)
                                   if(tym->AktNumer==Akt)  return tym->Y;
                                return 0; } // je¿eli nie ma co przypisaæ
  float DajNastSzerokosc(int Akt);
  bool UstSzerokosc(int Akt, int i);   //i- wiersz Wartosci
  TPoint DajIndeksyPunktSzer(int Akt,int i);
  bool DodajMaxMin(int Akt,int i) { if (DajAkt(Akt)->IndMax < 0)
                                               IndMax=i;
                                    else  if (DajAkt(Akt)->IndMin < 0)
                                               IndMin=i;
                                          else
                                             return false;   // Oba IndeksyUstawione;
                                    return true;      // Je¿eli Ustawiono Ktorys z Indeksow
                                  }
  bool KasujMaxMin(int Akt, int i);


};




class ErrorDajY {};
class ErrorDajX {};
class ErrorNazwaZb {};
class ErrorDajZb{};
class ErrorZakres
{

public:
  enum TRodzaj {X,Y} Rodzaj ;
  int indeks;
  ErrorZakres(int i):indeks(i){}
  ErrorZakres(int i, TRodzaj R):indeks(i),Rodzaj(R){};
};

class ZbiorTablic       // Zbiór (kolekcja) tablic o ró¿nych
						// wartoœciach zakresów pól
{
      double   X1,X2;   //  Pierwsze dwie wartoœci pól
      int rozmiar;      // liczba wierszy
      int ilosc;        // iloœæ plików
      TStringList* Pliki; // lista plików z wartoœciami
      ZbiorTablic* nast;
      static int MaxRozmiar;
      int NrAktPliku;     // Indeksuje napisy w liœcie Pliki
      bool WczytaneX ;     // Czy wczytano wartoœci x dla kolekcji
      double* X;           // zbiór wartoœci X-sów.
      double** Tab;        // tablica z danymi o tym samym zakresie pól
                           // Tab[0] x-sy wierszami
                           // Tab[1] Wartoœci pól pierwszego zbioru (NrAktPliku=0)
                           // ... itd.
      TabAsocInt* Asoc;
      int RozmiarPola;     // Liczba elementow tablicy Pola

 public:
       static double GlobalMaxX;
       static double GlobalMaxY;
       static double GlobalMinX;
       static double GlobalMinY;  
       static ZbiorTablic* glowa;
       // Tablica(y,x) z odwrotnym uporz¹dkowaniem kolumn i wierszy
       //dla zachowania czêœciowej zgodnoœci z poprzednim kodem
       double Tablica(int i,int j)throw (ErrorZakres) {
                                            if(j<0 && j>=LiczKolumn())
                                                throw ErrorZakres(j,ErrorZakres::X);
                                            if(i<0 && i>=LiczWierszy())
                                                 throw ErrorZakres(i,ErrorZakres::Y);
                                            return *(*(Tab+ j) + i); }
       double PrzypiszStTab(int y, int x, double wart)      // Przypisanie do tej tablicy
                                { return Tab[x][y]=wart;
                                }
       double PrzypiszTab(int kol, int wiersz, double wart)
                                { return Tab[kol][wiersz]=wart;
                                }

       Punkty* Pola;       // pola rezonansowe
       void Dodaj(AnsiString s){Pliki->Add(s);ilosc++;}
       void Dodaj(AnsiString s,int i);
       void Dodaj(AnsiString, int, long RozmiarPliku);  // Dodawanie zbiorów tekstowych
     //  void DodajPlikDat(AnsiString s,int i,int rozm);
       ZbiorTablic() {Inicjuj();}
       ZbiorTablic(char* s) {Inicjuj();Pliki->Add(s);ilosc++; }
       ZbiorTablic(AnsiString s) {Inicjuj();Dodaj(s);}
       ZbiorTablic(AnsiString s, int i) {Inicjuj(); Dodaj(s,i);}
       void Inicjuj() {
                        nast = NULL; Tab=NULL; rozmiar =0;ilosc=0;
                        NrAktPliku=0;Pliki= new TStringList; Asoc= new TabAsocInt;
                        WczytaneX = false;
                        RozmiarPola = 0;
                      }
       AnsiString NazwaPliku(){ if (NrAktPliku>=ilosc+1) NrAktPliku=0;
                                return Pliki->Strings[NrAktPliku++];
                               }
       AnsiString NazwaPliku(int i) {return Pliki->Strings[i];}
       AnsiString NazwaPlikuGlobal(int i) {return Pliki->Strings[DajIndeks(i)];}
       String Szukaj(int i) { int indeks = Asoc->SzukajAktNumer(i); // ZnajdŸ Napis skojarzony
                              if (indeks>=0)                        // z Unikalnym Nr Pliku
                                 return  Pliki->Strings[indeks];
                              return NULL;
                            }
       int SzukajNr(int i) {return Asoc->SzukajNrPliku(i);}    // ZnajdŸ Unikalny Nr Pliku skojarzony z Indeksem napisu
       int DajIndeks(int i) { return Asoc->SzukajAktNumer(i);} // Zwróæ lokalny indeks odpowiadaj¹cy globalnemu numerowi
       double DajWartosc(int i) {return Asoc->DajWartosc(i);}  // Liczba zawarta w nazwie pliku o globalnym numerze i
                                                               // w wypadku niepowodzena zwraca -2000
       bool KasujMaxMin(int NrPlik, int i)
                        { return Asoc->KasujMaxMin(NrPlik,i);}                                                   // W wypadku nie mo¿nosci wczytania wartosci zwraca -3000
       bool DodajMaxMin(int NrPlik,int i)
                  { return Asoc->DodajMaxMin(NrPlik,i);}    //Ustawiane Indeksu
       TPoint DajSzer(int NrPlik,int i) {return Asoc->DajIndeksyPunktSzer(NrPlik,i);}
       int Szukaj(String S) { for(int i=0;i<ilosc;i++) // ZnajdŸ Indeks napisu
                                    if(S==Pliki->Strings[i]) return i;
                                 return -1;} // gdy nie znaleŸiono
// ****** DO ZDEFINIOWANIA  *****************************
       ZbiorTablic* DajZbTab(int NrPlik) { return NULL; };
//  *****************************************************
       ZbiorTablic* DajNast() {return nast;}
       ZbiorTablic* Ogon() { ZbiorTablic* akt = glowa;
                             while(akt->nast) akt=akt->nast;
                             return akt; }
       void PrzydzielPola() ;
       double DajX1(){return X1;}
       double DajPrzedzial() {return (X2-X1)*rozmiar;}
       int LiczWierszy(){return rozmiar;}
       int LiczKolumn() {return ilosc+1;}   //liczba kolumn = liczbie plików + kolumna X-sów;
       int LiczWczytanychPlikow() { return (Pliki->Count);}
       bool Ustaw();
       int MaxRozm() {return MaxRozmiar;}
       void PrzypiszTablice(double** T) {Tab=T;}
       friend ZbiorTablic* DodajPlik(FILE*,AnsiString&,int);  // Funkcja sprawdza zakres i dodaje plik do
	   friend ZbiorTablic* DodajDat(FILE*,AnsiString&,int);  // Funkcja sprawdza zakres i dodaje plik do
       friend ZbiorTablic* DodajEPR(FILE*,AnsiString&,int);
	   TabAsocInt* DajLinie(int NrP) {return Asoc->DajAkt(NrP);}                                       // odpowiedniego zbioru
       friend double DajY(int a,int NrPliku)throw(ErrorDajY);
       // a - jest indeksem wiersza. NrPliku s³u¿y tylko do identyfikacji w³aœciwego ZbioruTablic
       friend double DajX(int a,int NrPliku)throw(ErrorDajX);
       friend String DajNazweZbioru(int NrPliku)throw(ErrorNazwaZb);
       friend ZbiorTablic* DajZb(int NrPlik)throw(ErrorDajZb);  // Zwraca ZbiorTablic zawieraj¹cy linie o danym numerze globalnym
       ~ZbiorTablic();
};





//*******************************************************************
// ***************        Deklaracje Globalne    ********************

//---------------------------------------------------------------------------
//extern class TRejestr;
extern float PowiekszX;                       //Tymczasowy parametr do powiêkszania Wykresu w X-sie
const char kropka = '\0';
extern  TStringList *ListaPlikow;
extern AnsiString    katalog;
extern bool zamrozonypanel;
extern char bufor_wzorca_bitowego[80];
extern strstream napis2;
extern long LiczbaWierszy;
extern int LiczbaWczytPlikow;
extern double **TablicaZbiorcza;
extern bool rysunek;
extern double WspX;    //Aktualny wspó³czynnik skalowania dla wyresu;
extern double WspY_B,WspY_C;
extern int popr; // Poprawka w pikselach do wysokoœci okna
extern double *TablicaWart[2]; //Tablica par NrPliku, StowarzyszonaWartoŒÆ
extern int *TablicaNrPlikow;   // Posortowana Tablica Numerów Plików
extern int LiczbaZebranychSzerokosci;
extern int LiczbaZebranychPolozen;
extern TJWykresyPunktow* Polozenia;;


//******************************************************************************
//************************  UWAGA!!!  ******************************************
        extern int NumerPliku;
//*  Globalny Numer linii aktualnie widocznej na wykresie
//*  i identyfikuj¹cy WSZÊDZIE tê liniê.
//*********************  BARDZO WA¯NY PARAMETR  ********************************
//******************************************************************************

TPoint SzukajNajblizszego(TCanvas*,ZbiorTablic* Akt,int X, int Y); // Funkcja poszukuje le¿¹cego najbli¿ej
                                                          // punktu (X,Y) punktu wykresu
 int SzukajNajblIndeksu(ZbiorTablic* Akt,int X, int Y);
 extern TJWykresyLinii *AktWykrLinii;
 extern struct StructOpcje Opcje;
 void EksportujMathematica(const char*);
 void EksportujDat(const char*);
 void EksportujSzerDat(const char* nazwa);
 void EksportujIntensywnoscDat(const char* nazwa);
 void Sortuj(int Tab[],int rozm,char kierunek);
 int DajPunktBMax(int NrP);
 
int DajPunktBMin(int NrP);
 void ZapisSesji(void);
 void OtwarcieZapisanejSesji(char*);
int UstPunktBMax(int NrP, int Wart);


int UstPunktBMin(int NrP, int Wart);

//---------------------------------------------------------------------------
#endif
