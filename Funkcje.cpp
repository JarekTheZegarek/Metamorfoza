//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Funkcje.h"
#include "metamorfozis_w5.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

double RealToDouble(real Liczba)
{
  real L;
  char tym;
  for(int i=0;i<6;i++) L[i]=Liczba[i];
  for(int i=0;i<3;i++)
  {
   tym=L[i];          //Zamie� kolejnosc bajt�w
   L[i]=L[5-i];  //gdy� s� wczytywane odwrotnie
   L[5-i]=tym;
  }
  unsigned char wzorzec=0x80;
  unsigned char cecha;
//po bicie znaku pierwsze 39 bit�w
//stanowi mantyse. Typ real mozna otrzymac
//znak*{2^(cecha-129)}*{1+suma(2^-[pozycja bitu mantysy])}

  if ((cecha=L[5])==0) //cecha to ostatni bajt typu real
              return 0;  //je�eli cecha =0 to liczba te�.

    union bitydouble
    { //wzorzec typu double
      //1       : bit znaku
      //2 - 12  : bity wyk�adnika (uzupe�niane do 1023)
      //12 - 64 : bity matntysy
     double liczba;
     int cecha;
     unsigned char reprezentacja[8];
    };
    bitydouble rezultat;
    for(int i=0;i<8;i++) rezultat.reprezentacja[i]=0; //wyzerowanie unii
    //ustalenie znaku - wzorzec binarny = 1000 0000
    rezultat.reprezentacja[1]=wzorzec & L[0]; //wyzerowanie drugiego bajtu
                                //gdyz nastepnie dwa pierwsze bajty
                                //odrw�cimy ze wzgl�du na reprezentacj�
                                //liczby ca�kowitej
    int cecha_dbl;
    cecha_dbl = cecha-129;      // ekstrkcja cechy liczby typu real
    cecha_dbl+=1023;            // zapis cechy liczby typu double
    rezultat.cecha|=(cecha_dbl<<4); //umieszczenie cechy double
                                    //zajmuj�cej 11 bit�w. (pierwszy bit
                                    //jest bitem znaku)
    //Zamiana kolejnosci bitow w zmienej integer (rezultat.cecha);
        tym=rezultat.reprezentacja[0];
        rezultat.reprezentacja[0]=rezultat.reprezentacja[1];
        rezultat.reprezentacja[1]=tym;
    L[0]&=0x7f; // wyzerowanie bitu znaku w liczbie real (& 0111 1111);

//umieszczenie bit�w mantysy w kolejnych bitach reprezentacji;
    rezultat.reprezentacja[1]|=(L[0]>>3);
    for(int i=0;i<4;i++)
    {
     rezultat.reprezentacja[i+2]|=(L[i]<<5);
     rezultat.reprezentacja[i+2]|=(L[i+1]>>3);
    }
    rezultat.reprezentacja[6]|=(L[4]<<5);
//kolejno�� BAJT�W jest dla reprezentacji double odwrotna (w tej unii)
    for(int i=0;i<4;i++)
    {
     tym=rezultat.reprezentacja[i];          //Zamie� kolejnosc bajt�w
     rezultat.reprezentacja[i]=rezultat.reprezentacja[7-i];  //gdy� s� wczytywane odwrotnie
     rezultat.reprezentacja[7-i]=tym;
    }

    return rezultat.liczba;

}


// Funkcja tworzy kolekcj� linii powi�zanych wsp�lnym zakresem
// p�l magnetycznych
// narazie bez wczytywania i zwraca wska�nik na t� kolekcj�
// Dostarczamy do funkjcji uchwyt i nazw� pliku, oraz
// unikalny Numer (kt�ry, b�dzie identyfikowa� lini�).

