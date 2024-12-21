//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ZapisOdczytSesji.h"
#include "metamorfozis_w5.h"
#include "WykresPunktowy.h"
#include <string.h>
#include "Rejestr.h"
#include "Punkty.h"
//---------------------------------------------------------------------------

extern ObszarPunkty::TListaPunktow ZebranePunkty;
extern ObszarPunkty::TListaPrzejsc ListaPrzejsc;
//extern Symbol* ObszarPunkty::Punkt::Ksztalt;       // "statyczny" kszta³t zebranych punków.



#pragma package(smart_init)
#define ChwiloweWylaczenieZKompilacji
#ifndef ChwiloweWylaczenieZKompilacji

Your installation program can provide support for context menu operations, such as Open, Print, and Print To, by setting appropriate registry entries. The context menu appears when the user clicks mouse button 2 on a document associated with your application.

Enabling Print in the registry gives the shell instructions about what to execute when the user chooses Print from the context menu. Usually an application will display a dialog box that says " Printing page n of N on LPTX."
Enabling Print To in the registry specifies the default action for "drag print." Print To displays the same dialog box as Print when you drag it to a specific printer. The Print To option is not displayed on the context menu, so it does not bring up anything (that is, it cannot be chosen).

The following example shows how to set commands for the context menu for files having the .WRI filename extension.

HKEY_CLASSES_ROOT\.wri = wrifile
HKEY_CLASSES_ROOT\wrifile = Write Document
HKEY_CLASSES_ROOT\wrifile\DefaultIcon =
    C:\Progra~1\Access~1\WORDPAD.EXE,2
HKEY_CLASSES_ROOT\wrifile\shell\open\command = WORDPAD.EXE %1
HKEY_CLASSES_ROOT\wrifile\shell\print\command =
    C:\Progra~1\Access~1\WORDPAD.EXE /p "%1"
HKEY_CLASSES_ROOT\wrifile\shell\printto\command =
    C:\Progra~1\Access~1\WORDPAD.EXE /pt "%1" "%2" "%3" "%4"


In the preceding commands, the %1 parameter is the filename, %2 is the printer name, %3 is the driver name, and %4 is the port name. In Windows 95, you can ignore the %3 and %4 parameters (the printer name is unique in Windows 95).

#endif

//TSaveDialog *ZapSesji=Form1->SaveSesji;

void ZapisSesji(void)
{AnsiString nazwa;
 char drive[_MAX_DRIVE];
 char directory[_MAX_DIR];
 char path[_MAX_PATH];

 if(Form1->SaveSesji->Execute())
 {
     _splitpath(Form1->SaveSesji->FileName.c_str(),drive,directory,0,0);
     _makepath(path,drive,directory,0,0);
     katalog=path;
     rejestr->zapiszStr("KatalogSesja",path);
     ofstream fout(Form1->SaveSesji->FileName.c_str());
     fout<<"SavMet ver. 1"<<endl;
     fout<<LiczbaWczytPlikow<<endl;
     int NrPliku;
     ZbiorTablic* Akt;
     Punkty* tym;
     for(int i=0; i<LiczbaWczytPlikow;i++)
     {
       NrPliku=TablicaNrPlikow[i];
       Akt=DajZb(NrPliku);
       nazwa= Akt->NazwaPlikuGlobal(NrPliku);
       fout<<nazwa.c_str();
       fout<<" * "<<NrPliku<<endl;
     }
     for(int i=0; i<LiczbaWczytPlikow;i++)
     {
       NrPliku=TablicaNrPlikow[i];
       Akt=DajZb(NrPliku);
       if((tym=Akt->Pola[Akt->DajIndeks(NrPliku)].nastepny)!=0) //Ustawia Pierwsz¹ Wartosc
       {

         fout<<NrPliku<<"\t";
          while(tym)               // Iteracja po nastepnych watosciach
           {
            fout<<tym->indeks<< "\t";
            tym=tym->nastepny ;
           }
           fout<<" *"<<endl;
       }
     }
     if(WykresPktForm->Polozenia->NumerPrzejscia==0) return ; //je¿eli nie zaznaczono poziomów energetycznych
     fout<<"[Poziomy]"<<endl;
     std::list<int>::iterator iterWew;
     std::list<ListSklad>::iterator iter;
     iter=WykresPktForm->Polozenia->Przejscia.begin();
     double theta,phi,frq;
     theta=WykresPktForm->ETheta->Text.ToDouble();
     phi=WykresPktForm->EPhi->Text.ToDouble();
     frq=WykresPktForm->Efrq->Text.ToDouble();
     fout<<theta<<" "<<phi<<"  "<<frq<<" "<<WykresPktForm->StalyKat->ItemIndex<<endl;
     while(iter!=WykresPktForm->Polozenia->Przejscia.end())
     {
         iterWew=iter->punkty.begin();
         if(iter->numer==0)
         {
              iter++;
              continue;
         }
         fout<<iter->Przejscie1<<" "
             <<iter->Przejscie2<<" "
             <<int(iter->Kolor)<<" "
             <<iter->numer<<" "<<endl;
         while(iterWew!=iter->punkty.end())
         {
            fout<< *iterWew <<" " ;
            iterWew++;
         }
         fout<<"*"<<endl;
         iter++;
      }
      rejestr->zapiszStr("KatalogSesja",path);
 }    // ******* Koniec Execute
}



