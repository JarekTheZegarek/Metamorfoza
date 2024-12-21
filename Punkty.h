//---------------------------------------------------------------------------

#ifndef PunktyH
#define PunktyH
#include <list>
#include <vector>
#include <stack>





//---------------------------------------------------------------------------
namespace ObszarPunkty
{

// -----------------------     KLASA ErrorPunkt -----------------------------
struct ErrorPunkt {     // Klasa b³êdu
char* komunikat;
ErrorPunkt(char* k) { komunikat = k; }
};

// ---------------------  Koniec ErrorPunkt ----------------------------------


/*  Narazie wylaczamy
// -------------------- Wspolna klasa dla Punkt i ListaPunktow --------------

class WspolnyPunkt
{
public:
        WspolnyPunkt(void) { }
};


// ---------------   Koniec Wspolnej Klasy ----------------------------------
*/


class Przejscie{
    int Poziom_1, Poziom_2;             // Numery poziomow energetycznych
    Symbol* Ksztalt;                     // Ksztalt symbolu
    int Numer;                          // Unikalny numer Przejœcia
public:
    Przejscie();
    Przejscie(int P1, int P2);
    ~Przejscie();
    Przejscie(int Nr);
    TParaOgl<int> DajPoziomy() { return TParaOgl<int>(Poziom_1,Poziom_2); }
    int DajPoziom(int i) { if (i==1)
                                return Poziom_1;
                           else if (i==2)
                                return Poziom_2;
                           else
                                return 0;    // Podano zly numer poziomu
                           }
    int DajNumer() { return Numer; }
    void UstawNumer(int Nr) { Numer = Nr;}
    void UstawKolor(TColor K) { Ksztalt->UstKolorLinii(K); }
    TColor DajKolor() { return Ksztalt->WezKolorLinii(); }
    void UstawKolorWyp(TColor K) {Ksztalt->UstKolorWypeln(K); }
    TColor DajKolorWyp() { return Ksztalt->WezKolorWypeln(); }
    void UstawRozmiar(int r) {Ksztalt->UstRozmiar(r);}
    void UstawKsztalt(Symbol* S) ;
    Symbol* DajKsztalt() {return Ksztalt; }
    bool CzyJestTakiePrzejscie(int P1, int P2);   // true - je¿eli poziomy P1 i P2 s¹ zapisane
    bool CzyJestTakiePrzejscie(Przejscie* P);     // true - je¿eli takie przejscie istnieje;
    void Wypisz(std::ofstream& wyp_z) const;
    void Wczytaj(std::ifstream& wcz_do);
    std::string Opis();
};

class TListaPrzejsc {
    int OstatniNumer;           // Numer ostatniego dodanego przejscia.
    list<Przejscie*> Przejscia;
    void Ustaw();
public:
    vector<int> NumeryPrzejsc;
    typedef  std::list<Przejscie*>::iterator TIter;
    typedef  std::list<Przejscie*>::const_iterator  TSt_iter;
    TListaPrzejsc(){};
    bool DodajPrzejscie(int P1, int P2);
    bool DodajPrzejscie(Przejscie* P);
    bool CzyJestTakiNumer(int Nr);
    bool CyzJestTakiePrzejscie(Przejscie* P);
    int DajNumerPrzejscia(int Poziom1, int Poziom2); // zero oznacza brak przejscia o takich poziomach
    int DajLiczbeZdefiniowanychPrzejsc(){ return Przejscia.size(); }
    Przejscie* DajPrzejscie(int Nr)const;
    Przejscie* DajNastepny();
    int DajNumerKolejny(int nr) { return  NumeryPrzejsc[nr] ; }  // zwraca Numer przejscia z tablicy
                                                                 // (indeksy tablicy s¹ po kolei co jest wa¿ne)
    int DajLiczbeZefiniowanychPrzejsci() { return Przejscia.size();}
  //  Punkt* ZnajdzPunkt(double  Wart, double B);
    void Wypisz(std::ofstream& wyp_z);
    int Wczytaj(std::ifstream& wcz_do);

};




class Punkt    //Klasa zawiera atrybuty zaznaczonego punktu (tzn. zebrane pola rezonansowe).
{
public:


private:
    int NrPliku;                // Numer Pliku
    double Wartosc;             // Ewentualna wartosc wczytana z pliku
    double Pole;                // Wartosc pola magnetycznego zaznaczonego punktu
    int Indeks;                 // Indeks Tablicy ze ZbioruTablic przechowujacego Pole;
    static Symbol* Ksztalt;     // Ksztalt punktow oznaczajacych zebrane pola rezonansowe.
    int OdczytNumer;            // uzywany w DajNastPrzejscie jako licznik;
    bool CzyOdczyt;             // *** UWAGA *** je¿eli true to dodanie przejscia niemozliwe
public:
    std::list<int> Przejscia;   // Lista numerow przejsc

