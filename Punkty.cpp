//---------------------------------------------------------------------------


#pragma hdrstop


#include "metamorfozis_w5.h"
#include "Punkty.h"
#include <algorithm>
#include <math>
#include "Test.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
using namespace ObszarPunkty;

// -----------------  Zmienne Globalne -----------------------

TListaPrzejsc ListaPrzejsc;       // Lista zdefiniowanych przejsc dla Weila
TListaPunktow ZebranePunkty;      // Lista zebranych pol rezonansowych
Symbol* Punkt::Ksztalt = 0;       // "statyczny" kszta³t zebranych punków.
TStosKasowania StosKasowania;     // "Siara" i wszystko jasne


// ---------------------  TLista Przejsc -----------------------------

bool TListaPrzejsc::CzyJestTakiNumer(int Nr)
{
      std::list<Przejscie*>::const_iterator it;
      it=Przejscia.begin();
      for(it;it!=Przejscia.end();it++)
      {
        if((*it)->DajNumer()==Nr)
          return true;
      }
      return false;
}

bool TListaPrzejsc::DodajPrzejscie(int P1, int P2)
{
  int tym = OstatniNumer+1;
  while(CzyJestTakiNumer(tym))
  {
        tym++;
        if (tym==MAXINT)
          throw ErrorPunkt("Zbyt du¿o przejsc i przerkoczony zosta³ zakres int");
  }
  std::list<Przejscie*>::const_iterator it=Przejscia.begin();
  for(it=Przejscia.begin();it!=Przejscia.end();it++)
  {
        if((*it)->CzyJestTakiePrzejscie(P1,P2))
          return false;
  }
  /* DONE 1 : dokoñczyæ wstawianie Przejœcia  gdy doddawane s¹ jednie poziomy. */

  OstatniNumer=tym;
  Ustaw();
  return true;
}


bool TListaPrzejsc::DodajPrzejscie(Przejscie* P)
{
  int tym = OstatniNumer+1;
  while(CzyJestTakiNumer(tym))
  {
        tym++;
        if (tym==MAXINT)
          throw ErrorPunkt("Zbyt du¿o przejsc i przerkoczony zosta³ zakres int");
  }
  std::list<Przejscie*>::const_iterator it=Przejscia.begin();
  for(it=Przejscia.begin();it!=Przejscia.end();it++)
  {
        if((*it)->CzyJestTakiePrzejscie(P))
         // throw ErrorPunkt("Zdefiniowano ju¿ przejœcie o podanych numerach poziomow");
          return false;
  }
  /* DONE 1 : dokoñczyæ wstawianie Przejœcia  gdy doddawane s¹ jednie poziomy. */
  P->UstawNumer(tym);
  OstatniNumer=tym;
  Przejscia.push_back(P);
  Ustaw();
  return true;
}

void TListaPrzejsc::Ustaw()
{
        NumeryPrzejsc.clear();
        int i=0;
        TSt_iter it;
        NumeryPrzejsc.resize(Przejscia.size()+5);
        for(it=Przejscia.begin();it!=Przejscia.end();it++)
              NumeryPrzejsc[i++]= (*it)->DajNumer();

}

Przejscie* TListaPrzejsc::DajPrzejscie(int Nr) const
{
        TSt_iter it;
        for(it=Przejscia.begin();it!=Przejscia.end();it++)
        {
          if( (*it)->DajNumer() == Nr )
             return *it;
        }
        return 0;   // je¿eli nie znaleziono przejscia o podanym numerze.
}

bool TListaPrzejsc::CyzJestTakiePrzejscie(Przejscie* P)
{
      std::list<Przejscie*>::const_iterator it;
      it=Przejscia.begin();
      for(it;it!=Przejscia.end();it++)
      {
        if((*it)->CzyJestTakiePrzejscie(P))
          return true;
      }
      return false;

}

//zwraca 0 gdy przejscia nie ma
int TListaPrzejsc::DajNumerPrzejscia(int Poziom1, int Poziom2)
{
    TSt_iter it=Przejscia.begin();
    while(it!=Przejscia.end())
    {
      if((*it)->CzyJestTakiePrzejscie(Poziom1,Poziom2))
        return (*it)->DajNumer();
      it++;
    }
    return 0;         // Nie znaleziono takiego przejscia
}