void ZapisSesji(int Wersja)
{ AnsiString nazwa;
  char drive[_MAX_DRIVE];
  char directory[_MAX_DIR];
  char path[_MAX_PATH];
  Form1->SaveSesji->FilterIndex = Wersja;
  if(Form1->SaveSesji->Execute())
  {
   _splitpath(Form1->SaveSesji->FileName.c_str(),drive,directory,0,0);
   _makepath(path,drive,directory,0,0);
   katalog=path;
   rejestr->zapiszStr("KatalogSesja",path);
   ofstream fout(Form1->SaveSesji->FileName.c_str());
   fout<<"SavMet ver. "<< Form1->SaveSesji->FilterIndex <<endl;  // Wybrany indeks jest jednoczesnie numerem wersji pliku
   fout<<LiczbaWczytPlikow<<endl;
   int NrPliku;
   ZbiorTablic* Akt;
   Punkty* tym;
   for(int i=0; i<LiczbaWczytPlikow;i++)
   {
     NrPliku=TablicaNrPlikow[i];
     Akt=DajZb(NrPliku);
     nazwa= Akt->NazwaPlikuGlobal(NrPliku);
     fout<<nazwa.c_str();
     fout<<" * "<<NrPliku<<endl;
   }
   switch(Form1->SaveSesji->FilterIndex)
   {
    case 1:  {
       for(int i=0; i<LiczbaWczytPlikow;i++)
       {
         NrPliku=TablicaNrPlikow[i];
         Akt=DajZb(NrPliku);
         if((tym=Akt->Pola[Akt->DajIndeks(NrPliku)].nastepny)!=0) //Ustawia Pierwsz¹ Wartosc
         {
            fout<<NrPliku<<"\t";
            while(tym)               // Iteracja po nastepnych watosciach
             {
              fout<<tym->indeks<< "\t";
              tym=tym->nastepny ;
             }
             fout<<" *"<<endl;
         }
       }
       if(WykresPktForm->Polozenia->NumerPrzejscia==0) return ; //je¿eli nie zaznaczono poziomów energetycznych
       fout<<"[Poziomy]"<<endl;
       std::list<int>::iterator iterWew;
       std::list<ListSklad>::iterator iter;
       iter=WykresPktForm->Polozenia->Przejscia.begin();
       double theta,phi,frq;
       theta=WykresPktForm->ETheta->Text.ToDouble();
       phi=WykresPktForm->EPhi->Text.ToDouble();
       frq=WykresPktForm->Efrq->Text.ToDouble();
       fout<<theta<<" "<<phi<<"  "<<frq<<" "<<WykresPktForm->StalyKat->ItemIndex<<endl;
       while(iter!=WykresPktForm->Polozenia->Przejscia.end())
       {
           iterWew=iter->punkty.begin();
           if(iter->numer==0)
           {
                iter++;
                continue;
           }
           fout<<iter->Przejscie1<<" "
               <<iter->Przejscie2<<" "
               <<int(iter->Kolor)<<" "
               <<iter->numer<<" "<<endl;
           while(iterWew!=iter->punkty.end())
           {
              fout<< *iterWew <<" " ;
              iterWew++;
           }
           fout<<"*"<<endl;
           iter++;
       }
       rejestr->zapiszStr("KatalogSesja",path);
       }
       break;
    case 2:
       ZebranePunkty.Wypisz(fout);
       if(ListaPrzejsc.DajLiczbeZdefiniowanychPrzejsc())
       {
         ListaPrzejsc.Wypisz(fout);
         WykresPktForm->Polozenia->ZapisParametrowWykresu(fout);
       }
       rejestr->zapiszStr("KatalogSesja",path);
       break;
   }
  }    // ******* Koniec Execute

}

