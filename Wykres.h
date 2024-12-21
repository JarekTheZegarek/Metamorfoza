//---------------------------------------------------------------------------
#ifndef WykresH
#define WykresH
#include <vcl.h>
#include <math.h>
#include <list>

//#include "metamorfozis_w5.h"


void InicjujNazwySymboli();       // prototyp funkcja wywolana w konstruktorze
                                  // TWykresPktForm::TWykresPktForm

class ZbiorTablic;

class TJWspolnyWykres
{
protected:
   TForm* Okno;
   TPaintBox* Sztaluga;
   int MaxX, MaxY;         // Rozmiary P³ótna
   int BazoweMaxX, BazoweMaxY; //Rozmiary widocznego okna;
   double WspX, WspY;      // Wspó³czynniki skalowania
   double MaxWartX, MaxWartY,MinWartX, MinWartY; // Zakres rysowanych zmiennych
   double BezwMaxX, BezwMaxY,BezwMinX, BezwMinY; // Bezwzglêdne minimum i maksimum X i Y.
   TColor KolorLinii;
   TCanvas *Plotno;
   Graphics::TBitmap *B;      // Wirutalne okno do rysowania
   float Odst;                // Odstêp od krawêdzi w procentach
   int dX, dY;                // Odstêpy od zewnêtrznych krawêdzi;
   int wys,szer;              // wysokoœæ i szerokoœæ obszaru wyrkesu;
   int Powiekszenie;
   float SkokPowiekszenia;    //
 public:
   TJWspolnyWykres(){B=0;KolorLinii=clRed;};   // Do zdefiniowania poŸniej
   TJWspolnyWykres(TPaintBox *P)
       { Sztaluga = P; BazoweMaxX=MaxX =P->ClientWidth;  BazoweMaxY=MaxY=P->ClientHeight;
         SkokPowiekszenia=0.1;
         B = new Graphics::TBitmap;  B->Height=MaxY;
         B->Width=MaxX;  Plotno= B->Canvas;KolorLinii=clRed;
         Odst=0.04; dX=dY=0; wys=MaxY; szer=MaxX;
       }
   TJWspolnyWykres(TForm *P)
       { Okno = P; BazoweMaxX=MaxX =P->ClientWidth;  BazoweMaxY=MaxY=P->ClientHeight;
         SkokPowiekszenia=0.1;
         B = new Graphics::TBitmap;  B->Height=MaxY;
         B->Width=MaxX;  Plotno= B->Canvas;KolorLinii=clRed;
         Odst=0.04; dX=dY=0; wys=MaxY; szer=MaxX;
       }
   TJWspolnyWykres(Graphics::TBitmap *P)
       { BazoweMaxX=MaxX =P->Width;  BazoweMaxY=MaxY=P->Height;
         B = P;  KolorLinii=clRed;
         SkokPowiekszenia=0.1;
         Plotno= B->Canvas;
         Odst=0.04; dX=dY=0; wys=MaxY; szer=MaxX;
       }
   void OdswierzBitMap() {
                          delete B;
                          B = new Graphics::TBitmap;  B->Height=MaxY;
                          B->Width=MaxX;  Plotno= B->Canvas;
                         }
   TPoint DajRozmiarPl() {return TPoint(MaxX,MaxY);}
   void UstawRozmiarPl(TControl* O) {szer=MaxX = O->ClientWidth;
                          wys=MaxY= O->ClientHeight;
                          dY =int(wys*Odst);  // Zmniejszenie o 2% wysokosci
                          wys = wys-2*dY;
                          dX =int(szer*Odst);  // Zmniejszenie o 2% wysokosci
                          szer = szer-2*dX;
                          if(MaxWartX==MinWartX)
                              WspX=1;
                          else
                             WspX = double(szer)/fabs(MaxWartX-MinWartX) ;
                          if(MaxWartY==MinWartY)
                             WspY=1;
                          else
                             WspY = double(wys)/fabs(MaxWartY-MinWartY);
                         }
   void UstawRozmiarPl() {UstawRozmiarPl(Okno);}
   void UstMaxMinY(double maks, double min)
                          { MaxWartY=maks; MinWartY=min;}
   void UstawKolorLinii(TColor K){KolorLinii=K;
                                  if(B) Plotno->Pen->Color=K;}
   TColor DajKolorLinii(){return KolorLinii;};
   double Odleglosc(int X1, int Y1, int X2, int Y2);
   virtual TPoint  DajWsp(double WartX, double WartY)=0;   // Oblicz Wspolrzedne Ekranowe Punktu
   virtual double  DajWartX(int X)=0;                     // zwróæ wartoœæ opowiadaj¹c¹ WspEkranowej X
   virtual double  DajWartY(int Y)=0;
   virtual void rysuj()=0;
};