ZbiorTablic* DodajDat(FILE* plik,AnsiString& Nazwa,int Numer)
{ // plik jest ju� otwarty do czytania
  struct stat struktura;
  stat(Nazwa.c_str(),&struktura);
  size_t RozmiarPliku = struktura.st_size;
  int LiczbaWierszy=0;
  char* TymPlik = new char[RozmiarPliku];
  fread(TymPlik,RozmiarPliku,1,plik);
  char Tznak;
  for(int i=0;i<RozmiarPliku;i++)
	 if ((Tznak=TymPlik[i])=='\n') LiczbaWierszy++;
  Form1->Memo1->Lines->Add("Rozmiar Pliku = " + AnsiString(RozmiarPliku)+"\nA liczba wierszy "
                                + AnsiString(LiczbaWierszy));
//  ShowMessage("Rozmiar Pliku = " + AnsiString(RozmiarPliku)+"\nA liczba wierszy "
//                                + AnsiString(LiczbaWierszy));

  int rozmiar = LiczbaWierszy;  // Dla zgodno�ci z poprzednio napisanym kodem
/*  rozmiar = (struktura.st_size-356)/12; // liczba wierszy danych
                                            // w pliku ktore rozpoczynaj�
											// si� od 356 bajtu          */
  if (rozmiar<1024)
  {
        AnsiString Wiadomosc =  "Prawdopodobnie plik Zawiera mniej ni� 1024 punkt�w \n";
		Wiadomosc+=             "W�a�ciwie to oko�o "+AnsiString(rozmiar)+" puntk�w\n"
                                "           Czy wczytywa� go dalej?   ";
		if(Application->MessageBox(Wiadomosc.c_str(), "UWAGA!!!",MB_YESNO)==IDNO)
            return NULL; // u�ytkwnik zrezygnowa� z wczytania
  }


  if(ZbiorTablic::MaxRozmiar<rozmiar) ZbiorTablic::MaxRozmiar = rozmiar;
//  fseek(plik,356,SEEK_SET);
  char x[4*sizeof(real)];   // wczytywanie pierwszych czterech liczb
							// zawieraj�cych x1,y1, x2, y2,
                            // x-sy s�u�� do okre�lenia kolekcji linii
  real L,L2;
  double xsy[2], xtym;
  int wyn  = sscanf(TymPlik,"%lf",&xsy[0]); //   fread(x,1,4*sizeof(real),plik);
  Tznak=TymPlik[0];
  int i;
  for( i=0;Tznak!='\n';i++) Tznak=TymPlik[i];
  int wyn2 = sscanf(TymPlik+i,"%lf",&xsy[1]);
  Form1->Memo1->Lines->Add( " X1 = "+AnsiString(xsy[0])+" X2 = "+AnsiString(xsy[1]));
//  ShowMessage(" X1 = "+AnsiString(xsy[0])+" X2 = "+AnsiString(xsy[1]));
 // return NULL;
  delete[] TymPlik;
/*  if (wyn==0 || wyn2==0) return NULL; // je�eli wczytanie si� nie powiod�o
  for(int i=0;i<6;i++)
     {
	   L[i]=*(x+i);                   // kopiowanie pierwszych 6 wczytanych bajt�w (X1);
       L2[i]=*(x+2*sizeof(real)+i); // kopiowanie nast�pego X-sa odleg�ego o sizeof(X1+Y1)od pocz�tku
	 }
  xsy[0]=RealToDouble(L);
  xsy[1]=RealToDouble(L2); */
  if(!ZbiorTablic::glowa) // je�eli dodajemy pierwszy plik
  {
   ZbiorTablic* T = new ZbiorTablic;
   if (T)
     ZbiorTablic::glowa = T;
   else
     return NULL;
	T->rozmiar = rozmiar; //liczba par x,y
    T->X1 =xsy[0]; T->X2=xsy[1];
	T->Dodaj(Nazwa,Numer,RozmiarPliku);
    return T;
  }

  // Przel�danie wczytanych kolekcji linii i dodawanie do
  // w�a�ciwego zbioru linii
  ZbiorTablic* akt = ZbiorTablic::glowa;
  do
  {
   if(akt->X1==xsy[0] && akt->X2==xsy[1] && akt->rozmiar==rozmiar)
   // je�eli ten sam zakres p�l
   {
      akt->Dodaj(Nazwa,Numer,RozmiarPliku);
	  return akt;
   }
  }
  while(akt=akt->nast);

  // je�eli nie znaleziono pasuj�cego zakresu za�� nowy ZbiorTablic
  ZbiorTablic* Tym = new ZbiorTablic;
  akt=ZbiorTablic::glowa;
  while(akt->nast) akt=akt->nast;  // przewi� na koniec listy
  akt->nast=Tym;                // do��cz wstawiany obiekt
 // Tym->ilosc=0;
  Tym->rozmiar=rozmiar;
  Tym->X1=xsy[0];Tym->X2=xsy[1];
  Tym->Dodaj(Nazwa,Numer,RozmiarPliku);
  return Tym;
}