void OdczytPlikow(fstream& fin)
{



}

void Ver0(char* NazwaPliku)
//void Ver0(ifstream& fin)
{
      char buf[200], fake;
      int LiczbaPlikow, numerP;
      ifstream fin(NazwaPliku);
      fin>>buf>>buf>>buf;      // ponowny odczyt pierwszej linijki
      FILE* wejscie;
      strstream ekran;// ShowMessage("Aktualny Numer wersji Pliku to  " + AnsiString(NumerVersji));
      fin>>LiczbaPlikow;
      char drive[_MAX_DRIVE];
      char directory[_MAX_DIR];
      char path[_MAX_PATH];
      char nazwaP[_MAX_FNAME];
      char rozszerzenie[_MAX_EXT];
        // Tu dodac obsluge innego katalogu.
        Form1->ProgressBar1->Max=LiczbaPlikow-1;
     for(int i =0;i<LiczbaPlikow;i++)
     {
          Form1->ProgressBar1->Position=i;
          fin>>fake;  // koniec linii ?
          fin.putback(fake);   // podczytuje koniec linii
          fin.get(buf,200,'*');
          fin>>fake;   // gwiazdka
          fin>>numerP;
          wejscie=fopen(buf,"rb");
          if(!wejscie)
          {
                Application->MessageBox("Nie powiod³o siê otworzenie pliku"
                                        " zapisanego w sesji/n/nZmieñ œcie¿ki"
                                        " dostêpu w pliku sesji lub umieœæ "
                                        " pliki w katalogu tam okreœlonym ",
                                        " UWAGA!!!",MB_OK);
                return;
          }
          _splitpath(buf,drive,directory,nazwaP,rozszerzenie);
          AnsiString T=nazwaP;
          T+=rozszerzenie;
          Form1->LabelPliki->Caption= T;
          bool wynik;
          AnsiString NazwaPlikuTemp = buf;
          if(!strncmp(rozszerzenie,"dat",4))
          {
            wynik=DodajDat(wejscie,NazwaPlikuTemp,numerP);
          }
          else
          {
               wynik= DodajPlik(wejscie,NazwaPlikuTemp,numerP);
          }
          if (!wynik)
            {
             ekran<<"Plik "<<buf<<" nie zosta³ dodany w procedurze DodajPlik";
             ekran<<endl<<" Mo¿e to powodowaæ niestabilne dzia³anie programu";
             ekran<<endl<<endl<<"Czy chcesz przerwaæ wczytywanie ";
             ekran<<kropka; ekran.seekp(0);
             if(Application->MessageBox(ekran.str(),"UWAGA",MB_YESNO)==IDYES)
                return;
             //ShowMessage(ekran.str());
            }
           fclose(wejscie);
        }
          NumerPliku=numerP;
          LiczbaWczytPlikow=LiczbaPlikow;
          Form1->UpDown2->Max = LiczbaPlikow;
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
          ZbiorTablic* Akt=DajZb(NumerPliku);


          int Indeks;
           // DajIndeks(NumerPliku) daje numer lokalny
          // Ca³e wyra¿enie dodaje odpowiedni indeks tablicy do zbioru punktów
           fin>>numerP;
           fake=0;
           while(!fin.eof())
           {
             fin>>Indeks>>fake;
             Akt=DajZb(numerP);
             if(fake!='*')
             {
                 fin.putback(fake);
                 if (Akt->Pola[Akt->DajIndeks(numerP)].Dodaj(Indeks))
                         LiczbaZebranychPolozen++;
             }
             else
             {
                  fake=0;
                  if (Akt->Pola[Akt->DajIndeks(numerP)].Dodaj(Indeks))
                         LiczbaZebranychPolozen++;
                  fin>>numerP;
             }
          }




}