void TListaPrzejsc::Wypisz(std::ofstream& zapisz)
{
  try {
    zapisz<<    "[Definicje_Przejsc]"      << endl;
    zapisz<<    Przejscia.size()           << endl;
    TSt_iter it;
    for(it=Przejscia.begin();it!=Przejscia.end();it++)
    {
       (*it)->Wypisz(zapisz);
       zapisz << endl;
    }
  }
  catch(std::ios_base::failure& blad)
  {
        AnsiString T = "Wyst¹pi³ jakiœ b³¹d w trakcie zapisywania przejœæ \n"
                       " miêdzy poziomami.";
        T+="\n Komunikat biblioteczny w jêzyku angielskim: \n";
        T+=blad.what();
        ShowMessage(T);
  }
  catch( ... )
  {
        AnsiString T = "Wyst¹pi³ jakiœ b³¹d w trakcie zapisywania przejœæ \n"
                       " miêdzy poziomami.";
        ShowMessage(T);
  }

}



Przejscie* TListaPrzejsc::DajNastepny()
{
    if(Przejscia.empty()) return 0;
    static TIter it=Przejscia.begin();
 /*   static unsigned int licznik = Przejscia.size();
    if(licznik!=Punkty.size() ) //dodano w miêdzyczasie nowy przejscie
    {
        licznik = Punkty.size();
        it=Punkty.begin();
    }
 */
    Przejscie* P;
    while(it!=Przejscia.end())
    {

                    P= *it;
                    it++;
                    return P;

    }
    it=Przejscia.begin();  // "zawijanie" listy wydawanych Przejsc
    return 0;           // osi¹gniêto koniec bez NrPliku
}


int TListaPrzejsc::Wczytaj(std::ifstream& wczyt)
{
  try {
    int LiczbaPrz;
    char fake;
    wczyt>>fake;
    if(fake!='[')
    {
        wczyt.putback(fake);
        return 0;
    }
    wczyt.putback(fake);
    std::string zakladka;
    wczyt>>zakladka;
    if(zakladka.compare("[Definicje_Przejsc]")) // compare daje 0 gdy napisy sa rowne
    {
      for(unsigned int i=0;i<zakladka.size();i++)
      {
        fake=zakladka[i];
        wczyt.putback(fake);
      }
        return 0;
    }
    wczyt>>LiczbaPrz;
    Przejscie* Prz;
      /* TODO : Usun¹æ testowy kod poni¿ej */
    AnsiString T = "Odczyt przejscia nr = " ;
    for(int i=0;i<LiczbaPrz;i++)
    {
       FormTest->Memo1->Lines->Add(T+i);
       Prz= new Przejscie();
       Prz->Wczytaj(wczyt);
       DodajPrzejscie(Prz);
    }
    return LiczbaPrz;
  }
  catch(std::ios_base::failure& blad)
  {
        AnsiString T = "Wyst¹pi³ jakiœ b³¹d w trakcie wczytywania danych \n"
                       " przejœæ miêdzy poziomami. \n"
                       "Prawdopodnie plik z zapisem sesji jest uszkodzony";
        T+="\n Komunikat biblioteczny w jêzyku angielskim: \n";
        T+=blad.what();
        ShowMessage(T);
        throw;
  }
  catch( ... )
  {
        AnsiString T = "Wyst¹pi³ jakiœ b³¹d w trakcie wczytywania  \n"
                       " przejœæ miêdzy poziomami. \n"
                       "Prawdopodnie plik z zapisem sesji jest uszkodzony";
        ShowMessage(T);
        throw;
  }

}

// =========================== Koniec TListaPrzejsc ===========================


// ------------------------- Przejscie  ---------------------------------------


Przejscie::Przejscie()
{
   Poziom_1=-1;
   Poziom_2=-1;             // Numery poziomow energetycznych wartosci ujemne to wartosci bledne
   Ksztalt = new Kwadrat(8);                     // Ksztalt symbolu
   Ksztalt->UstKolorLinii(clRed);
   Ksztalt->UstKolorWypeln(clWhite);
   Numer=0 ;
}


