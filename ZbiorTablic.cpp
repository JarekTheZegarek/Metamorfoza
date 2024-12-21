//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ZbiorTablic.h"
#include "metamorfozis_w5.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
static AnsiString S1, S2;
static strstream ekran;
static  real L;
// class ZbiorTablic;
// class TabAsocInt;
//extern class
// ********************** Implementacja metod ZbiorTablic  ****************

void ZbiorTablic::Dodaj(AnsiString s,int i,long rozm)   // Dodawanie pliku Dat
{
  static ZbiorTablic*  akt= this;
  char drive[_MAX_DRIVE];
  char directory[_MAX_DIR];
  char roz[_MAX_EXT];
  char path[_MAX_PATH];
  _splitpath(s.c_str(),drive,directory,0,roz);
  if(!WczytaneX)
  {
   try{
         // tymczasowa tablica na wczytany plik
         unsigned char* pchar = new  char[rozm];
         X = new double[rozmiar];      // miejsce na pola rezonansowe
         FILE* wejscie = fopen(s.c_str(),"rb");
         // fseek(wejscie,356,SEEK_SET); // 356 - wstêpny obszar pliku
         int wynikfred;
         wynikfred=fread(pchar,1,rozm,wejscie);
         if(!wynikfred)
         {
           Form1->Memo1->Lines->Add(" ** WCZYTYWANIE  PLIKÓW NIEPOWIOD£O SIÊ ** ");
           Form1->Memo1->Lines->Add("pierwszy wiersz " + s);
           return;
         }
         unsigned char* TymPlik = pchar;
         if(strcmp(".dat",roz))
         {
            TymPlik=strstr((char*)TymPlik,"F0 S0");
            TymPlik+=strlen("F0 S0")+2;
         }
         for(int i=0;i<rozmiar;i++) // Wczytywanie wartoœci pól rezonansowych
         {
                int wyn  = sscanf(TymPlik,"%lf",&X[i]);
                while(*TymPlik++!='\n')
                        ; //PrzejdŸ do koñca linijki
         }
         if(X[0]<ZbiorTablic::GlobalMinY) ZbiorTablic::GlobalMinY=X[0];
         if(X[rozmiar-1]>ZbiorTablic::GlobalMaxY) ZbiorTablic::GlobalMaxY=X[rozmiar-1];
         fclose(wejscie);
         Form1->Memo1->Lines->Add("");
         Form1->Memo1->Lines->Add("*******************************************************");
         S1="Odczytujê aktualnie zakres od ";
         S2= X1;
         S1=S1+ S2 + " mT";
         S1= S1+" do ";
         S2=  String(X1 + DajPrzedzial())+" mT";
         S1=S1+S2;
         Form1->Memo1->Lines->Add(S1);
         Form1->Memo1->Lines->Add("*******************************************************");
         Form1->Memo1->Lines->Add("");
         WczytaneX=true;
         delete[] pchar;
    }
     catch(std::bad_alloc)
    {   // ENTER THIS BLOCK ONLY IF std::bad_alloc IS THROWN.
        S1= "Nie mogê przydzieliæ pamiêci. Bye ...\0";
        Application->MessageBox(S1.c_str(), "Wyst¹pi³ b³¹d", MB_OK);
        exit(-1);
    }
  }
  if(akt!=this && WczytaneX)
      {
         Form1->Memo1->Lines->Add("");
         Form1->Memo1->Lines->Add("*******************************************************");
         S1="Odczytujê aktualnie zakres od ";
         S2= X1;
         S1=S1+ S2 + " mT";
         S1= S1+" do ";
         S2=  String(X1 + DajPrzedzial())+" mT";
         S1=S1+S2;
         Form1->Memo1->Lines->Add(S1);
         Form1->Memo1->Lines->Add("*******************************************************");
         Form1->Memo1->Lines->Add("");
      }
          //Pliki->Add zwraca pozycjê w liœcie

          if(!strcmp(".dat",roz))
                  Asoc->DodajDat(Pliki->Add(s),i,s, rozmiar,rozm);
          else
                  Asoc->DodajEPR(Pliki->Add(s),i,s, rozmiar,rozm);

  ilosc++;
  akt = this;
 }

 void ZbiorTablic::Dodaj(AnsiString s,int i)
     {
        static ZbiorTablic*  akt= this;
		if(!WczytaneX)        // Pierwsza linia z nowej serii
        {
         try{
              // tymczasowa tablica na wczytany plik
              unsigned char* pchar = new unsigned char[rozmiar*12];
              X = new double[rozmiar];      // miejsce na pola rezonansowe


          FILE* wejscie = fopen(s.c_str(),"rb");
          fseek(wejscie,356,SEEK_SET); // 356 - wstêpny obszar pliku
          int wynikfred;
          wynikfred=fread(pchar,1,rozmiar*12,wejscie);  // struktura.st_size-356
           if(!wynikfred)
           {
             Form1->Memo1->Lines->Add(" ** WCZYTYWANIE  PLIKÓW NIEPOWIOD£O SIÊ ** ");
             Form1->Memo1->Lines->Add("pierwszy wiersz " + s);
             return;
           }
           for(int i=0;i<rozmiar;i++) // Wczytywanie wartoœci pól rezonansowych
           {
            for(int j=0;j<6;j++) L[j]=*(pchar+i*2*6+j);
            X[i]=RealToDouble(L);
           }
           if(X[0]<ZbiorTablic::GlobalMinY) ZbiorTablic::GlobalMinY=X[0];
           if(X[rozmiar-1]>ZbiorTablic::GlobalMaxY) ZbiorTablic::GlobalMaxY=X[rozmiar-1];
           fclose(wejscie);
           Form1->Memo1->Lines->Add("");
           Form1->Memo1->Lines->Add("*******************************************************");
           S1="Odczytujê aktualnie zakres od ";
           S2= X1;
           S1=S1+ S2 + " mT";
           S1= S1+" do ";
           S2=  String(X1 + DajPrzedzial())+" mT";
           S1=S1+S2;
           Form1->Memo1->Lines->Add(S1);
           Form1->Memo1->Lines->Add("*******************************************************");
           Form1->Memo1->Lines->Add("");
           WczytaneX=true;
           delete[] pchar;
          }
           catch(std::bad_alloc)
          {   // ENTER THIS BLOCK ONLY IF std::bad_alloc IS THROWN.
              S1= "Nie mogê przydzieliæ pamiêci. Bye ...\0";
              Application->MessageBox(S1.c_str(), "Wyst¹pi³ b³¹d", MB_OK);
              exit(-1);
          }
        }
        if(akt!=this && WczytaneX)  // Je¿eli by³a przerwa w ci¹gu lini o tym samym zakresie
            {
               Form1->Memo1->Lines->Add("");
               Form1->Memo1->Lines->Add("*******************************************************");
               S1="Odczytujê aktualnie zakres od ";
               S2= X1;
               S1=S1+ S2 + " mT";
               S1= S1+" do ";
               S2=  String(X1 + DajPrzedzial())+" mT";
               S1=S1+S2;
               Form1->Memo1->Lines->Add(S1);
               Form1->Memo1->Lines->Add("*******************************************************");
               Form1->Memo1->Lines->Add("");
            }

        Asoc->Dodaj(Pliki->Add(s),i,s, rozmiar);
        ilosc++;
        akt = this;
     }