//-------------------------------------------------------------------------------------------


ZbiorTablic* DodajEPR(FILE* plik,AnsiString& Nazwa,int Numer)
{ // plik jest ju� otwarty do czytania
  struct stat struktura;
  stat(Nazwa.c_str(),&struktura);
  size_t RozmiarPliku = struktura.st_size;
  int LiczbaWierszy=0;
  char* TymPlik = new char[RozmiarPliku];
  fread(TymPlik,RozmiarPliku,1,plik);
  char Tznak;
  for(int i=0;i<RozmiarPliku;i++)
	 if ((Tznak=TymPlik[i])=='\n') LiczbaWierszy++;
  Form1->Memo1->Lines->Add("Rozmiar Pliku = " + AnsiString(RozmiarPliku)+"\nA liczba wierszy "
								+ AnsiString(LiczbaWierszy));
//  ShowMessage("Rozmiar Pliku = " + AnsiString(RozmiarPliku)+"\nA liczba wierszy "
//                                + AnsiString(LiczbaWierszy));

  int rozmiar = LiczbaWierszy;  // Dla zgodno�ci z poprzednio napisanym kodem
/*  rozmiar = (struktura.st_size-356)/12; // liczba wierszy danych
											// w pliku ktore rozpoczynaj�
											// si� od 356 bajtu          */
  if (rozmiar<1024)
  {
		AnsiString Wiadomosc =  "Prawdopodobnie plik Zawiera mniej ni� 1024 punkt�w \n";
		Wiadomosc+=             "W�a�ciwie to oko�o "+AnsiString(rozmiar)+" puntk�w\n"
								"           Czy wczytywa� go dalej?   ";
		if(Application->MessageBox(Wiadomosc.c_str(), "UWAGA!!!",MB_YESNO)==IDNO)
			return NULL; // u�ytkwnik zrezygnowa� z wczytania
  }


  if(ZbiorTablic::MaxRozmiar<rozmiar) ZbiorTablic::MaxRozmiar = rozmiar;
//  fseek(plik,356,SEEK_SET);
//  char x[4*sizeof(real)];   // wczytywanie pierwszych czterech liczb
							// zawieraj�cych x1,y1, x2, y2,
							// x-sy s�u�� do okre�lenia kolekcji linii
  real L,L2;
  double xsy[2], xtym;
  char* aktualny = TymPlik;

  aktualny = strstr(TymPlik,"Samples=");
  aktualny+=strlen("Samples=");
  int LiczbaZmiennych = atoi(aktualny);
 // ShowMessage(AnsiString("Liczba zmiennych = ")+ AnsiString(LiczbaZmiennych));
  rozmiar = LiczbaZmiennych;
  //while(
  aktualny = strstr(TymPlik,"F0 S0");
  aktualny+= strlen("F0 S0")+2;
  int wyn  = sscanf(aktualny,"%lf",&xsy[0]); //   fread(x,1,4*sizeof(real),plik);
  Tznak=aktualny[0];
  int i;
  for( i=0;Tznak!='\n';i++) Tznak=aktualny[i];
  int wyn2 = sscanf(aktualny+i,"%lf",&xsy[1]);
  Form1->Memo1->Lines->Add( " X1 = "+AnsiString(xsy[0])+" X2 = "+AnsiString(xsy[1]));
  delete[] TymPlik;
/*  if (wyn==0 || wyn2==0) return NULL; // je�eli wczytanie si� nie powiod�o
  for(int i=0;i<6;i++)
	 {
	   L[i]=*(x+i);                   // kopiowanie pierwszych 6 wczytanych bajt�w (X1);
	   L2[i]=*(x+2*sizeof(real)+i); // kopiowanie nast�pego X-sa odleg�ego o sizeof(X1+Y1)od pocz�tku
	 }
  xsy[0]=RealToDouble(L);
  xsy[1]=RealToDouble(L2); */
  if(!ZbiorTablic::glowa) // je�eli dodajemy pierwszy plik
  {
   ZbiorTablic* T = new ZbiorTablic;
   if (T)
	 ZbiorTablic::glowa = T;
   else
	 return NULL;
	T->rozmiar = rozmiar; //liczba par x,y
	T->X1 =xsy[0]; T->X2=xsy[1];
	T->Dodaj(Nazwa,Numer,RozmiarPliku);
	return T;
  }

  // Przel�danie wczytanych kolekcji linii i dodawanie do
  // w�a�ciwego zbioru linii
  ZbiorTablic* akt = ZbiorTablic::glowa;
  do
  {
   if(akt->X1==xsy[0] && akt->X2==xsy[1] && akt->rozmiar==rozmiar)
   // je�eli ten sam zakres p�l
   {
	  akt->Dodaj(Nazwa,Numer,RozmiarPliku);
	  return akt;
   }
  }
  while(akt=akt->nast);

  // je�eli nie znaleziono pasuj�cego zakresu za�� nowy ZbiorTablic
  ZbiorTablic* Tym = new ZbiorTablic;
  akt=ZbiorTablic::glowa;
  while(akt->nast) akt=akt->nast;  // przewi� na koniec listy
  akt->nast=Tym;                // do��cz wstawiany obiekt
 // Tym->ilosc=0;
  Tym->rozmiar=rozmiar;
  Tym->X1=xsy[0];Tym->X2=xsy[1];
  Tym->Dodaj(Nazwa,Numer,RozmiarPliku);
  return Tym;
}