Przejscie::Przejscie(int P1, int P2)
{
   Poziom_1= P1;
   Poziom_2= P2;             // Numery poziomow energetycznych wartosci ujemne to wartosci bledne
   Ksztalt = new Kwadrat(8);                     // Ksztalt symbolu
   Ksztalt->UstKolorLinii(clRed);
   Ksztalt->UstKolorWypeln(clWhite);
   Numer=0 ;
}


Przejscie::~Przejscie()
{
    delete Ksztalt;
}

bool Przejscie::CzyJestTakiePrzejscie(int P1, int P2)
{
   if( (P1==Poziom_1 && P2==Poziom_2) ||
                        (P2==Poziom_1 && P1==Poziom_2) )
        return true;
   else
        return false;
}


bool Przejscie::CzyJestTakiePrzejscie(Przejscie* P)
{
        int P1 = P->Poziom_1;
        int P2 = P->Poziom_2;
        return CzyJestTakiePrzejscie(P1, P2);
}


void Przejscie::UstawKsztalt(Symbol* S)
{
        Ksztalt = S;
}

std::string Przejscie::Opis()
{
        std::string nap;
        nap= Poziom_1;
        nap+= " -> ";
        nap+= Poziom_2;
        return nap;
}

void Przejscie::Wypisz(std::ofstream& wyp_do) const
{
    wyp_do << Numer <<"\t"<< Poziom_1 << "\t" << Poziom_2 <<"\n" ;
    wyp_do << Ksztalt->DajTypSymbolu() << '\t';
    Ksztalt->Wypisz(wyp_do);
}

void Przejscie::Wczytaj(std::ifstream& wcz_z)
{

   wcz_z>> Numer >> Poziom_1 >> Poziom_2;
   /* TODO : Usun¹æ testowy kod poni¿ej */
   AnsiString T = "Odczytano : " ;
   T+="Numer "; T+= Numer; T+=" P1 ";T+=Poziom_1 ;
   T+=" P2 "; T+=Poziom_2;
   FormTest->Memo1->Lines->Add(T);

   std::string NazwaTypuSymbolu;
   wcz_z>>NazwaTypuSymbolu;

   /* TODO : Usun¹æ testowy kod poni¿ej */
   T=" Nazwa Symbolu: "; T+=NazwaTypuSymbolu.c_str();
   FormTest->Memo1->Lines->Add(T);
   Ksztalt = NowySymbol(NazwaTypuSymbolu);

   Ksztalt->Wczytaj(wcz_z);
}

// ============================ Przejscie Koniec ==============================

// ---------------------------- Punkt    --------------------------------------


void  Punkt::DodajPrzejscie(int NrPrzejscia)
{
     if(CzyOdczyt)
                return;

     if(ListaPrzejsc.CzyJestTakiNumer(NrPrzejscia))     //Sprawdzenie czy dane przejscie istnieje
     {
           if(Przejscia.empty())
           {
              Przejscia.push_back(NrPrzejscia);       // Dodaj pierwszy numer.
              return;
           }


           if(find(Przejscia.begin(),Przejscia.end(),NrPrzejscia)==Przejscia.end()) // nie dodano
                Przejscia.push_back(NrPrzejscia);
            //  Przejscia.insert(Przejscia.end(),NrPrzejscia);
     }
     else
        throw  ErrorPunkt("Nie zdefiniowano przejscia o ¿¹danym numerze");

}
bool Punkt::CzyZawieraPrzejscie(int NrPrzej)
{
   TStIntIter iter;
   for(iter = Przejscia.begin();iter!=Przejscia.end();iter++)
   {
        if( *iter == NrPrzej ) return true;
   }
   return false;
}

bool Punkt::CzyZawieraPrzejscie(Przejscie* Prz)
{
        return CzyZawieraPrzejscie(Prz->DajNumer());
}

void Punkt::UstawSymbol(Symbol* K)
{
        Ksztalt = K;
}

Symbol* Punkt::DajSymbol()
{
  return Ksztalt;
}

int Punkt::DajNastPrzejscie()
{
 if(Przejscia.empty()) return 0;
 static std::list<int>::iterator it=Przejscia.begin();
 if (it==Przejscia.begin())
                CzyOdczyt= true;                 // zapobieganie zmianie listy
 if (it==Przejscia.end())                       //zawiniêcie listy
 {
        it=Przejscia.begin();
        CzyOdczyt = false;                     // CzyOdczyt koniec odczytywania
        return 0;                              // 0 oznacza koniec listy.
 }
 int wyn = *it;
 it++;
 return wyn;

}