/*
 void ZbiorTablic::Dodaj(AnsiString s,int i,long RozmiarPliku)
     {
        static ZbiorTablic*  akt= this;
        if(!WczytaneX)
        {
         try{
              // tymczasowa tablica na wczytany plik
              unsigned char* pchar = new unsigned char[rozmiar*12];
              X = new double[rozmiar];      // miejsce na pola rezonansowe


          FILE* wejscie = fopen(s.c_str(),"rb");
          fseek(wejscie,356,SEEK_SET); // 356 - wstêpny obszar pliku
          int wynikfred;
          wynikfred=fread(pchar,1,rozmiar*12,wejscie);  // struktura.st_size-356
           if(!wynikfred)
           {
             Form1->Memo1->Lines->Add(" ** WCZYTYWANIE  PLIKÓW NIEPOWIOD£O SIÊ ** ");
             Form1->Memo1->Lines->Add("pierwszy wiersz " + s);
             return;
           }
           for(int i=0;i<rozmiar;i++) // Wczytywanie wartoœci pól rezonansowych
           {
            for(int j=0;j<6;j++) L[j]=*(pchar+i*2*6+j);
            X[i]=RealToDouble(L);
           }
           fclose(wejscie);
           Form1->Memo1->Lines->Add("");
           Form1->Memo1->Lines->Add("*******************************************************");
           S1="Odczytujê aktualnie zakres od ";
           S2= X1;
           S1=S1+ S2 + " mT";
           S1= S1+" do ";
           S2=  String(X1 + DajPrzedzial())+" mT";
           S1=S1+S2;
           Form1->Memo1->Lines->Add(S1);
           Form1->Memo1->Lines->Add("*******************************************************");
           Form1->Memo1->Lines->Add("");
           WczytaneX=true;
           delete[] pchar;
          }
           catch(std::bad_alloc)
          {   // ENTER THIS BLOCK ONLY IF std::bad_alloc IS THROWN.
              S1= "Nie mogê przydzieliæ pamiêci. Bye ...\0";
              Application->MessageBox(S1.c_str(), "Wyst¹pi³ b³¹d", MB_OK);
              exit(-1);
          }
        }
        if(akt!=this && WczytaneX)
            {
               Form1->Memo1->Lines->Add("");
               Form1->Memo1->Lines->Add("*******************************************************");
               S1="Odczytujê aktualnie zakres od ";
               S2= X1;
               S1=S1+ S2 + " mT";
               S1= S1+" do ";
               S2=  String(X1 + DajPrzedzial())+" mT";
               S1=S1+S2;
               Form1->Memo1->Lines->Add(S1);
               Form1->Memo1->Lines->Add("*******************************************************");
               Form1->Memo1->Lines->Add("");
            }

        Asoc->Dodaj(Pliki->Add(s),i,s, rozmiar);
        ilosc++;
        akt = this;
     }

*/
bool ZbiorTablic::Ustaw()
{
    double** tym=Tab;
    try {                                               // TEST FOR EXCEPTIONS.
      Tab = new double*[LiczKolumn()];   // STEP 1: SET UP THE ROWS.
      for (int j = 0; j < LiczKolumn(); j++)
       {
          Tab[j] = new double[rozmiar];  // STEP 2: SET UP THE COLUMNS
       }
    }
    catch(std::bad_alloc)
      {  // ENTER THIS BLOCK ONLY IF std::bad_alloc IS THROWN.
        ekran << "Nie mogê przydzieliæ pamiêci. Bye ...\0";
        Application->MessageBox(ekran.str(), "Wyst¹pi³ b³¹d", MB_OK);
        exit(-1);
      }
    Tab[0]= X;
    for(int i=1;i<LiczKolumn();i++)
    {
     Tab[i]= Asoc->WartosciY(i-1);
    }
    if(tym) delete tym;
    PrzydzielPola();
    return true;
}