ZbiorTablic* DodajPlik(FILE* plik, AnsiString& Nazwa,int Numer)
{
  struct stat struktura;
  stat(Nazwa.c_str(),&struktura);
  int rozmiar = (struktura.st_size-356)/12; // liczba wierszy danych
                                            // w pliku ktore rozpoczynaj�
                                            // si� od 356 bajtu
  if (rozmiar<1024)
  {
        AnsiString Wiadomosc =  "Prawdopodobnie plik Zawiera mniej ni� 1024 punkt�w \n";
        Wiadomosc+=             "W�a�ciwie to oko�o "+AnsiString(rozmiar)+" puntk�w\n"
                                "           Czy wczytywa� go dalej?   ";
        switch(Application->MessageBox(Wiadomosc.c_str(), "UWAGA!!!",MB_YESNOCANCEL))
        {
        case IDNO:
                   return NULL; // u�ytkwnik zrezygnowa� z wczytania
                   break;
        case IDCANCEL:
                   ShowMessage("Przepraszam! Uruchom program od pocz�tku");
                   Application->Terminate();
        }
  }
  if(ZbiorTablic::MaxRozmiar<rozmiar) ZbiorTablic::MaxRozmiar = rozmiar;
  fseek(plik,356,SEEK_SET);
  char x[4*sizeof(real)];   // wczytywanie pierwszych czterech liczb
                            // zawieraj�cych x1,y1, x2, y2,
                            // x-sy s�u�� do okre�lenia kolekcji linii
  real L,L2;
  double xsy[2];
  int wyn = fread(x,1,4*sizeof(real),plik);
  if (wyn==0) return NULL; // je�eli wczytanie si� nie powiod�o
  for(int i=0;i<6;i++)
     {
       L[i]=*(x+i);                   // kopiowanie pierwszych 6 wczytanych bajt�w (X1);
       L2[i]=*(x+2*sizeof(real)+i); // kopiowanie nast�pego X-sa odleg�ego o sizeof(X1+Y1)od pocz�tku
     }
  xsy[0]=RealToDouble(L);
  xsy[1]=RealToDouble(L2);
  if(!ZbiorTablic::glowa) // je�eli dodajemy pierwszy plik
  {
   ZbiorTablic* T = new ZbiorTablic;
   if (T)
     ZbiorTablic::glowa = T;
   else
     return NULL;
    T->rozmiar = rozmiar; //liczba par x,y
    T->X1 =xsy[0]; T->X2=xsy[1];
    T->Dodaj(Nazwa,Numer);
    return T;
  }

  // Przel�danie wczytanych kolekcji linii i dodawanie do
  // w�a�ciwego zbioru linii
  ZbiorTablic* akt = ZbiorTablic::glowa;
  do
  {
   if(akt->X1==xsy[0] && akt->X2==xsy[1] && akt->rozmiar==rozmiar)
   // je�eli ten sam zakres p�l
   {
      akt->Dodaj(Nazwa,Numer);
      return akt;
   }
  }
  while(akt=akt->nast);

  // je�eli nie znaleziono pasuj�cego zakresu i funkcja si�
  // jescze nie zako�czy�a to za�� nowy ZbiorTablic
  ZbiorTablic* Tym = new ZbiorTablic;
  akt=ZbiorTablic::glowa;
  while(akt->nast) akt=akt->nast;  // przewi� na koniec listy
  akt->nast=Tym;                // do��cz wstawiany obiekt
 // Tym->ilosc=0;
  Tym->rozmiar=rozmiar;
  Tym->X1=xsy[0];Tym->X2=xsy[1];
  Tym->Dodaj(Nazwa,Numer);
  return Tym;
}