    typedef std::list<int>::iterator TIntIter;
    typedef std::list<int>::const_iterator TStIntIter;

    Punkt()
    {
        NrPliku=-1; Indeks = -1 ; OdczytNumer=0; CzyOdczyt=false;
    }

    Punkt(int NrP, int Ind, float Wart, double P):
        NrPliku(NrP),Wartosc(Wart),Pole(P),Indeks(Ind), OdczytNumer(0), CzyOdczyt(false)
    {};

    static void UstawSymbol(Symbol* K);
    static Symbol* DajSymbol();
        /* TODO : Poprawiæ procedurê wypisywania kolejnych numerów przejœæ. */

    int DajNastPrzejscie();     //zwraca numer kolejnego przejscia; 0 oznacza wyczerpanie listy.
                                // chyba na razie Ÿle dziala
    void SkasujPrzejscie(int P);
    void DodajPrzejscie(int NrPrzejscia);
    bool CzyZawieraPrzejscie(int NrPrzejscia);
    bool CzyZawieraPrzejscie(Przejscie* Prz);

    TPara DajWspolrzedne() const;
    int DajIndeks() { if(NrPliku<0)
                          throw ErrorPunkt("B³¹d przy odczycie Indeksu, prawdopodobnie jeden z zebranych punktow  jest zle okreslonyn");
                      else
                          return Indeks; }
    int DajNumerPliku() { if(NrPliku<0)
                            throw ErrorPunkt("B³¹d przy odczycie NumeruPliku, prawdopodobnie jeden z zebranych punktow  jest zle okreslonyn");
                          else
                            return NrPliku; }
    double DajWartosc() { if(NrPliku<0)
                            throw ErrorPunkt("B³¹d przy odczycie Wartoœci odczytanej z nazwy zbioru, prawdopodobnie jeden z zebranych punktow  jest zle okreslonyn");
                          else
                              return Wartosc; }
    void UstawWartosc(double Wart) { Wartosc=Wart; }
    double DajPole() { if(NrPliku<0)
                            throw ErrorPunkt("B³¹d przy odczycie wartoœci pola rezonansowego, prawdopodobnie jeden z zebranych punktow  jest zle okreslonyn");
                          else
                            return Pole; }
    friend bool operator!=(const Punkt&, const Punkt&);
    friend bool operator==(const Punkt& P1, const Punkt& P2);
    void Wypisz(std::ofstream& wyp_z) const;
    void Wczytaj(std::ifstream& wcz_do);
};

//Czy ta klasa jest w ogóle potrzeba - struct, aby skorzystaæ z dobrodziejwst list
class TListaPunktow
{
public:
           bool ZaznaczanieWielokrotne; //   Czy ten sam punkt moze miec wiele przejsc
private:
    std::list<Punkt> Punkty;

public:
    typedef  std::list<Punkt>::iterator TIter;
    typedef  std::list<Punkt>::const_iterator  TSt_iter;
// --------- Metody -------------------------
    TListaPunktow() {ZaznaczanieWielokrotne= false;}
    int  DajLiczbePunktow() { return Punkty.size(); }
    bool CzyJestTakiNumer(int Nr);
    bool DodajPunkt(Punkt P);
    bool UsunPunkt(Punkt P);
    void DodajPrzejscie(int NrPrzej,Punkt* Pkt);
    Punkt* ZnajdzPunkt(double  Wart, double B);    // Znajduje punkt le¿¹cy najbli¿ej podanych
                                                // wartoœci Wart i B, zero oznacza nieznalezienie;
    Punkt* DajNastepny(int NrPliku);  // Koniec  wczytywania 0, wstawienie w miêdzyczasie
                                      // nowego punktu powoduje "podawanie" punktow od poczatku;
    Punkt* ZnajdzPunkt(double  Wart, double B, int NrZb);  // Znajduje punkt le¿¹cy najbli¿ej podanych
                                                // wartoœci Wart i B; Dla danego numeru Linii;

    typedef TParaOgl<int> (*Wsp)(double, double);
    Punkt* ZnajdzPunkt(double Wart, double B, Wsp );      //Wsp wskaŸnik na funkcje zdefiniowany wy¿ej
    Punkt* DajNastepny();
    void Wypisz(std::ofstream& wyp_z);
    int Wczytaj(std::ifstream& wcz_do);
};


enum TypSkasowany {punkt, lista} ;

class TStosKasowania
{
        std::stack<Punkt> PunktSkasowany;
        std::stack<TListaPunktow*> ListaSkasowana;
        std::stack<TypSkasowany> StosTypow;
public:
        bool umiesc(Punkt Pkt);  // true je¿eli siê udalo
        bool umiesc(TListaPunktow* WskLista);
        bool wez(TListaPunktow* DocelowaLista);
};





}  // -------------------- Koniec przestrzeni nazw.

extern ObszarPunkty::TStosKasowania StosKasowania;
#endif