void ZbiorTablic::PrzydzielPola()
{
   if (!Pola)
   {
        Pola = new Punkty[ilosc];
        RozmiarPola = ilosc;
   }
   else //Dodajemy nowe Punkty
   {
     Punkty* tym = Pola;
     Pola = new Punkty[ilosc];
     for(int i=0;i<RozmiarPola;i++)  // zapamiêtujem dotychczasowe.
        Pola[i]=tym[i];
     RozmiarPola = ilosc;
   }
}



ZbiorTablic::~ZbiorTablic()
      {
       delete Pliki;
       if(Tab) delete[] Tab;
       Asoc->Zwolnij();
      }







//************************  Implementacje metod klasy TabAsocInt   ***********

void TabAsocInt::Zwolnij()
{
                 TabAsocInt* tym=Nast;
                 TabAsocInt* pop;
                 while(tym)
                 { pop = tym;
                   tym=tym->Nast;
                   delete pop;
                 }
                 delete this;
}

// funkcja odczytuje liczbê zawart¹ w nazwie zbioru
double Ekstrakt(AnsiString Nap)
{
  char* akt;
  char plik[_MAX_FNAME];
  char liczba[20]={'X'};

  _splitpath(Nap.c_str(),0,0,plik,0);
  akt=plik;
  while(*akt)
  { // po napotkaniu pierwszej cyfry
    if (isdigit(*akt) || *akt == '-' || *akt=='+')
        {  int i=0;
           do
            { liczba[i++]=*akt++;
               if(*akt==',') *akt='.';  // zamieñ przecinek dziesiêtny
            }
          while(isdigit(*akt)|| *akt == '.' );
          break;
        }
        akt++;
  }
  double wynik;
  char *koniec;
  wynik =strtod(liczba,&koniec);
  if (liczba == koniec )return -3000;  // Niepowodzenie
  return wynik;
}
/*
double DajWartosc(int Akt) {for(TabAsocInt* tym=this;tym!=NULL;tym=tym->Nast)
                                   if(tym->NumerPliku==Akt) return tym->Wartosc;
                              return -2000;} // w wypadku niepowodzenia.
 */

 float TabAsocInt::DajNastSzerokosc(int Akt)
 {
  return 0;
 }

 bool TabAsocInt::KasujMaxMin(int Akt, int ind)
 {
  int Min=MaxInt;
  TabAsocInt* T=DajAkt(Akt);
  if ( T->dBMax<0 && T->dBMin<0) return false;
  if ( T->dBMax<0 )
  {
         T->dBMin=-1;
         return true;
  }
  else if (T->dBMin<0)
  {
         T->dBMin=-1;
         return true;
  }
  if( abs(T->dBMax -ind) > abs(T->dBMin - ind))
      dBMin=-1;
  else
      dBMax=-1;
  return true;
 }