class TJWykresyLinii:public TJWspolnyWykres
{
  int ZakresZaznaczania;  // Okreœla "czu³oœæ" zaznaczania w pikselach
 public:
  TJWykresyLinii(TForm* P):TJWspolnyWykres(P){ZakresZaznaczania=5;};
  TJWykresyLinii(Graphics::TBitmap *P):TJWspolnyWykres(P){ZakresZaznaczania=5;};
  TJWykresyLinii(){ZakresZaznaczania=5;};
  int DajZakrZazn() {return ZakresZaznaczania;}
  void UstZakrZazn(int Z){ZakresZaznaczania=Z;}
  TPoint DajWsp(double WartX, double WartY); //pierwsza wartosc jest indeksem punktu w tablicy
  double DajWartX(int X);
  double DajWartY(int Y);
   void rysuj() {}
  friend int SzukajNajblIndeksu(ZbiorTablic* Akt,int X, int Y);
  //funkcja SzukajNaj... zwraca -1 w wypadku niepowodzenia
  //w innym wypadku Tablicowy Indeks  Punktu Doswiadczalnego.
};

struct ErrorRysowania {

      ErrorRysowania() {
         Application->MessageBox("Nie zdefiniowano obiektu na którym mo¿na rysowaæ",
                                 "B³¹d Rysowania",MB_OK);
         }
      };

class Symbol
{
  TColor StaryKolorLinii,StaryKolorWyp;
  TCanvas *P;                   // P³ótno Wykresu
protected:
  Graphics::TBitmap *KanwaSym;  // P³ótno Na przechowanie Symbolu;
  bool Odswierz;                // Czy stworzyc now¹ mapê bitow¹ symbolu?
  int rozmiar;
  TPoint Srodek;
  TColor KolorLinii, KolorWyp;    // Kolory Pen i Brush
  std::string NazwaSymbolu;
public:
  Symbol(): Odswierz(true),P(0),KanwaSym(0), KolorLinii(clBlack),KolorWyp(clWhite), Srodek(0,0){};
  Symbol(TPoint S):Odswierz(true),P(0), KanwaSym(0),KolorLinii(clBlack),KolorWyp(clWhite) { Srodek=S; }
  Symbol(int X, int Y):Odswierz(true),P(0), KanwaSym(0),KolorLinii(clBlack),KolorWyp(clWhite){Srodek.x=X; Srodek.y=Y;}
  void UstKolorLinii(TColor K){KolorLinii=K; Odswierz=true;}
  TColor WezKolorLinii() {return KolorLinii;}
  TColor WezKolorWypeln() {return KolorWyp;}
  void UstKolorWypeln(TColor K) {KolorWyp=K; Odswierz=true;}
  void UstSrodek(TPoint S) {Srodek=S;}
  void UstSrodek(int X, int Y){Srodek.x=X; Srodek.y=Y;}
  void UstRozmiar(int R) {      if(R!=rozmiar)
                                        Odswierz = true;
                                rozmiar = R;            }
  int DajRozmiar() { return rozmiar; }
  void Inicjuj() { if (!P)
                        { throw ErrorRysowania();
                        }
                   StaryKolorLinii = P->Pen->Color;  P->Pen->Color=KolorLinii;
                   StaryKolorWyp=P->Brush->Color;    P->Brush->Color=KolorWyp;
                   }
  void Inicjuj(TCanvas *Plotno) {P=Plotno; Inicjuj(); }
  void Zwolnij() {
                P->Pen->Color=StaryKolorLinii;
                P->Brush->Color=StaryKolorWyp;
                }

  void Wypisz(std::ofstream& wyp_do);
  void Wczytaj(std::ifstream& wczyt_z);
  std::string DajTypSymbolu() { return NazwaSymbolu;}

  virtual void Rysuj(TCanvas*){};
  virtual void Rysuj(TCanvas*, int, int)=0; // Ustaw P³ótno i œrodek
};


Symbol* NowySymbol(std::string nazwa);  // prototyp funkcji do odczytu  typu
                                        // symbolu na podstawie nazwy

class Kolo:public Symbol
{
  //int Promien;
public:
  Kolo(int r)  {UstRozmiar(r); NazwaSymbolu="Kolo";}
  Kolo(){UstRozmiar(3); NazwaSymbolu="Kolo";}
  void UstPromien(int r)
                {UstRozmiar(r);}
  int DajPromien()
                {return rozmiar;}
  void Rysuj(TCanvas *Pl)
                { RysujKolo(Pl);}
  void Rysuj(TCanvas *Pl, int X, int Y)
                {UstSrodek(X,Y);RysujKolo(Pl); }
  void RysujKolo(TCanvas *Pl);
  void RysujKoloN(TCanvas *Pl);
};