TPara Punkt::DajWspolrzedne() const
{
    return TPara(Wartosc, Pole);
}

bool ObszarPunkty::operator!=(const Punkt& P1, const Punkt& P2)
{

        if( (P1.Wartosc == P2.Wartosc) && (P1.NrPliku == P2.NrPliku) &&
            (P1.Pole == P2.Pole) )
                return false;
        else
                return true;
}

bool ObszarPunkty::operator==(const Punkt& P1, const Punkt& P2)
{
     if( (P1.Wartosc == P2.Wartosc) && (P1.NrPliku == P2.NrPliku) &&
            (P1.Pole == P2.Pole) )
                return true;
        else
                return false;
}

void Punkt::Wypisz(std::ofstream& wyp_do) const
{
    wyp_do << NrPliku <<"\t"<< Wartosc << "\t" << Pole << "\t" << Indeks <<"\n" ;
    TStIntIter it;
    wyp_do<< Przejscia.size();   // zapisz liczbe przejsc
    for(it=Przejscia.begin();it!=Przejscia.end();it++)
    {
        wyp_do << '\t' << *it;
    }
}

void Punkt::Wczytaj(std::ifstream& wczyt_z)
{
    wczyt_z >> NrPliku;
    wczyt_z >> Wartosc;
    wczyt_z >> Pole;
    wczyt_z >> Indeks ;
    int i, przej;
    wczyt_z>>i;   // zapisz liczbe przejsc
    for(i;i>0;i--)
    {
       wczyt_z>>przej;
       Przejscia.push_back(przej);
    }
}


// ============================= Koniec Punkt =================================

// ----------------------------- TListaPunktow --------------------------------

void TListaPunktow::DodajPrzejscie(int NrPrzej,Punkt* Pkt)
{
    if(!ZaznaczanieWielokrotne  &&  Pkt->Przejscia.size()>0 ) // jezeli Dodano juz przejscie
                                                              // a mozna jedno
                return;
        Pkt->DodajPrzejscie(NrPrzej);
}


bool TListaPunktow::DodajPunkt(Punkt P) //Dodaje sortuj¹c rosn¹co
{
        //std::vector<Punkt>::iterator it;
        TIter it;
        try {
            if(Punkty.empty())      //wstaw pierwszy punkt
            {
              Punkty.push_back(P);
           //   AnsiString S = P
              return true;
            }
            else
            {
               for(it=Punkty.begin();it!=Punkty.end();it++)
               {
                  if( (*it).DajWspolrzedne().Y>=P.DajWspolrzedne().Y)  //sortuj rosn¹co
                  {
                      if( (*it) == P )
                          return false;    // ten punkt byl juz dodany;
                      else
                      {
                          Punkty.insert(it,P);
                          return true;
                      }
                  }
               }
               Punkty.push_back(P);  // P jest wiêksze od innych wiêc wstaw na koniec
               return true;
            }
        }
        catch(...)                  //cos uniemozliwolo dodanie punku
        {
          return false;
        }
}


bool TListaPunktow::UsunPunkt(Punkt P)
{

       // std::vector<Punkt>::iterator it;
       TIter it;
        it = find(Punkty.begin(),Punkty.end(),P);
        if(it == Punkty.end())
                return false;
        Punkty.erase(it);
        return true;
}

Punkt* TListaPunktow::ZnajdzPunkt(double  Wart, double B)
{
   //std::vector<Punkt>::const_iterator it; //,first,last;
   TIter it;
   if(Punkty.empty()) return 0; // je¿eli lista jest pusta zwroc zero;
   Punkt* Min= &(*Punkty.begin());
   double Odl,MinOdl;
   it = Punkty.begin();
   Odl=MinOdl=sqrt(  ((B-(*it).DajWspolrzedne().Y)*(B-(*it).DajWspolrzedne().Y))+
                     ((Wart-(*it).DajWspolrzedne().X)*(Wart-(*it).DajWspolrzedne().X)) );

   it++;
   for(it;it!=Punkty.end();it++)
   {
     Punkt P=*it;
     Odl=sqrt(   ((B-(*it).DajWspolrzedne().Y)*(B-(*it).DajWspolrzedne().Y))+
                 ((Wart-(*it).DajWspolrzedne().X)*(Wart-(*it).DajWspolrzedne().X))  );
     if(Odl<=MinOdl)
     {
        MinOdl= Odl;
        Min = &(*it);
     }
   }
   return Min;
}