bool TabAsocInt::UstSzerokosc(int Akt, int i)  //i- wiersz Wartosci
 {
     TabAsocInt* TymA=DajAkt(Akt);
     if(TymA)
         if(Szer->Dodaj(i)) return true;
         return false;   // W razie niepowodzenia
 }

  TPoint TabAsocInt::DajIndeksyPunktSzer(int Akt,int i)
  {
   /* TODO : Do dokoñczenia znajdowanie szerokosci i okoliczne funkcje */
  /* TPoint Ekstrema;
   int X1=i,X2=i;
   float LokMax, LokMin;
   LokMax=-MaxInt; LokMin=MaxInt;
   TabAcocInt* TymA=DajAkt(Akt);
   if(TymA->IndMax  > TymA->IndMin)    //Je¿eli linia typu góra-dó³
             {
               while( TymA->WartoscY(X1)<TymA->WartoscY(X1-1) && X1>0)
                 {
                   X1-=2;
                 }
               if(TymA->WartoscY(X1) >= TymA->WartoscY(X1-4) && X1>0); // To Prawdopodobnie mamy maksimum
   return TPoint(X1,X2); */
    TabAsocInt* T=DajAkt(Akt);
    return TPoint(T->dBMax,T->dBMin);
  }






// Akt - lokalny numer , Plik - unikalny numer pliku
double* TabAsocInt::Dodaj(int Akt, int Plik, AnsiString Nazwa, int rozm)
{
 TabAsocInt* tym=this;
 while(tym->Nast)            // idŸ na koniec listy
       tym=tym->Nast;
 tym->Nast = new TabAsocInt;
 tym->Nast->AktNumer=Akt;
 tym->Nast->NumerPliku=Plik;
 tym->Nast->Wartosc=Ekstrakt(Nazwa);
 tym=tym->Nast;
 FILE* wejscie=fopen(Nazwa.c_str(),"rb");
  struct stat struktura;
    /*
       st_mode	Bit mask giving information about the file's mode
       st_dev	Drive number of disk containing the file or file handle if the file is on a device
       st_rdev	Same as st_dev
       st_nlink	Set to the integer constant 1
       st_size	Size of the file in bytes
       st_atime	Most recent access (Windows) or last time modified (DOS)
       st_mtime	Same as st_atime
       st_ctime	Same as st_atime
    */
   stat(Nazwa.c_str(),&struktura);
   tym->Czas=struktura.st_ctime;
 if (wejscie == NULL)
 {
     S1= "Nie mogê otworzyæ pliku ";
     S2= Nazwa;
     S1=S1+S2;
     S1=S2+ " Bye ...";
     Application->MessageBox(S1.c_str(), "Wyst¹pi³ b³¹d", MB_OK);
     exit(-1);
 }

   try{
              // tymczasowa tablica na wczytany plik
              unsigned char* pchar = new unsigned char[rozm*12];
              tym->Y = new double[rozm];      // miejsce na pola rezonansowe


          S1=Nazwa+"   \t\tnumer ";
          S2 = Plik;
          S1=S1+S2;
          Form1->Memo1->Lines->Add(S1);
          fseek(wejscie,356,SEEK_SET);
          fread(pchar,1,rozm*12,wejscie);
          tym->MaxY=-MaxInt;
          tym->MinY=MaxInt;
          double TymD;
          for(int z=0;z<rozm;z++)  // Wczytywanie wartoœci wychyleñ
            {
             for(int j=0;j<6;j++) L[j]=*(pchar+6+z*12+j);

             TymD=tym->Y[z]=RealToDouble(L);
             if(TymD > tym->MaxY)
             {
               tym->MaxY=TymD;
               tym->IndMax=z;
             }
             else if(TymD < tym->MinY)
             {
               tym->MinY=TymD;
               tym->IndMin=z;
             }

            }
          tym->dBMin=tym->IndMin;
          tym->dBMax=tym->IndMax;
          if(tym->MinY<ZbiorTablic::GlobalMinY) ZbiorTablic::GlobalMinY=tym->MinY;
          if(tym->MaxY>ZbiorTablic::GlobalMaxY) ZbiorTablic::GlobalMaxY=tym->MaxY;
          fclose(wejscie);
          delete[] pchar;
          return tym->Y;
          }
   catch(std::bad_alloc)
          {   // ENTER THIS BLOCK ONLY IF std::bad_alloc IS THROWN.
              S1= "Nie mogê przydzieliæ pamiêci dla pliku  ";
              S2= Nazwa;
              S1=S1+S2;
              S1=S2+ " Bye ...";
              Application->MessageBox(S1.c_str(), "Wyst¹pi³ b³¹d", MB_OK);
              exit(-1);
          }
}