ZbiorTablic::~ZbiorTablic()
{
 delete Pliki;
 if(Tab) delete[] Tab;
 Asoc->Zwolnij();
}


double DajY(int x,int NrPlik) throw(ErrorDajY)
{
   ZbiorTablic* tym = ZbiorTablic::glowa;
   while(tym)
   {
//     for(int i=0;i<tym->ilosc;i++) // Znajd� Indeks napisu

        if(tym->DajIndeks(NrPlik)>=0)
               return tym->Tablica(x,tym->DajIndeks(NrPlik)+1); // Warto�ci Y zaczynaj� si� od indeksu jeden
     tym=tym->nast;
   }
   throw ErrorDajY();
}

// a - jest indeksem tablicy X-s�w (B[mT]). NrPliku s�u�y tylko do identyfikacji w�a�ciwego ZbioruTablic
double DajX(int a,int NrPliku) throw(ErrorDajX)
       {
             ZbiorTablic* tym = ZbiorTablic::glowa;
             while(tym)
             {
               if(tym->DajIndeks(NrPliku)>=0)
                 return tym->Tablica(a, 0);
               tym=tym->nast;
             }
             throw ErrorDajX();
       }


String DajNazweZbioru(int NrPlik)   throw(ErrorNazwaZb)
{
   ZbiorTablic* tym = ZbiorTablic::glowa;
   while(tym)
   {
        if(tym->DajIndeks(NrPlik)>=0) return tym->Pliki->Strings[tym->DajIndeks(NrPlik)];
     tym=tym->nast;
   }
   throw ErrorNazwaZb();
}



//Chyba zwraca wska�nik na zbi�r tablic zawieraj�cy dane z pliku o NrPliku
// w wypadku niepowidzenia zwraca 0.
ZbiorTablic* DajZb(int NrPlik) throw(ErrorDajZb)
{
   ZbiorTablic* tym = ZbiorTablic::glowa;
   while(tym)
   {
   //  for(int i=0;i<tym->ilosc;i++) // Znajd� Indeks napisu
        if(tym->DajIndeks(NrPlik)>=0) return tym;
     tym=tym->nast;
   }
   if(tym )throw ErrorDajZb(); //Je�eli nie ma ko�ca listy
   return 0;

}




inline double Odl(int X1, int Y1, int X2, int Y2)
{
 return  sqrt((X1-X2)*(X1-X2)+(Y1-Y2)*(Y1-Y2));
}