void OdczytPoziomow(ifstream& fin)
{
       WykresPktForm->Polozenia->Update(LiczbaZebranychPolozen);
       std::list<int>::iterator iterWew;
       std::list<ListSklad>::iterator iter;
       ListSklad TymList;
       int fake;
       char fakeChar;
       iter=WykresPktForm->Polozenia->Przejscia.begin();
       double theta,phi,frq;
       fin>>theta>>phi>>frq>>fake;
       WykresPktForm->StalyKat->ItemIndex=fake;
       WykresPktForm->ETheta->Text=theta;
       WykresPktForm->EPhi->Text=phi;
       WykresPktForm->Efrq->Text=frq;

      // ------------- Nowe ver 2 -------------------------
       TPara WspPkt;
       int NrPliku;
       ObszarPunkty::Przejscie *Prz;
       ObszarPunkty::Punkt *TP;
      // ------------- Koniec ver 2 -----------------------


       while(!fin.eof())
       {
         if(!(fin>>TymList.Przejscie1)) break;
          fin>>TymList.Przejscie2;
          fin>>fake;
          fin>>TymList.numer;
          TymList.Kolor=static_cast<TColor>(fake);

        // ----------- Nowe ver 2 ----------------------------
          Prz = new ObszarPunkty::Przejscie(TymList.Przejscie1, TymList.Przejscie2);
          Prz->UstawNumer(TymList.numer);
          Prz->UstawKolor(TymList.Kolor);
          ListaPrzejsc.DodajPrzejscie(Prz);
        // ------------- Koniec ver 2 -------------------------------

          WykresPktForm->Polozenia->Przejscia.insert(WykresPktForm->Polozenia->Przejscia.begin(),TymList);    //wstaw now¹ listê na pocz¹tek
          WykresPktForm->Polozenia->AktualnaLista= WykresPktForm->Polozenia->Przejscia.begin();               // ustaw iterator równie¿ na pocz¹tek
          WykresPktForm->Polozenia->NumerPrzejscia=TymList.numer;
          while(fin>>fake)
          {
                   WykresPktForm->Polozenia->AktualnaLista->punkty.insert(WykresPktForm->Polozenia->AktualnaLista->punkty.begin(),fake);

             // Nowe -------------------------- ver 2 --------------------------
                   NrPliku=WykresPktForm->Polozenia->DajNrPliku(fake);
                   WspPkt=WykresPktForm->Polozenia->DajDanePunktu(fake);
                   TP=ZebranePunkty.ZnajdzPunkt(WspPkt.X,WspPkt.Y,NrPliku);
                   TP->DodajPrzejscie(TymList.numer);   // DodajPrzejscie;
             // ------------------------  Koniec Ver 2 -------------------------
          }
          fin.clear();
          fin>>fakeChar; // sczytanie '*'
          WykresPktForm->Polozenia->AktualnaLista->punkty.sort();
       }
       WykresPktForm->Edit1->Text=TymList.Przejscie1;
       WykresPktForm->Edit2->Text=TymList.Przejscie2;
       WykresPktForm->Edit1->Enabled=false;
       WykresPktForm->Edit2->Enabled=false;

}

