//---------------------------------------------------------------------------


#pragma hdrstop

#include "Odczyt.h"
#include "metamorfozis_w5.h"
#include "Rejestr.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
static int Ost_Wczyt =0;


bool  TForm1::Sprawdz()
{

	if(!ListaPlikow) ListaPlikow = new TStringList;     // Dodaj do ListyPlikow
   // LiczbaWczytPlikow =ListaPlikow->Count;    // liczba dotychczas wczytanych nazw plików
	for(int i=0;i< OpenDialog1->Files->Count;i++)
	{
	 bool unikalny = true;
	 for(int j=0; j<LiczbaWczytPlikow; j++)
	   if(OpenDialog1->Files->Strings[i]==ListaPlikow->Strings[j])
	   {
		 unikalny=false;
		 break;
	   }
	 if(unikalny)
	 {
		  ListaPlikow->Add(OpenDialog1->Files->Strings[i]);
		  Ost_Wczyt++;
	 }
	}
	if(!Ost_Wczyt)
		return false;  // nie dodano nowego pliku
	else
		return true;
}

void TForm1::WczytajMLA()
{
  String S1,S2;
  char drive[_MAX_DRIVE];
  char directory[_MAX_DIR];
  char path[_MAX_PATH];
  if(!Sprawdz()) return;    // sprawdz czy mozna dodac plik
	_splitpath(OpenDialog1->FileName.c_str(),drive,directory,0,0);
	_makepath(path,drive,directory,0,0);
	katalog=path;
	rejestr->zapiszStr("Katalog",path);

  real L;
  FILE* wejscie;
  strstream ekran;
  char wzorzec[]="LBR AA PCLABOR C 2000 Liber"; //pierwsze bajty pliku
  char wspolne[375];
  // ************ Sprawdzanie identyfikatorów plików i dodawanie do kolekcji *************
  // LiczbaWczytPlikow zawiera pierwszy wolny indeks ListyPlikow
  int koniec = LiczbaWczytPlikow+Ost_Wczyt;
  NumerPliku = LiczbaWczytPlikow+1;
  Form1->ProgressBar1->Max=OpenDialog1->Files->Count-1;
  for(int AktNrPliku=0;AktNrPliku<OpenDialog1->Files->Count; AktNrPliku++)
  {
    Form1->ProgressBar1->Position=AktNrPliku;
    AnsiString T=ExtractFileName(OpenDialog1->Files->Strings[AktNrPliku]);
    Form1->LabelPliki->Caption= T;
    for(int i=0;i<375;i++) wspolne[i]='\0';
    wejscie=fopen(OpenDialog1->Files->Strings[AktNrPliku].c_str(),"rb");


    fread(&wspolne,1,strlen(wzorzec),wejscie);
        if (strcmp(wzorzec,wspolne)!=0)
        {
          S1 = "Plik "+OpenDialog1->Files->Strings[AktNrPliku]+" nie pasuje do wzorca\0";
          ekran<<"Plik "<<OpenDialog1->Files->Strings[AktNrPliku].c_str()<<" nie pasuje do wzorca\0";
          Memo1->Lines->Add(ekran.str());
          continue; //return ;
        }
    // DodajPlik jest zdefiniowane w pliku funkcje.cpp
    if (!DodajPlik(wejscie,OpenDialog1->Files->Strings[AktNrPliku],NumerPliku))
    {
     ekran<<"Plik "<<OpenDialog1->Files->Strings[AktNrPliku].c_str()<<" nie zosta³ dodany w procedurze DodajPlik";
     ekran<<kropka; ekran.seekp(0);
     Memo1->Lines->Add(ekran.str());
    }
    else
    {
      NumerPliku++;
    }
   fclose(wejscie);
  }
  // *********** Koniec sprawdzania identyfikatorów plików  *************

  LiczbaWczytPlikow=--NumerPliku;
  UpDown2->Max = NumerPliku;
  ZbiorTablic* Aktualna = ZbiorTablic::glowa;
  LiczbaZebranychSzerokosci=LiczbaWczytPlikow;
  while(Aktualna)
  {
    Aktualna->Ustaw();
    Aktualna=Aktualna->DajNast();
  }
  TablicaWart[0]=new double[LiczbaWczytPlikow];
  TablicaWart[1] = new double[LiczbaWczytPlikow];
  TablicaNrPlikow = new int[LiczbaWczytPlikow];
  Sortuj(TablicaNrPlikow,LiczbaWczytPlikow,'r');
  NumerPliku = TablicaNrPlikow[0];
  Memo1->Lines->Add("");
  Memo1->Lines->Add(" ***** KONIEC WCZYTYWANIA PLIKÓW **** ");
  Memo1->Lines->Add("");
}

