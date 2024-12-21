//---------------------------------------------------------------------------

#ifndef termoparaH
#define termoparaH
//#include <math.h>
class ZakresNap
{
};


class Term
{
  int ilosc;
  float** Dane;
  int iter;
public:
  bool wczytaj_dane();
  Term(): ilosc(0), Dane(0),iter(0){};
  float DajTemp(i)
          { if(Dane) return Dane[i][0];
             else return -500;}
  float DajNapiecie(i)
          {if (Dane) return Dane[i][1];
           else return -500;}
  int zacznij_iteracje() { return iter=0;}
  int iteruj() {return (++iter<ilosc)?iter:0;}
  float TempCelsjusz(float);
  float TempKelvin(float L){ return TempCelsjusz(L)+273.15;}
  float  CelsjuszNap(float L);
  //float PrzepDouble(float d) {return d;}  // Funkcja Potrzbna do obslugi wsk do metod
};

extern Term termopara;
//---------------------------------------------------------------------------
#endif