// Akt - lokalny numer , Plik - unikalny numer pliku
double* TabAsocInt::DodajDat(int Akt, int Plik, AnsiString Nazwa, int LWierszy, int rozm)
{
 TabAsocInt* tym=this;
 while(tym->Nast)            // idŸ na koniec listy
       tym=tym->Nast;
 tym->Nast = new TabAsocInt;
 tym->Nast->AktNumer=Akt;
 tym->Nast->NumerPliku=Plik;
 tym->Nast->Wartosc=Ekstrakt(Nazwa);
 tym=tym->Nast;
 FILE* wejscie=fopen(Nazwa.c_str(),"rb");
  struct stat struktura;
    /*
       st_mode	Bit mask giving information about the file's mode
       st_dev	Drive number of disk containing the file or file handle if the file is on a device
       st_rdev	Same as st_dev
       st_nlink	Set to the integer constant 1
       st_size	Size of the file in bytes
       st_atime	Most recent access (Windows) or last time modified (DOS)
       st_mtime	Same as st_atime
       st_ctime	Same as st_atime
    */
   stat(Nazwa.c_str(),&struktura);
   tym->Czas=struktura.st_ctime;
 if (wejscie == NULL)
 {
     S1= "Nie mogê otworzyæ pliku ";
     S2= Nazwa;
     S1=S1+S2;
     S1=S2+ " Bye ...";
     Application->MessageBox(S1.c_str(), "Wyst¹pi³ b³¹d", MB_OK);
     exit(-1);
 }

   try{
              // tymczasowa tablica na wczytany plik
              unsigned char* pchar = new  char[rozm];
              tym->Y = new double[LWierszy];      // miejsce na pola rezonansowe


          S1=Nazwa+"   \t\tnumer ";
          S2 = Plik;
          S1=S1+S2;
          Form1->Memo1->Lines->Add(S1);
          //fseek(wejscie,356,SEEK_SET);
          fread(pchar,rozm,1,wejscie);
          tym->MaxY=-MaxInt;
          tym->MinY=MaxInt;
          double TymD;
          char* TymChar=pchar;
          int j=0;
          for(int z=0;z<rozm;z++)  // Wczytywanie wartoœci wychyleñ
          {

             while(!isdigit(TymChar[z]) && TymChar[z]!='-') z++;
             //sscanf(TymChar+z,"%lf",&LD);
            while(isdigit(TymChar[z]) || TymChar[z]=='.' || TymChar[z]=='-') z++; // Przeskocz cyfy
            while(!isdigit(TymChar[z]) && TymChar[z]!='-') z++; // do nastêpnej liczby
            sscanf(TymChar+z,"%lf", &TymD);
            tym->Y[j++]=TymD;
            while(isdigit(TymChar[z]) || TymChar[z]=='.' || TymChar[z]=='-') z++; // Przeskocz cyfy


             if(TymD > tym->MaxY)
             {
               tym->MaxY=TymD;
               tym->IndMax=j;
             }
             else if(TymD < tym->MinY)
             {
               tym->MinY=TymD;
               tym->IndMin=j;
             }
            if (z>=rozm || j>=LWierszy) break;
          }
          Form1->Memo1->Lines->Add("Liczba wczytanych Wierszy = "+AnsiString(j));
          tym->dBMin=tym->IndMin;
          tym->dBMax=tym->IndMax;
          if(tym->MinY<ZbiorTablic::GlobalMinY) ZbiorTablic::GlobalMinY=tym->MinY;
          if(tym->MaxY>ZbiorTablic::GlobalMaxY) ZbiorTablic::GlobalMaxY=tym->MaxY;
          fclose(wejscie);
          delete[] pchar;
          return tym->Y;
          }
   catch(std::bad_alloc)
          {   // ENTER THIS BLOCK ONLY IF std::bad_alloc IS THROWN.
              S1= "Nie mogê przydzieliæ pamiêci dla pliku  ";
              S2= Nazwa;
              S1=S1+S2;
              S1=S2+ " Bye ...";
              Application->MessageBox(S1.c_str(), "Wyst¹pi³ b³¹d", MB_OK);
              exit(-1);
          }
}