void Ver1(char* NazwaPliku)
//void Ver1(ifstream& fin)
{
    char buf[_MAX_PATH], fake;
    bool poprawSciezke = false;
    bool poprawSciezkeRecznie = false;
    int LiczbaPlikow, numerP;
    ifstream fin(NazwaPliku);
    fin>>buf>>buf>>buf;      // ponowny odczyt pierwszej linijki
    FILE* wejscie;
    strstream ekran;// ShowMessage("Aktualny Numer wersji Pliku to  " + AnsiString(NumerVersji));
    fin>>LiczbaPlikow;
    char drive[_MAX_DRIVE];
    char directory[_MAX_DIR];
    char path[_MAX_PATH];
    char nazwaP[_MAX_FNAME];
    char rozszerzenie[_MAX_EXT];
    _splitpath(NazwaPliku,drive,directory,nazwaP,rozszerzenie);
    // Tu dodac obsluge innego katalogu.
    Form1->ProgressBar1->Max=LiczbaPlikow;
    for(int i =0;i<LiczbaPlikow;i++)
    {
      Form1->ProgressBar1->Position=i;
      fin>>fake;  // koniec linii ?
      fin.putback(fake);   // podczytuje koniec linii
      fin.get(buf,_MAX_PATH,'*');
      fin>>fake;   // gwiazdka
      fin>>numerP;
      if(FileExists(buf) && !poprawSciezke && !poprawSciezkeRecznie )
              wejscie=fopen(buf,"rb");
      else if(!poprawSciezkeRecznie)
      {
             _splitpath(buf,0,0,nazwaP,rozszerzenie);
             _makepath(path,drive,directory,nazwaP,rozszerzenie);
             if(FileExists(path))
             {
                     poprawSciezke = true;
                     strcpy(buf,path);
                     wejscie = fopen(buf,"rb");
             }
             else
             {

                    AnsiString tym;
                    ShowMessage("Nie odnaleziono zbiorów projektu, spróbuj poszukaæ rêcznie");
                    tym= nazwaP;
                    tym+=rozszerzenie;
                    Form1->OpenDialog1->FileName = tym;
                    if(Form1->OpenDialog1->Execute())
                    {
                       poprawSciezkeRecznie = true;
                       _splitpath(Form1->OpenDialog1->FileName.c_str(),drive,directory,nazwaP,rozszerzenie);
                       _makepath(path,drive,directory,nazwaP,rozszerzenie);
                       strcpy(buf,path);
                       wejscie = fopen(buf,"rb");
                    }
                    else
                    {
                            ShowMessage("Nie uda³o siê ustaliæ po³o¿enia plików projektu, koñczê proceurê");
                            return;
                    }

             }
      }  // ************ Koniec end if  PoprawSciezke    ********************
      else    // Wczytanie pliku
      {
              _splitpath(buf,0,0,nazwaP,rozszerzenie);
              _makepath(buf,drive,directory,nazwaP,rozszerzenie);
              if(FileExists(buf))
                    wejscie = fopen(buf,"rb");
              else                             //je¿eli s¹ jeszcze pliki w innych katalogach
              {                                // ktore trzeba wskazac rêcznie.
                    AnsiString tym;
                    ShowMessage("Nie odnaleziono zbiorów projektu, spróbuj poszukaæ rêcznie");
                    tym= nazwaP;
                    tym+=rozszerzenie;
                    Form1->OpenDialog1->FileName = tym;
                    if(Form1->OpenDialog1->Execute())
                    {
                       poprawSciezkeRecznie = true;
                       _splitpath(Form1->OpenDialog1->FileName.c_str(),drive,directory,nazwaP,rozszerzenie);
                       _makepath(path,drive,directory,nazwaP,rozszerzenie);
                       strcpy(buf,path);
                       wejscie = fopen(buf,"rb");
                    }
              }
      }
      if(!wejscie)
      {
                    Application->MessageBox("Nie powiod³o siê otworzenie pliku"
                                                                    " zapisanego w sesji/n/nZmieñ œcie¿ki"
                                                                    " dostêpu w pliku sesji lub umieœæ "
                                                                    " pliki w katalogu tam okreœlonym ",
                                                                    " UWAGA!!!",MB_OK);

                      return;

      }
      _splitpath(buf,drive,directory,nazwaP,rozszerzenie);    // Dodawanie poszczególnych plików
      AnsiString T=nazwaP;
      T+=rozszerzenie;
      Form1->LabelPliki->Caption= T;
      strlwr(rozszerzenie);
      bool wynik;
      int wynikPor=strncmp(rozszerzenie,".dat",4);
      AnsiString NazwaPlikuTemp = buf;
      if(!wynikPor)
      {
            wynik=DodajDat(wejscie,NazwaPlikuTemp,numerP);
      }
      else 	  if(!strncmp(rozszerzenie,".epr",4))
      {
              wynik=DodajEPR(wejscie,NazwaPlikuTemp,numerP);
      }
      else  if(!strncmp(rozszerzenie,".mla",4))
      {
               wynik= DodajPlik(wejscie,NazwaPlikuTemp,numerP);
      }
      else
      {
            wynik =DodajDat(wejscie,NazwaPlikuTemp,numerP);
      }
      if (!wynik)
            {
             ekran<<"Plik "<<buf<<" nie zosta³ dodany w procedurze DodajPlik";
             ekran<<endl<<" Mo¿e to powodowaæ niestabilne dzia³anie programu";
             ekran<<endl<<endl<<"Czy chcesz przerwaæ wczytywanie ";
             ekran<<kropka; ekran.seekp(0);
             if(Application->MessageBox(ekran.str(),"UWAGA",MB_YESNO)==IDYES)
                    return;
            }
       fclose(wejscie);
    } // ********************  Koniec pêtli wczytuj¹cej pliki *****
    NumerPliku=numerP;
    LiczbaWczytPlikow=LiczbaPlikow;
    Form1->UpDown2->Max = LiczbaPlikow;
    ZbiorTablic* Aktualna = ZbiorTablic::glowa;
    LiczbaZebranychSzerokosci=LiczbaWczytPlikow;
    while(Aktualna)
    {
          Aktualna->Ustaw();
          Aktualna=Aktualna->DajNast();
    }

// TablicaWart zawiera numery plikow i stowarzyszone z nimi wartosci
// otrzymane z odczytania liczb zapisanych w nazwach zbiorow
// TablicaNrPlikow zawiera  posortowane wg. wartosci stowarzyszonych
// numery plikow - funkcja sort() zdefiniowana w Funkcjach :)

    TablicaWart[0]=new double[LiczbaWczytPlikow];
    TablicaWart[1] = new double[LiczbaWczytPlikow];
    TablicaNrPlikow = new int[LiczbaWczytPlikow];

 // Funkcja Sortuj - przepisuje ze ZbiorPlikow wartosci do tabilc i
 // je sortuje.

    Sortuj(TablicaNrPlikow,LiczbaWczytPlikow,'r');
    NumerPliku = TablicaNrPlikow[0];
    ZbiorTablic* Akt=DajZb(NumerPliku);

  // Odczyt wartosci pol rezonanowych jako ideksow tablicy zawierajacych
  // wartosci pol magnetycznych.
  // Ca³e wyra¿enie dodaje odpowiedni indeks tablicy do zbioru punktów

    int Indeks;
    fin>>numerP;
    fake=0;
    while(!fin.eof())
    {
           fin>>Indeks>>fake;
           Akt=DajZb(numerP);
           if(fake!='*')
           {
                   fin.putback(fake);
                   ObszarPunkty::Punkt P(numerP,Indeks,Akt->DajWartosc(numerP),DajX(Indeks,numerP));
                   if(ZebranePunkty.DodajPunkt(P))
                   {
                     if (Akt->Pola[Akt->DajIndeks(numerP)].Dodaj(Indeks))
                                   LiczbaZebranychPolozen++;
                   }
           }
           else  // Je¿eli napotkano *
           {
                    fin>>fake;
                    fin.putback(fake);
                    if(fake=='[')
                    {
                            fin>>buf;
                            if(!strcmp(buf,"[Poziomy]"))
                                  OdczytPoziomow(fin);
                           // ShowMessage(buf);
                            break;
                    }
                    fake=0;
                    ObszarPunkty::Punkt P(numerP,Indeks,Akt->DajWartosc(numerP),DajX(Indeks,numerP));
                    if(ZebranePunkty.DodajPunkt(P))
                    {
                       if (Akt->Pola[Akt->DajIndeks(numerP)].Dodaj(Indeks))
                                   LiczbaZebranychPolozen++;
                    }
                    fin>>numerP;
           }
    } // *******************  Koniec while eof *********************
}