/*
  Funkcja zwraca numer indeksu kom�rki tablicy zawieraj�ce wsp�rz�dne
  "rzeczywiste" odpowiadaj�ce odpowiedniemu punktowi ekranowemu,
  oczywi�cie dla aktualnej linii.        
*/
int SzukajNajblIndeksu(ZbiorTablic* Akt,int X, int Y)
{
  TPoint Punkt;
  int AktX = X;
  int AktY = Y;
  int ZakrZmIx;          // Jak daleko poszukiwa� najbli�szego punktu
  int Indeks;
  ZakrZmIx=AktWykrLinii->ZakresZaznaczania;
  double odleg, MinOdl=Screen->Width;
  bool JestLiniaWZakresie = false;
  int StartX = ((X-ZakrZmIx)<0)?0:X-ZakrZmIx;
  int StartY = ((Y-ZakrZmIx)<0)?0:Y-ZakrZmIx;
  for (int i=StartX;i<X+ZakrZmIx && i<AktWykrLinii->DajRozmiarPl().x;i++)
    for(int j=StartY;j<Y+ZakrZmIx && j<AktWykrLinii->DajRozmiarPl().y;j++)
    {
       if(AktWykrLinii->Plotno->Pixels[i][j]==AktWykrLinii->KolorLinii
          || AktWykrLinii->Plotno->Pixels[i][j]==Form1->Zaznaczenie->WezKolorLinii()
          || AktWykrLinii->Plotno->Pixels[i][j]==Form1->Doswiadczalny->WezKolorLinii())
       {
          JestLiniaWZakresie=true;
          odleg=Odl(i,j,X,Y);
          if(odleg<MinOdl)
           { MinOdl=odleg;
             //   AktX =x1 ; AktY=y1;
                  Indeks=i;
            }
          //break;           //Przerywa tylko wewn�trzn� p�tl� i dlatego w
       }                   //zewn�trznej dodatkowy warunek !JestLiniaW...
    }
  if(!JestLiniaWZakresie) return -1;  // je�eli dook�a  jest pusto
  // obliczanie startowego indeksu tablicy z danymi tj. przekszta�cenie wsp�rz�dnych
  // ekranowych na indeks tablicy zawieraj�cy dopowiedni punkt rzeczywisty
  int StartIndeksX = int(Indeks/WspX) ;//int(X/WspX);
  //int StartIndeksX=Indeks;
  int y1,x1;
 Indeks=StartIndeksX;
 MinOdl=Screen->Width;
  for(int i=StartIndeksX;i<StartIndeksX+ZakrZmIx && i<Akt->LiczWierszy()-1;i++)
  {  // Sprawd� odleglo�� 20 punkt�w (nie pixeli!) w praw� stron�
     /*y1=int(DajY(i, NumerPliku)*WspY_B+WspY_C);
           y1-=popr/2;
     x1= int(WspX*i);
     odleg=Odl(x1,y1,X,Y);*/
     Punkt= AktWykrLinii->DajWsp(i,DajY(i,NumerPliku));
     odleg=Odl(Punkt.x,Punkt.y,X,Y);
     if(odleg<MinOdl)
           { MinOdl=odleg;
          //   AktX =x1 ; AktY=y1;
             Indeks=i;
           }
  }
  for(int i=StartIndeksX;i>StartIndeksX-ZakrZmIx && i>=0;i--)
  {  // Sprawd� odleglo�� 20 punkt�w (nie pixeli!) w lew� stron�
    /* y1=int(DajY(i, NumerPliku)*WspY_B+WspY_C);
           y1-=popr/2;
     x1= int(WspX*i);
     odleg=Odl(x1,y1,X,Y);*/
     Punkt= AktWykrLinii->DajWsp(i,DajY(i,NumerPliku));
     odleg=Odl(Punkt.x,Punkt.y,X,Y);
     if(odleg<MinOdl)
           { MinOdl=odleg;
            // AktX =x1 ; AktY=y1;
             Indeks=i;
           }
  }
  return Indeks;
}