double* TabAsocInt::DodajEPR(int Akt, int Plik, AnsiString Nazwa, int LWierszy, int rozm)
{
 TabAsocInt* tym=this;
 while(tym->Nast)            // idŸ na koniec listy
       tym=tym->Nast;
 tym->Nast = new TabAsocInt;
 tym->Nast->AktNumer=Akt;
 tym->Nast->NumerPliku=Plik;
 tym->Nast->Wartosc=Ekstrakt(Nazwa);
 tym=tym->Nast;
 FILE* wejscie=fopen(Nazwa.c_str(),"rb");
  struct stat struktura;
    /*
       st_mode	Bit mask giving information about the file's mode
       st_dev	Drive number of disk containing the file or file handle if the file is on a device
       st_rdev	Same as st_dev
       st_nlink	Set to the integer constant 1
       st_size	Size of the file in bytes
       st_atime	Most recent access (Windows) or last time modified (DOS)
       st_mtime	Same as st_atime
       st_ctime	Same as st_atime
    */
   stat(Nazwa.c_str(),&struktura);
   tym->Czas=struktura.st_ctime;
   //rozm = struktura.st_size;
 if (wejscie == NULL)
 {
	 S1= "Nie mogê otworzyæ pliku ";
	 S2= Nazwa;
	 S1=S1+S2;
	 S1=S2+ " Bye ...";
	 Application->MessageBox(S1.c_str(), "Wyst¹pi³ b³¹d", MB_OK);
	 exit(-1);
 }

   try{
              // tymczasowa tablica na wczytany plik
              unsigned char* pchar = new  char[rozm];
              tym->Y = new double[LWierszy];      // miejsce na pola rezonansowe


          S1=Nazwa+"   \t\tnumer ";
		  S2 = Plik;
		  S1=S1+S2;
		  Form1->Memo1->Lines->Add(S1);
		  //fseek(wejscie,356,SEEK_SET);
		  fread(pchar,rozm,1,wejscie);
		  tym->MaxY=-MaxInt;
		  tym->MinY=MaxInt;
		  double TymD;
		  char* TymChar=pchar;
		  TymChar=strstr(TymChar,"F0 S0");
		  TymChar+=strlen("F0 S0")+2;
		  int j=0;
		  int roznica = TymChar-pchar;
		  for(int z=0;z<rozm-roznica;z++)  // Wczytywanie wartoœci wychyleñ
		  {

			while(!isdigit(TymChar[z]) && TymChar[z]!='-') z++;
			 //sscanf(TymChar+z,"%lf",&LD);
			while(isdigit(TymChar[z]) || TymChar[z]=='.' || TymChar[z]=='-') z++; // Przeskocz cyfy
			while(!isdigit(TymChar[z]) && TymChar[z]!='-') z++; // do nastêpnej liczby
			sscanf(TymChar+z,"%lf", &TymD);
			tym->Y[j++]=TymD;
			while(isdigit(TymChar[z]) || TymChar[z]=='.' || TymChar[z]=='-') z++; // Przeskocz cyfy


			 if(TymD > tym->MaxY)
			 {
			   tym->MaxY=TymD;
			   tym->IndMax=j;
			 }
			 else if(TymD < tym->MinY)
			 {
			   tym->MinY=TymD;
			   tym->IndMin=j;
			 }
			if (z>=rozm || j>=LWierszy) break;
		  }
		  Form1->Memo1->Lines->Add("Liczba wczytanych Wierszy = "+AnsiString(j));
		  tym->dBMin=tym->IndMin;
		  tym->dBMax=tym->IndMax;
		  if(tym->MinY<ZbiorTablic::GlobalMinY) ZbiorTablic::GlobalMinY=tym->MinY;
		  if(tym->MaxY>ZbiorTablic::GlobalMaxY) ZbiorTablic::GlobalMaxY=tym->MaxY;
		  fclose(wejscie);
		  delete[] pchar;
		  return tym->Y;
		  }
   catch(std::bad_alloc)
		  {   // ENTER THIS BLOCK ONLY IF std::bad_alloc IS THROWN.
			  S1= "Nie mogê przydzieliæ pamiêci dla pliku  ";
			  S2= Nazwa;
			  S1=S1+S2;
			  S1=S2+ " Bye ...";
			  Application->MessageBox(S1.c_str(), "Wyst¹pi³ b³¹d", MB_OK);
			  exit(-1);
		  }
}


int DajPunktBMax(int NrP)
{
       ZbiorTablic* Akt=DajZb(NrP);
        return Akt->DajLinie(NrP)->dBMax;
}

int DajPunktBMin(int NrP)
{
  ZbiorTablic* Akt=DajZb(NrP);
  return Akt->DajLinie(NrP)->dBMin;
}

int UstPunktBMax(int NrP, int Wart)
{
       ZbiorTablic* Akt=DajZb(NrP);
        return Akt->DajLinie(NrP)->dBMax=Wart;
}

int UstPunktBMin(int NrP, int Wart)
{
  ZbiorTablic* Akt=DajZb(NrP);
  return Akt->DajLinie(NrP)->dBMin=Wart;
}