class Kwadrat:public Symbol
{
  //int Bok;
public:
  Kwadrat(int Bok)  {UstBok(Bok);NazwaSymbolu="Kwadrat";}
  Kwadrat(){UstBok(4);NazwaSymbolu="Kwadrat";}
  void UstBok(int r)
                {UstRozmiar((r%2)?r+1:r);}  // Bok musi byæ parzysty
  int DajBok()
                {return rozmiar;}
  void Rysuj(TCanvas *Pl)
                { RysujKwadrat(Pl);}
  void Rysuj(TCanvas *Pl, int X, int Y)
                {UstSrodek(X,Y);RysujKwadrat(Pl); }
  void RysujKwadrat(TCanvas *Pl);
  void RysujKwadratN(TCanvas *Pl);
};



/*struct TPara
{
  double X,Y;
  TPara(double a, double b): X(a), Y(b){}

};
*/

template<class T> struct TParaOgl
{
        T X,Y;
        TParaOgl(T a, T b)
        {
           X = a;    Y = b;
        }
        TParaOgl(){ };
};

typedef TParaOgl<double>  TPara;


struct ListSklad {
               std::list< int > punkty;
               int Przejscie1, Przejscie2;
               TColor Kolor;
               int numer;
               ListSklad():Kolor(clRed),Przejscie1(-1),Przejscie2(-1),punkty(),
                                numer(0){};
               };

class TJWykresyPunktow: public TJWspolnyWykres
{
   int Czulosc;  // Okreœla "czu³oœæ" zaznaczania w pikselach
   double** Dane;      //Tablica wszystkich zaznaczonych(Form1) danych
                       //   Dane[0][i] = Temperatura lub K¹ty
                       //   Dane[1][i] = Zaznaczone Pola lub Szerokosci;
                       //   Dane[2][i] = NumerLinii.
                       //   Dane[3][i] = 0 je¿eli punkt nie by³ jeszcze wybrany (wykres punktów)
                       //                lub liczba zaznaczeñ (np. dla dwóch serii).
   double** StareDane; // wykorzystane przy Update
   int rozmiar, Stary_rozmiar;        // liczba punktów
   Symbol* AktSymbol;
   Symbol* ZaznSymbol;
   void inicjuj();
   bool otwarty; //=true gdy wkres byl juz otwarty (w celu weliminowania bledu pojawiajacego sie
                // przy zamykaniu programu bez podgladu wykresu punktow zaznaczonych.
 public:
  int NumerPrzejscia;
  std::list< int > lista;
  std::list<ListSklad> Przejscia;
  std::list<ListSklad>::iterator AktualnaLista;
  TJWykresyPunktow(TForm* P):TJWspolnyWykres(P),Przejscia(){inicjuj();}
  TJWykresyPunktow(TPaintBox *P):TJWspolnyWykres(P),Przejscia(){inicjuj();}
  TJWykresyPunktow(Graphics::TBitmap *P):TJWspolnyWykres(P),Przejscia(){inicjuj();}
  TJWykresyPunktow():Przejscia(){inicjuj();}
  int DajCzulosc() {return Czulosc;}
  void UstCzulosc(int Z){Czulosc=Z;}
  TPoint DajWsp(double WartX, double WartY);
  friend TParaOgl<int> DajWsp(double WartX, double WartY, TJWykresyPunktow& P);
  void rysuj() ;
  int SzukajNajblIndeksu(int X, int Y);
  int SzukajNrPliku(int X, int Y) ;
  TPara DajDanePunktu(int i) { return TPara(Dane[0][i],Dane[1][i]); }
  double DajPole(int i) {return Dane[1][i];}
  double DajDaneX(int i) {return Dane[0][i];}
  int DajNrPliku(int i) {return static_cast<int>(Dane[2][i]);}
  bool Zaznaczony(int i) { return Dane[3][i];}    // zwraca czy dany punkt by³ ju¿ zaznaczony
  void Zaznacz(int i, int a=1) { if(a==0)
                                        Dane[3][i]=0;
                                 else
                                      ++Dane[3][i]/*=a*/;
                                return; }
  void Odznacz(int i){Dane[3][i]=Dane[3][i]?--Dane[3][i]:0;}
  double DajWartX(int X) ;
  double DajWartY(int Y) ;
  bool CzyWykresBylJuzOtwierany() { return otwarty;};
  void Update(int r);
  void UstawSymbolPunktow(Symbol *P){AktSymbol=P; }
  Symbol* DajSymbolPunktow() {return AktSymbol;}
  void UstawSymbolZaznaczania(Symbol* P) {ZaznSymbol = P ; }
  Symbol* DajSymbolZaznaczania() { return ZaznSymbol; }
  void ZapisParametrowWykresu(std::ofstream& zapisz_do);
  int OdczytParametrowWykresu(std::ifstream& wczytaj_z);
};

//---------------------------------------------------------------------------
#endif