void TForm1::WczytajDAT()
{
   String S1,S2;
  char drive[_MAX_DRIVE];
  char directory[_MAX_DIR];
  char path[_MAX_PATH];
   if(!Sprawdz()) return;    // sprawdz czy mozna dodac plik
	_splitpath(OpenDialog1->FileName.c_str(),drive,directory,0,0);
	_makepath(path,drive,directory,0,0);
	katalog=path;
	rejestr->zapiszStr("Katalog",path);


  FILE* wejscie;
  strstream ekran;
  // ************ Sprawdzanie identyfikatorów plików i dodawanie do kolekcji *************
  // LiczbaWczytPlikow zawiera pierwszy wolny indeks ListyPlikow
  int koniec = LiczbaWczytPlikow+Ost_Wczyt;
  NumerPliku = LiczbaWczytPlikow+1;
  Form1->ProgressBar1->Max=OpenDialog1->Files->Count-1;
  for(int AktNrPliku=0;AktNrPliku<OpenDialog1->Files->Count; AktNrPliku++)
  {
        Form1->ProgressBar1->Position=AktNrPliku;
        AnsiString T=ExtractFileName(OpenDialog1->Files->Strings[AktNrPliku]);
        Form1->LabelPliki->Caption= T;
	wejscie=fopen(OpenDialog1->Files->Strings[AktNrPliku].c_str(),"rb");
	// DodajDat jest zdefiniowane w  Funkcje.cpp deklaracja jako friend w klasie ZbiorTablic
	if (!DodajDat(wejscie,OpenDialog1->Files->Strings[AktNrPliku],NumerPliku))
	{
	 ekran<<"Plik "<<OpenDialog1->Files->Strings[AktNrPliku].c_str()<<" nie zosta³ dodany w procedurze DodajPlik";
	 ekran<<kropka; ekran.seekp(0);
	 Memo1->Lines->Add(ekran.str());
	}
	else
	{
	  NumerPliku++;
	}
   fclose(wejscie);
  }
  // *********** Koniec sprawdzania identyfikatorów plików  *************

  LiczbaWczytPlikow=--NumerPliku;
  UpDown2->Max = NumerPliku;
  ZbiorTablic* Aktualna = ZbiorTablic::glowa;
  LiczbaZebranychSzerokosci=LiczbaWczytPlikow;
  while(Aktualna)
  {
	Aktualna->Ustaw();
	Aktualna=Aktualna->DajNast();
  }
  TablicaWart[0]=new double[LiczbaWczytPlikow];
  TablicaWart[1] = new double[LiczbaWczytPlikow];
  TablicaNrPlikow = new int[LiczbaWczytPlikow];
  Sortuj(TablicaNrPlikow,LiczbaWczytPlikow,'r');
  NumerPliku = TablicaNrPlikow[0];
  Memo1->Lines->Add("");
  Memo1->Lines->Add(" ***** KONIEC WCZYTYWANIA PLIKÓW **** ");
  Memo1->Lines->Add("");
}



void TForm1::WczytajEPR()
{
   String S1,S2;
  char drive[_MAX_DRIVE];
  char directory[_MAX_DIR];
  char path[_MAX_PATH];
   if(!Sprawdz()) return;    // sprawdz czy mozna dodac plik
	_splitpath(OpenDialog1->FileName.c_str(),drive,directory,0,0);
	_makepath(path,drive,directory,0,0);
	katalog=path;
	rejestr->zapiszStr("Katalog",path);


  FILE* wejscie;
  strstream ekran;
  // ************ Sprawdzanie identyfikatorów plików i dodawanie do kolekcji *************
  // LiczbaWczytPlikow zawiera pierwszy wolny indeks ListyPlikow
  int koniec = LiczbaWczytPlikow+Ost_Wczyt;
  NumerPliku = LiczbaWczytPlikow+1;
  Form1->ProgressBar1->Max=OpenDialog1->Files->Count-1;
  for(int AktNrPliku=0;AktNrPliku<OpenDialog1->Files->Count; AktNrPliku++)
  {
        Form1->ProgressBar1->Position=AktNrPliku;
        AnsiString T=ExtractFileName(OpenDialog1->Files->Strings[AktNrPliku]);
        Form1->LabelPliki->Caption= T;
	wejscie=fopen(OpenDialog1->Files->Strings[AktNrPliku].c_str(),"rb");
	// DodajDat jest zdefiniowane w  Funkcje.cpp deklaracja jako friend w klasie ZbiorTablic
	if (!DodajEPR(wejscie,OpenDialog1->Files->Strings[AktNrPliku],NumerPliku))
	{
	 ekran<<"Plik "<<OpenDialog1->Files->Strings[AktNrPliku].c_str()<<" nie zosta³ dodany w procedurze DodajPlik";
	 ekran<<kropka; ekran.seekp(0);
	 Memo1->Lines->Add(ekran.str());
	}
	else
	{
	  NumerPliku++;
	}
   fclose(wejscie);
  }
  // *********** Koniec sprawdzania identyfikatorów plików  *************

  LiczbaWczytPlikow=--NumerPliku;
  UpDown2->Max = NumerPliku;
  ZbiorTablic* Aktualna = ZbiorTablic::glowa;
  LiczbaZebranychSzerokosci=LiczbaWczytPlikow;
  while(Aktualna)
  {
	Aktualna->Ustaw();
	Aktualna=Aktualna->DajNast();
  }
  TablicaWart[0]=new double[LiczbaWczytPlikow];
  TablicaWart[1] = new double[LiczbaWczytPlikow];
  TablicaNrPlikow = new int[LiczbaWczytPlikow];
  Sortuj(TablicaNrPlikow,LiczbaWczytPlikow,'r');
  NumerPliku = TablicaNrPlikow[0];
  Memo1->Lines->Add("");
  Memo1->Lines->Add(" ***** KONIEC WCZYTYWANIA PLIKÓW **** ");
  Memo1->Lines->Add("");
}
