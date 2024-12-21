#include "metamorfozis_w5.h"
TStringList *ListaPlikow = NULL;
AnsiString    katalog;
float PowiekszX;
TJTryby Tryb;
bool zamrozonypanel = true;
char bufor_wzorca_bitowego[80];
strstream napis2;
long LiczbaWierszy;
int LiczbaWczytPlikow=0;
double **TablicaZbiorcza;
bool rysunek = false;
double WspX;    //Aktualny wspó³czynnik skalowania dla wyresu;
double WspY_B,WspY_C;
int popr; // Poprawka w pikselach do wysokoœci okna
int NumerPliku=1;
int LiczbaZebranychSzerokosci;
int LiczbaZebranychPolozen;