// ----------------------    Koniec funkcji Ver1  -----------------------------


void Ver2(std::ifstream& fin,char* NazwaPliku)
//void Ver1(ifstream& fin)
{
    char buf[_MAX_PATH], fake;
    bool poprawSciezke = false;
    bool poprawSciezkeRecznie = false;
    int LiczbaPlikow, numerP;
    FILE* wejscie;
  //  fin>>buf>>buf>>buf;      // ponowny odczyt pierwszej linijki
    strstream ekran;// ShowMessage("Aktualny Numer wersji Pliku to  " + AnsiString(NumerVersji));
    fin>>LiczbaPlikow;
    char drive[_MAX_DRIVE];
    char directory[_MAX_DIR];
    char path[_MAX_PATH];
    char nazwaP[_MAX_FNAME];
    char rozszerzenie[_MAX_EXT];
    _splitpath(NazwaPliku,drive,directory,nazwaP,rozszerzenie);
    // Tu dodac obsluge innego katalogu.
    Form1->ProgressBar1->Max=LiczbaPlikow-1;
    for(int i =0;i<LiczbaPlikow;i++)
    {
      Form1->ProgressBar1->Position=i;
      fin>>fake;  // koniec linii ?
      fin.putback(fake);   // podczytuje koniec linii
      fin.get(buf,_MAX_PATH,'*');
      fin>>fake;   // gwiazdka
      fin>>numerP;
      if(FileExists(buf) && !poprawSciezke && !poprawSciezkeRecznie )
              wejscie=fopen(buf,"rb");
      else if(!poprawSciezkeRecznie)
      {
             _splitpath(buf,0,0,nazwaP,rozszerzenie);
             _makepath(path,drive,directory,nazwaP,rozszerzenie);
             if(FileExists(path))
             {
                     poprawSciezke = true;
                     strcpy(buf,path);
                     wejscie = fopen(buf,"rb");
             }
             else
             {

                    AnsiString tym;
                    ShowMessage("Nie odnaleziono zbiorów projektu, spróbuj poszukaæ rêcznie");
                    tym= nazwaP;
                    tym+=rozszerzenie;
                    Form1->OpenDialog1->FileName = tym;
                    if(Form1->OpenDialog1->Execute())
                    {
                       poprawSciezkeRecznie = true;
                       _splitpath(Form1->OpenDialog1->FileName.c_str(),drive,directory,nazwaP,rozszerzenie);
                       _makepath(path,drive,directory,nazwaP,rozszerzenie);
                       strcpy(buf,path);
                       wejscie = fopen(buf,"rb");
                    }
                    else
                    {
                            ShowMessage("Nie uda³o siê ustaliæ po³o¿enia plików projektu, koñczê proceurê");
                            return;
                    }

             }
      }  // ************ Koniec end if  PoprawSciezke    ********************
      else    // Wczytanie pliku
      {
              _splitpath(buf,0,0,nazwaP,rozszerzenie);
              _makepath(buf,drive,directory,nazwaP,rozszerzenie);
              if(FileExists(buf))
                    wejscie = fopen(buf,"rb");
              else                             //je¿eli s¹ jeszcze pliki w innych katalogach
              {                                // ktore trzeba wskazac rêcznie.
                    AnsiString tym;
                    ShowMessage("Nie odnaleziono zbiorów projektu, spróbuj poszukaæ rêcznie");
                    tym= nazwaP;
                    tym+=rozszerzenie;
                    Form1->OpenDialog1->FileName = tym;
                    if(Form1->OpenDialog1->Execute())
                    {
                       poprawSciezkeRecznie = true;
                       _splitpath(Form1->OpenDialog1->FileName.c_str(),drive,directory,nazwaP,rozszerzenie);
                       _makepath(path,drive,directory,nazwaP,rozszerzenie);
                       strcpy(buf,path);
                       wejscie = fopen(buf,"rb");
                    }
              }
      }
      if(!wejscie)
      {
                    Application->MessageBox("Nie powiod³o siê otworzenie pliku"
                                                                    " zapisanego w sesji/n/nZmieñ œcie¿ki"
                                                                    " dostêpu w pliku sesji lub umieœæ "
                                                                    " pliki w katalogu tam okreœlonym ",
                                                                    " UWAGA!!!",MB_OK);

                      return;

      }
      _splitpath(buf,drive,directory,nazwaP,rozszerzenie);    // Dodawanie poszczególnych plików
      AnsiString T=nazwaP;
      T+=rozszerzenie;
      Form1->LabelPliki->Caption= T ;
      strlwr(rozszerzenie);
      bool wynik;
      int wynikPor=strncmp(rozszerzenie,".dat",4);
      AnsiString NazwaPlikuTemp = buf;
      if(!wynikPor)
      {
            wynik=DodajDat(wejscie,NazwaPlikuTemp,numerP);
      }
      else 	  if(!strncmp(rozszerzenie,".epr",4))
      {
              wynik=DodajEPR(wejscie,NazwaPlikuTemp,numerP);
      }
      else  if(!strncmp(rozszerzenie,".mla",4))
      {
               wynik= DodajPlik(wejscie,NazwaPlikuTemp,numerP);
      }
      else
      {
            wynik =DodajDat(wejscie,NazwaPlikuTemp,numerP);
      }
      if (!wynik)
            {
             ekran<<"Plik "<<buf<<" nie zosta³ dodany w procedurze DodajPlik";
             ekran<<endl<<" Mo¿e to powodowaæ niestabilne dzia³anie programu";
             ekran<<endl<<endl<<"Czy chcesz przerwaæ wczytywanie ";
             ekran<<kropka; ekran.seekp(0);
             if(Application->MessageBox(ekran.str(),"UWAGA",MB_YESNO)==IDYES)
                    return;
            }
       fclose(wejscie);
    } // ********************  Koniec pêtli wczytuj¹cej pliki *****
    NumerPliku=numerP;
    LiczbaWczytPlikow=LiczbaPlikow;
    Form1->UpDown2->Max = LiczbaPlikow;
    ZbiorTablic* Aktualna = ZbiorTablic::glowa;
    LiczbaZebranychSzerokosci=LiczbaWczytPlikow;
    while(Aktualna)
    {
          Aktualna->Ustaw();
          Aktualna=Aktualna->DajNast();
    }

// TablicaWart zawiera numery plikow i stowarzyszone z nimi wartosci
// otrzymane z odczytania liczb zapisanych w nazwach zbiorow
// TablicaNrPlikow zawiera  posortowane wg. wartosci stowarzyszonych
// numery plikow - funkcja sort() zdefiniowana w Funkcjach :)

    TablicaWart[0]=new double[LiczbaWczytPlikow];
    TablicaWart[1] = new double[LiczbaWczytPlikow];
    TablicaNrPlikow = new int[LiczbaWczytPlikow];

 // Funkcja Sortuj - przepisuje ze ZbiorPlikow wartosci do tabilc i
 // je sortuje.

    Sortuj(TablicaNrPlikow,LiczbaWczytPlikow,'r');
    NumerPliku = TablicaNrPlikow[0];


  // Odczyt wartosci pol rezonanowych jako ideksow tablicy zawierajacych
  // wartosci pol magnetycznych.
  // Ca³e wyra¿enie dodaje odpowiedni indeks tablicy do zbioru punktów

    int Indeks;
   // fin>>numerP;    // liczba zebranych po³o¿eñ
    fake=0;

    LiczbaZebranychPolozen=ZebranePunkty.Wczytaj(fin);
    ListaPrzejsc.Wczytaj(fin);
    WykresPktForm->Polozenia->OdczytParametrowWykresu(fin);

  /*
    ZbiorTablic* Akt=DajZb(NumerPliku);
    while(!fin.eof())
    {
           fin>>Indeks>>fake;
           Akt=DajZb(numerP);
           if(fake!='*')
           {
                   fin.putback(fake);
                   if (Akt->Pola[Akt->DajIndeks(numerP)].Dodaj(Indeks))
                                   LiczbaZebranychPolozen++;
           }
           else  // Je¿eli napotkano *
           {
                    fin>>fake;
                    fin.putback(fake);
                    if(fake=='[')
                    {
                            fin>>buf;
                            if(!strcmp(buf,"[Poziomy]"))
                                  OdczytPoziomow(fin);
                           // ShowMessage(buf);
                            break;
                    }
                    fake=0;
                    if (Akt->Pola[Akt->DajIndeks(numerP)].Dodaj(Indeks))
                                   LiczbaZebranychPolozen++;
                    fin>>numerP;
           }
    } // *******************  Koniec while eof *********************
   */
}