Punkt* TListaPunktow::ZnajdzPunkt(double  Wart, double B, int NrZbioru)
{
   TSt_iter it;
   Punkt* P, *Min=0;              // Min = 0 gdy nie ma to nie ma Min
   double Odl,MinOdl=MAXDOUBLE;
   while((P=DajNastepny(NrZbioru))!=0 )
   {
      Odl=sqrt(  ((B- P->DajWspolrzedne().Y)*(B-P->DajWspolrzedne().Y))+
                   ((Wart - P->DajWspolrzedne().X)*(Wart - P->DajWspolrzedne().X)) );
       if(Odl<=MinOdl)
     {
        MinOdl= Odl;
        Min = P;
     }
   }
   return Min;
}


Punkt* TListaPunktow::ZnajdzPunkt(double Wart, double B, TParaOgl<int> (*WspEkr)(double, double) )
{
   TIter it;
   if(Punkty.empty()) return 0; // je¿eli lista jest pusta zwroc zero;
   Punkt* Min= &(*Punkty.begin());
   double Odl,MinOdl;
   it = Punkty.begin();
   TParaOgl<int> P_e, Wej_e;
     Wej_e = WspEkr(Wart, B);
     P_e   = WspEkr((*it).DajWspolrzedne().X,(*it).DajWspolrzedne().Y) ;
   Odl=MinOdl=sqrt(     (Wej_e.X-P_e.X)*(Wej_e.X-P_e.X) +
                        (Wej_e.Y-P_e.Y)*(Wej_e.Y-P_e.Y) );

   it++;
   for(it;it!=Punkty.end();it++)
   {
     Punkt P=*it;
     Wej_e = WspEkr(Wart, B);
     P_e   = WspEkr((*it).DajWspolrzedne().X,(*it).DajWspolrzedne().Y) ;
     Odl=sqrt(          (Wej_e.X-P_e.X)*(Wej_e.X-P_e.X) +
                        (Wej_e.Y-P_e.Y)*(Wej_e.Y-P_e.Y) );
     if(Odl<=MinOdl)
     {
        MinOdl= Odl;
        Min = &(*it);
     }
   }
   return Min;
}


Punkt* TListaPunktow::DajNastepny(int NrPliku)
{
    if(Punkty.empty()) return 0;
    static TIter it=Punkty.begin();
    static unsigned int licznik = Punkty.size();
    if(licznik!=Punkty.size() ) //dodano w miêdzyczasie nowy punkt
    {
        licznik = Punkty.size();
        it=Punkty.begin();
    }
    Punkt* P;
    while(it!=Punkty.end())
    {
        if( (*it).DajNumerPliku() == NrPliku)
        {
                    P=&(*it);
                    it++;
                    return P;
        }
        it++;
    }
  /*
    if(it==Punkty.end())
    {
        it=Punkty.begin();  // "zawijanie" listy wydawanych Punktow
        return 0;           // osi¹gniêto koniec bez NrPliku
    }
  */
   it=Punkty.begin();  // "zawijanie" listy wydawanych Punktow
   return 0;           // osi¹gniêto koniec bez NrPliku
}


Punkt* TListaPunktow::DajNastepny()
{
    if(Punkty.empty()) return 0;
    static TIter it=Punkty.begin();
    static unsigned int licznik = Punkty.size();
    if(licznik!=Punkty.size() ) //dodano w miêdzyczasie nowy punkt
    {
        licznik = Punkty.size();
        it=Punkty.begin();
    }
    Punkt* P;
    while(it!=Punkty.end())
    {

                    P=&(*it);
                    it++;
                    return P;

    }
    it=Punkty.begin();  // "zawijanie" listy wydawanych Punktow
    return 0;           // osi¹gniêto koniec bez NrPliku
}