TPoint SzukajNajblizszego(TCanvas *Pl, ZbiorTablic* Akt,int X, int Y)
{
  //TPoint Punkt;
  int StartIndeksX = int(X/WspX);
  int AktX = X;
  int AktY = Y;
  int Indeks, IndeksRos=StartIndeksX;
  int IndeksMal=StartIndeksX;
  int y1,y2;
  for(int i=StartIndeksX;i<Akt->LiczWierszy()-1;i++)  // Znajd� najbli�ej po�o�ony
  {                                                   // punkt wykresu w stron� rosnacych X-s�w
     y1=int(DajY(i, NumerPliku)*WspY_B+WspY_C);
           y1-=popr/2;
     y2=int(DajY(i+1,NumerPliku)*WspY_B+WspY_C);
           y2-=popr/2;
     if( Y>=y1 && Y<=y2) {
                           IndeksRos=i;
                           break;
                          }
  }
  for(int i=StartIndeksX;i>0;i--) // Znajd� w stron� malej�cych X-s�w
  {
     y1=int(DajY(i-1, NumerPliku)*WspY_B+WspY_C);
           y1-=popr/2;
     y2=int(DajY(i,NumerPliku)*WspY_B+WspY_C);
           y2-=popr/2;
     if( Y>=y1 && Y<=y2) {
                           IndeksMal=i;
                           break;
                          }
  }
  //Indeks = Minimalna odleglosc punktu od lini w kierunku x-sowym
  Indeks = ((IndeksRos-StartIndeksX)<(StartIndeksX-IndeksMal))?IndeksRos:IndeksMal;
  AktX = int(WspX*Indeks);
  AktY = int(DajY(Indeks,NumerPliku)*WspY_B+WspY_C)-popr/2;
  int XX,YY;
  double StartOdl = Odl(X,Y,AktX,AktY);
  X = AktX;  Y= AktY;                                   // Nowy punkt zaczepienia
  double odleg, MinOdlRos=StartOdl, MinOdlMal=StartOdl;
  int i;
  for(i=Indeks;i<Indeks+20 && i < Akt->LiczWierszy();i++)
   {
        XX=int(WspX*i);
        YY=int(DajY(i,NumerPliku)*WspY_B+WspY_C)-popr/2;
        odleg=Odl(XX,YY,AktX,AktY);
        if(odleg<MinOdlRos)
           { MinOdlRos=odleg;
             X =XX ; Y=YY;
           }
   }
   if(MinOdlRos<StartOdl && i<Akt->LiczWierszy())
   {
     bool straz = true;
     do {            // szukaj minimalnej odleg�o�ci po prawej stronie
        i++;
        XX=int(WspX*i);
        YY=int(DajY(i,NumerPliku)*WspY_B+WspY_C)-popr/2;
        odleg=Odl(XX,YY,AktX,AktY);
        if(odleg<MinOdlRos)   
           { MinOdlRos=odleg;
             X =XX ; Y=YY;
             straz = false;
           }
        }
        while( straz && i< Akt->LiczWierszy());
   }
   else   // Je�eli nie to szukaj po lewej stronie
       {
        i=Indeks; bool straz=true;
        while(straz )
        { i--;
          XX=int(WspX*i);
          YY=int(DajY(i,NumerPliku)*WspY_B+WspY_C)-popr/2;
          odleg=Odl(XX,YY,AktX,AktY);
          if(odleg<MinOdlMal)
             { MinOdlMal=odleg;
               X =XX ; Y=YY;
               straz=false;
             }
        }
       }
  return TPoint(X,Y);
}

void zamien(int& a, int& b)
{
  int tym = a;
  a = b;
  b = tym;
}

void UstawW(int Tab[], int rozm)
{
    ZbiorTablic* Akt;
    for(int i=0;i<rozm;i++)  //NumeryPlikow od Zera
    {
      if(Akt=DajZb(i+1))
      {
        TablicaWart[0][i]=i+1;  // Wpisz Numery Plik�w
        //Akt=DajZb(i+1);
        TablicaWart[1][i]= Akt->DajWartosc(i+1);
        Tab[i]= i+1 ; //Inicjuj TablicaNrPlikow;
      }
      else
         TablicaWart[0][i]=0;  // Gdy nie ma takiego numeru pliku

    }

}


// Sortowanie warto�ci stowarzyszonych z plikami
// prosta metoda b�belkowa; wynik: posortowane numery plikow
// umieszczone w tablicy Tab; kierunke r = rosn�cy m=malej�cy;
void Sortuj(int Tab[],int rozm,char kierunek)
{
 UstawW(Tab, rozm);
 int i,j, jmax=rozm-1;
 int NrPliku,NrNast;
 switch(kierunek)
 {
  case 'r':                             //rosn�co
       for(i=0;i<jmax;i++,j--)
          for(j=0; j<jmax;j++)
          {   NrPliku = Tab[j]-1;
              NrNast=Tab[j+1]-1;
              if (TablicaWart[1][NrPliku] > TablicaWart[1][NrNast])
                           zamien(Tab[j],Tab[j+1]);
          }
        break;
  case 'm':                             //malej�co
       for(i=0;i<jmax;i++)
       {
          for(j=0; j<jmax;j++)
          {   NrNast=Tab[j+1]-1;
              NrPliku = Tab[j]-1;
              if (TablicaWart[1][NrPliku] < TablicaWart[1][NrNast])
                           zamien(Tab[j],Tab[j+1]);
          }
       }
          break;
  }
 /*
  procedure SortujNazwiska(var T: array of baza; zakres:integer);
Var
  i,j, jmax: integer;
  tym: baza;
Begin
  jmax:=zakres-1;
  For i:=0 To zakres-1 Do
  begin
    For j:=0 To jmax Do
    begin
       if T[j].nazwisko > T[j+1].nazwisko Then
       begin
          tym :=T[j];
          T[j] := T[j+1];
          T[j+1]:=tym;
       end;
    end;
    jmax:=jmax-1;
  end;
End;
 */
}



