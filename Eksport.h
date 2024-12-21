//---------------------------------------------------------------------------

#ifndef EksportH
#define EksportH
#include "metamorfozis_w5.h"
//---------------------------------------------------------------------------
extern Term termopara;
class TJKonwersje
{
 int a;
public:
  float PrzepDouble(float d) {return d;}  // Funkcja Potrzbna do obslugi wsk do metod
  float TempCelsjusz(float d){ return termopara.TempCelsjusz(d);}
  float TempKelvin(float L){ return TempCelsjusz(L)+273.15;}
};
#endif