// ---------------------- Koniec funkcji Ver2 --------------------------------

void OtwarcieZapisanejSesji(char* NazwaPliku)
{
  char buf[200], fake;
  int NumerVersji;
  int LiczbaPlikow, numerP;
//  char drive[_MAX_DRIVE];
//  char directory[_MAX_DIR];
//  char path[_MAX_PATH];
//  _splitpath(NazwaPliku,drive,directory,0,0);
//  _makepath(path,drive,directory,0,0);
//  katalog=path;
  //rejestr->WriteString("KatalogSesji",path);
 // katalog=rejestr->odczytStr("KatalogSesji",path);
  ifstream fin(NazwaPliku);
  fin>>buf;
  if(strcmp(buf,"SavMet"))
  {
        ShowMessage("Nie rozpoznany format pliku sesji");
        return;
  }
  fin>>buf>>NumerVersji;

  Form1->PrzycWstPunkt->Down=true;   // Ustawienie trybu wstawiania punktow
  Form1->PrzycWstPunkt->Click();
 // Form1->PrzycWstPunktClick(Form1);
  //Form1->PrzycWstPunkt->Down=true;
  Form1->Memo1->Hide();
  Form1->Image1->Visible=true;
  rysunek = true;
  Form1->PanelProgressBar->Visible=true;
  Form1->PanelProgressBar->BringToFront();
  switch(NumerVersji)
  {
    case 0:     fin.close();
                Ver0(NazwaPliku);// (fin);
            break;
    case 1:     fin.close();
                Ver1(NazwaPliku);//(fin);
            break;
    case 2:     Ver2(fin, NazwaPliku);
            break;
  }
  Form1->PanelProgressBar->Visible=false;
  Form1->wczytano=true;
  rysunek=true;
}