void TListaPunktow::Wypisz(std::ofstream& zapisz)
{
  try {
    zapisz<< Punkty.size() << endl;
    TSt_iter it;
    for(it=Punkty.begin();it!=Punkty.end();it++)
    {
       (*it).Wypisz(zapisz);
       zapisz << endl;
    }
   Symbol* S = Punkt::DajSymbol();
   if(!S)  // Je¿eli nie zdefiniowano sybolu dla symboli.
        S= Form1->Zaznaczenie;


   zapisz<<S->DajTypSymbolu()<<'\t';
   S->Wypisz(zapisz);
   zapisz<<endl;
  }
  catch(std::ios_base::failure& blad)
  {
        AnsiString T = "Wyst¹pi³ jakiœ b³¹d w trakcie zapisywania zaznaczonych \n"
                       " punktów.";
        T+="\n Komunikat biblioteczny w jêzyku angielskim: \n";
        T+=blad.what();
        ShowMessage(T);
  }
  catch( ... )
  {
        AnsiString T = "Wyst¹pi³ jakiœ b³¹d w trakcie zapisywania zaznaczonych \n"
                       " punktów.";
        ShowMessage(T);
  }

}


int TListaPunktow::Wczytaj(std::ifstream& wczyt)
{
  try {
    int LiczbaPkt;
    char fake;
    wczyt>>LiczbaPkt;
    Punkt* Pkt;
    for(int i=0;i<LiczbaPkt;i++)
    {
       Pkt=new Punkt();
       Pkt->Wczytaj(wczyt);
       DodajPunkt(*Pkt);
       delete Pkt;
    }
    std::string NazwaTypuSymbolu;
    wczyt>>NazwaTypuSymbolu;
    Punkt::UstawSymbol( NowySymbol(NazwaTypuSymbolu) );
    Punkt::DajSymbol()->Wczytaj(wczyt);

    return LiczbaPkt;
  }
  catch(std::ios_base::failure& blad)
  {
        AnsiString T = "Wyst¹pi³ jakiœ b³¹d w trakcie wczytywania zaznaczonych \n"
                       " punktów. Prawdopodnie plik z zapisem sesji jest uszkodzony";
        T+="\n Komunikat biblioteczny w jêzyku angielskim: ";
        T+=blad.what();
        ShowMessage(T);
        throw;
  }
  catch( ... )
  {
        AnsiString T = "Wyst¹pi³ jakiœ b³¹d w trakcie wczytywania zaznaczonych \n"
                       " punktów. Prawdopodnie plik z zapisem sesji jest uszkodzony";
        ShowMessage(T);
        throw;
  }

}

// ==========================  Koniec TListaPunktow   =========================

// --------------------------  TStosKasowania ---------------------------------

bool TStosKasowania::umiesc(Punkt Pkt)
{
        StosTypow.push(punkt);
        PunktSkasowany.push(Pkt);
        return true;
}

bool TStosKasowania::umiesc(TListaPunktow* WskLista)
{
        StosTypow.push(lista);
        ListaSkasowana.push(WskLista);
        return true;
}

 bool TStosKasowania::wez(TListaPunktow* DocelowaLista)
 {
      TypSkasowany Typ;
      if(StosTypow.empty())
                        return false;
      Typ = StosTypow.top();
      StosTypow.pop();
      Punkt* Pkt;
      TListaPunktow* Lista;
      int i=0;
      switch(Typ)
      {
        case punkt:
                    if(PunktSkasowany.empty())
                        return false;
                    if(DocelowaLista->DodajPunkt(PunktSkasowany.top()))
                    {
                        PunktSkasowany.pop();
                        return true;
                    }
                    else
                    {
                        PunktSkasowany.pop();      // operacja sie nie udala
                        return false;              // ale trzeba wyczyscic stos
                    }

        case lista:
                    if(ListaSkasowana.empty())
                        return false;
                    Lista = ListaSkasowana.top();
                    while( (Pkt=Lista->DajNastepny())!=0 )
                    {
                        i++;
                        if(DocelowaLista->DodajPunkt(*Pkt))
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    ListaSkasowana.pop();
                    if(i!=Lista->DajLiczbePunktow())
                    {
                        delete Lista;
                        return false;
                    }
                    else
                    {
                        delete Lista;
                        return true;
                    }
      }
    return false;
 }

// ========================= Koniec TStosKasowania ============================

