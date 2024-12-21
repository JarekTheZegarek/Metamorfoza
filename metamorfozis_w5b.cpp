//---------------------------------------------------------------------------
#include <vcl.h>
#include "Unit2.h"
#pragma hdrstop
#include "metamorfozis_w5b.h"
#include "OkienkoSave.h"
#include <sys\stat.h>
#include "StrukturaOpcje.h"
#include "RamkaOpcje.h"
#include <errno.h>
#include "WykresPunktowy.h"
#include "Wykres.h"
#include "UnitPrzelicznik.h"
#include "Prezentacja.h"
#include "Unit3.h"
#include "Rejestr.h"
#include "Punkty.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "Unit1"
#pragma resource "*.dfm"
TForm1 *Form1;
ZbiorTablic* ZbiorTablic::glowa = NULL;
int ZbiorTablic::MaxRozmiar=0;
double ZbiorTablic::GlobalMaxX=MINDOUBLE;
double ZbiorTablic::GlobalMaxY=MINDOUBLE;
double ZbiorTablic::GlobalMinX=MAXDOUBLE;
double ZbiorTablic::GlobalMinY=MAXDOUBLE;
Punkty *Pola;
ZbiorTablic Fantom;
extern TListaPunktow ZebranePunkty;





//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
// HintWindowClass=  __classid(TFontyDlaHinta);
//Screen->HintFont->Name="Arial";
 //Application->HintColor=clAqua;
// Screen->HintFont->Charset=  EASTEUROPE_CHARSET	;
// Screen->HintFont->Size= 12;
 ZnakKolumny[0]= ',';
 ZnakKolumny[1]=kropka;               // koniec ³añcucha '\0'
 Doswiadczalny=new Kolo();
 Zaznaczenie=new Kwadrat();
 Image1->Top=0;
 Image1->Left=0;
 AktWykrLinii=0;
 Tryb = Nieokreslony;
 PowiekszX=0;
 LiczbaZebranychPolozen=0;
 LiczbaZebranychSzerokosci=0;
 Opcje.CzytajRejestr();
 ListBox1=0;
 Panelek=0;

 if(ParamCount()==1)
  {
         if(!ListaPlikow)
            ListaPlikow = new TStringList;     // Dodaj do ListyPlikow
        else
        {
           ShowMessage("Chwilowo odczyt sesji jest mo¿liwy jedynie wtedy gdy\n"
                        "¿adne pliki nie s¹ wczytane" );
           Application->Terminate();
        }
         OtwarcieZapisanejSesji(ParamStr(1).c_str());
  }

}
//---------------------------------------------------------------------------



void __fastcall TForm1::Wczytaj1Click(TObject *Sender)
{

  OpenDialog1->Options.Clear();
  OpenDialog1->InitialDir=katalog;
  OpenDialog1->Options << ofAllowMultiSelect << ofFileMustExist;
  OpenDialog1->Filter = "Pliki EPR Lab (*.mla)|*.mla|Pliki tekstowe (*.dat) |*.dat|Wszystkie (*.*)|*.*|Pliki EPR |*.epr"
                        "|Pliki ze spektrometru (mla, epr)|*.mla;*.epr";
  OpenDialog1->FilterIndex = 5; // start the dialog showing zbiory *.epr
			   // liczba wczytanych w tej sesji plików
 // int LiczWszystPlikow;             // liczba wszyskich wczytanych plików
 // LiczWszystPlikow = Ost_Wczyt + OpenDialog1->Files->Count;
  char rozszerzenie[_MAX_EXT];
  int indeks;
  if(OpenDialog1->Execute())
  {
        indeks =OpenDialog1->FilterIndex;
        if (indeks ==5 )
                indeks = 3 ;             // je¿eli wybrano pliki spektrometru to
                                         // to podonie jak gdy wybrano wszystkie pliki.
	switch (indeks)
	{
		case 1: WczytajMLA();
				break;
		case 4: WczytajEPR();
				break;
		case 2:
		default: WczytajDAT();
				break;
                case 3: _splitpath(OpenDialog1->FileName.c_str(),0,0,0,rozszerzenie);
		  strlwr(rozszerzenie);
		  bool wynik;
		  int wynikPor=strncmp(rozszerzenie,".dat",4);
		  if(!wynikPor)
		  {
                        WczytajDAT();
		  }
		  else if(!strncmp(rozszerzenie,".epr",4))
                  {
                        WczytajEPR();
                  }
                  else if(!strncmp(rozszerzenie,".mla",4))
                  {
                        WczytajMLA();
                  }
                  else
                  {
                    if(Application->MessageBoxA("Nie rozpoznano rozszerzenia pliku. \n"
                                                "Mo¿na spróbowaæ wczytaæ plik przy za³o¿eniu, \n"
                                                "¿e to jest plik tekstowy typu \".dat\" \n"
                                                "Czy spróbowaæ wczytaæ plik?",
                                                "UWAGA!",MB_YESNO)==IDOK )
                    {
                        WczytajDAT();
                    }
                    else
                    {
                        return;
                    }
                  }
                  break;
	}
 }
 rejestr->zapiszStr("Katalog",katalog);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Koniec1Click(TObject *Sender)
{
  Close();
}


const crMyCursor=5;
void __fastcall TForm1::FormCreate(TObject *Sender)
{
  char drive[_MAX_DRIVE];
  char directory[_MAX_DIR];
  char filename[_MAX_FNAME];
  char extension[_MAX_EXT];
  char path[_MAX_PATH];
  _splitpath(ParamStr(0).c_str(),drive,directory,filename,extension);
  _makepath(path,drive,directory,0,0);
  char ShortPath[_MAX_PATH];
  GetShortPathName(ParamStr(0).c_str(),ShortPath,_MAX_PATH);
  AnsiString KrotkaNazwa = ShortPath;
  HICON K = LoadCursor(HInstance, "KROSS");
  Screen->Cursors[crMyCursor] = K;//LoadCursor(HInstance, "K¯Y¯YK");
  Image1->Cursor = crMyCursor;
  Application->ProcessMessages();
  Memo1->BringToFront();
  if(ParamCount()==1)
  {

	bool WynPoz=SetCurrentDir(path);
   // ShowMessage(AnsiString(path)+" "+AnsiString(int(WynPoz)));
   }
  if(!termopara.wczytaj_dane())
	  Application->MessageBox("Do prawid³owego dzia³ania przeliczania"
							  "temperatur Aplikacja potrzebuje danych"
							  "zawartych w zbiorze term.txt \n w katalogu"
							  " w którym znajduje siê program wykonywalny",
							  "Uwaga",MB_OK);
  else Memo1->Lines->Add("Wczytano plik term.txt zawieraj¹cy charakterystykê termopary");
  Application->ShowHint = true;
  TStringList *Uzytkownicy;
  rejestr->open();
  if(!rejestr->KeyExists("Nowy u¿ytkownik")) rejestr->CreateKey("Nowy u¿ytkownik");
  Uzytkownicy=new TStringList;
  if(!(rejestr->ValueExists("Ostatni")) || !(rejestr->KeyExists(rejestr->ReadString("Ostatni"))))
	  {
		//rejestr->WriteString("Ostatni","Nowy u¿ytkownik");
		rejestr->zapiszStr("Ostatni","Nowy u¿ytkownik");
	  }
  rejestr->open(true);
  rejestr->GetKeyNames(Uzytkownicy);
  Form2 = new TForm2(Application);
  if (Uzytkownicy->Count!=0)
  {
	  {
	   Form2->ComboBox1->Items=Uzytkownicy;
	  }
  }
  //Form2->ComboBox1->Text=rejestr->ReadString("Ostatni");
  Form2->ComboBox1->Text=rejestr->odczytStr("Ostatni");
  Form2->ShowModal();
  rejestr->open(true);
  switch (Form2->ModalResult)
  {
   case mrOk:   {
				 AnsiString NewString;
				 strstream napis;
				 napis<<" Aktualny u¿ytkownik: ";
				 if((Form2->ComboBox1->Text )== "Nowy u¿ytkownik")
				 {    if (InputQuery("Wpisz identyfikator", \
					  "Identyfikator", NewString))
				  {
					   //rejestr->WriteString("Ostatni",NewString);
					   //rejestr->OpenKey(NewString,true);
					   rejestr->zapiszStr("Ostatni",NewString);
					   rejestr->UstKlucz(NewString);
					   napis<< NewString.c_str()<<'\0';
				  }
                 }
                 else
                 {napis.seekp(0); napis<<endl;
				  napis<<"Czy zarejestrowaæ nowego u¿ytkownika: "<<Form2->ComboBox1->Text.c_str()<<endl;
				  if(!rejestr->KeyExists(Form2->ComboBox1->Text))
                  {
					if(MessageDlg(napis.str(),mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
					{
//						 rejestr->WriteString("Ostatni",Form2->ComboBox1->Text);
//						 rejestr->OpenKey(Form2->ComboBox1->Text,true);
						 rejestr->zapiszStr("Ostatni",Form2->ComboBox1->Text);
						 rejestr->UstKlucz(Form2->ComboBox1->Text);
						 napis.seekp(0);
						 napis<<"Aktualny u¿ytkownik: "<< Form2->ComboBox1->Text.c_str()<<'\0';
					}
				  }
				  else
				  {
//					  rejestr->WriteString("Ostatni",Form2->ComboBox1->Text);
//					  rejestr->OpenKey(Form2->ComboBox1->Text,false);
                      rejestr->zapiszStr("Ostatni",Form2->ComboBox1->Text);
					  rejestr->UstKlucz(Form2->ComboBox1->Text);
					  napis.seekp(0);
					  napis<<"Aktualny u¿ytkownik: "<< Form2->ComboBox1->Text.c_str()<<'\0';
				  }
				 }
				  Form1->Caption = napis.str();
				  rejestr->open();
                          //        Opcje.CzytajRejestr();
				  if (rejestr->ValueExists("Katalog"))
					katalog = rejestr->odczytStr("Katalog");
				  else
                    katalog = GetCurrentDir();

                }
                break;

   case mrAbort:  {
                    strstream napis;
					napis<<"Czy na pewno chcesz usun¹æ u¿ytkownika "<<Form2->ComboBox1->Text.c_str();
					rejestr->open(true);
					if(rejestr->KeyExists(Form2->ComboBox1->Text))
					  if (MessageDlg(napis.str(),mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
					  {
						rejestr->DeleteKey(Form2->ComboBox1->Text);
						if(rejestr->ReadString("Ostatni")==Form2->ComboBox1->Text.c_str())
					  //rejestr->WriteString("Ostatni","Nowy u¿ytkownik");
							 rejestr->zapiszStr("Ostatni","Nowy u¿ytkownik");
					  }
				  }
   case mrCancel:   exit(0);

  }
  Form2->Close();
  KeyPreview = true;

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//#undef Objekt
#define Obiekt
#ifndef Obiekt
  const nBuf=20000000;
  char BufTym[nBuf];
#endif

void __fastcall TForm1::Zbiorczy1Click(TObject *Sender)
{
  String S1,S2;
  if(!ListaPlikow)
  {
   Memo1->Lines->Add("");
   Memo1->Lines->Add("**** NIE WCZYTANO ¯ADNYCH PLIKÓW ****");
   return;
  }
  //SaveDialog1->Options.Clear();
  SaveDialog1->InitialDir=katalog;
//  SaveDialog1->Options << ofAllowMultiSelect << ofFileMustExist;
  SaveDialog1->Filter = "Pliki EPR Dat (*.dat)|*.dat|All files (*.*)|*.*";
  SaveDialog1->FilterIndex = 1;
  if(SaveDialog1->Execute())
  {
   char bufor[25]; // Bufor na liczbê

#ifdef Obiekt
     ofstream wyjscie(SaveDialog1->FileName.c_str());
#else

   ostrstream wyjscie(BufTym,nBuf,0);
   int uchwyt = open(SaveDialog1->FileName.c_str(),O_WRONLY|O_CREAT|O_TRUNC|O_TEXT,S_IREAD|S_IWRITE); //
   if (uchwyt==-1)
      {
        switch(errno)
        {
         case EACCES: S1="Brak Dostêpu";
                        break;
         case EBADF  : S1="Z³y uchwyt pliku";
                        break;
        }
        Application->MessageBox(S1.c_str(),"Uwaga",MB_OK);
        return;
      }
#endif
   strstream tymczas(bufor,25);						//strstream tymczas(bufor,25,0);
   char filename[_MAX_FNAME];
   char fileextension[_MAX_EXT];
   char path[_MAX_PATH];
   ZbiorTablic* Aktualna=ZbiorTablic::glowa;
   float dotych;
   if(Opcje.ZapisTemperatury)
   {
     while(Aktualna)
     {
        wyjscie<<"T [K] =";
        for (int k=1;k<Aktualna->LiczKolumn();k++)
        {
        // k-1 bo linie w obrêbie danego ZbioruLinii sa indeksowane od 0,
         //  a zerowa kolumna tablicy zawiera X-sy
               dotych=static_cast<float>(Aktualna->DajWartosc(Aktualna->SzukajNr(k-1)));
               wyjscie<<'\t'<<termopara.TempKelvin(dotych);
        }
        wyjscie<<"\t\t\t\t";
        Aktualna=Aktualna->DajNast();
     }
     wyjscie<<endl;
     Aktualna=ZbiorTablic::glowa;
   }

   while(Aktualna)
   {
      wyjscie<<"B[mT] ";
      for (int k=1;k<Aktualna->LiczKolumn();k++)
      {
       _splitpath(Aktualna->NazwaPliku(k-1).c_str(),0,0,filename,fileextension);
       _makepath(path,0,0,filename,fileextension);
       wyjscie<<'\t'<<path;
      }
      wyjscie<<"\t\t\t\t";
      Aktualna=Aktualna->DajNast();
   }
   wyjscie<<'\n';
   int indeks;
   Memo1->Lines->Add("");
   indeks = Memo1->Lines->Add(" **** ZAPISYWANIE PLIKU ZBIORCZEGO **** ");
   Memo1->Lines->Add("");
   Memo1->Lines->Add("");
   strstream ekran;
   indeks++;
   Aktualna=ZbiorTablic::glowa;
   for(int i=0;i<Aktualna->MaxRozm();i++) // dla maxymalnej liczby wierszy
   {
      if(!(i%100))
      {
       Memo1->Lines->Delete(indeks);
       S1="Zapisujê aktualnie wiersz nr ";
       S2=i;
       S1=S1+S2;
       Memo1->Lines->Insert(indeks,S1);
      }
      while(Aktualna)
      {

       for(int j=0;j<Aktualna->LiczKolumn() && i< Aktualna->LiczWierszy();j++)
       {
         tymczas<<Aktualna->Tablica(i,j)<<'\0';
         int z=0;
         while(bufor[z]!='\0')
         { if (bufor[z]=='.')
           {
             wyjscie.put(',');
             z++ ;
           }
           wyjscie.put(bufor[z++]);
         }
         wyjscie<<"\t";
         tymczas.seekp(0);
       }
       wyjscie<<"\t\t\t";
       Aktualna=Aktualna->DajNast();
     }                              // ******************** Koniec While Aktualna
     Aktualna=ZbiorTablic::glowa;
     wyjscie<<'\n';
   }
                      // ******************** Koniec FOR
#ifndef Obiekt
   wyjscie<<ends;

   int lzap=  wyjscie.pcount();
  // char* BuforZapisu = new char[lzap];
  // strcpy(BuforZapisu,wyjscie.str());
   int ldzap = write(uchwyt,wyjscie.str(),lzap);
   wyjscie.rdbuf()->freeze(false);
   if (ldzap==-1)
      {
        switch(errno)
        {
         case EACCES: S1="Brak Dostêpu";
                        break;
         case EBADF  : S1="Z³y uchwyt pliku";
                        break;
        }
        Application->MessageBox(S1.c_str(),"Uwaga",MB_OK);

      }

   else
   {
     char tym;
     tym=EOF;
     write(uchwyt,&tym,1);
     Memo1->Lines->Add("**** KONIEC ZAPISYWANIA PLIKU ZBIORCZEGO ****");
   }
   close(uchwyt);
  #else
     Memo1->Lines->Add("**** KONIEC ZAPISYWANIA PLIKU ZBIORCZEGO ****");
  #endif
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PlikizKropk1Click(TObject *Sender)
{
   String S1,S2;
   if(!ListaPlikow)
  {
   Memo1->Lines->Add("");
   Memo1->Lines->Add("**** NIE WCZYTANO ¯ADNYCH PLIKÓW ****");
   return;
  }
  TSaves* Save = new TSaves(this);
  Save->Katalogi->Directory=katalog;
  if(Save->ShowModal()==mrOk)
  {

   AnsiString sciezka;
   char filename[_MAX_FNAME];
   char fileextension[_MAX_EXT];
   char filekatalog[_MAX_DIR];
   char filedysk[_MAX_DRIVE];
   char path[_MAX_PATH];
   filedysk[0]= Save->Katalogi->Drive;
   filedysk[1]='\0';

   strcpy(filekatalog,Save->Katalogi->Directory.c_str());
   //_splitpath(SaveDialog2->FileName.c_str(),filedysk,filekatalog,0,0);
   strstream ekran;
   if(ZbiorTablic::glowa)// je¿eli stowrzono listê plików
   {
     Memo1->Lines->Add("");
     Memo1->Lines->Add("**** ZAPISYWANIE PLIKÓW DAT ****");
     ZbiorTablic* Akt=ZbiorTablic::glowa;
     if(Opcje.SeparatorKolumn==StructOpcje::Inne)
                     strcpy(ZnakKolumny, Opcje.TekstInne.c_str());
     else
     {
        ZnakKolumny[0]= Opcje.ZnakSeparatora;
        ZnakKolumny[1]='\0';
     }

     while(Akt)
     {
        for(int i=1;i<Akt->LiczKolumn();i++)
        {
           _splitpath(Akt->NazwaPliku(i-1).c_str(),0,0,filename,fileextension);
//           _makepath(path,filedysk,filekatalog,filename,"dat");
           _makepath(path,"",filekatalog,filename,"dat");
          ofstream wyjscie(path);
          if(wyjscie)
          {
            S2=path;
            S1="Zapisujê aktualnie plik: "+S2;
            Memo1->Lines->Add(S1);
            for(int j=0;j<Akt->LiczWierszy();j++)
            {
              wyjscie<<Akt->Tablica(j,0)<<ZnakKolumny<<Akt->Tablica(j,i);
              wyjscie<<'\n';
              if( (Akt->LiczWierszy()> 2048) && (Opcje.ZredukowanaLiczbaDanych)) j++; //wczytaj co drug¹ liczbê
            }                                                              //je¿eli plik zwiera co najmniej 2048 wierszy
            wyjscie.close();
          }
        }
        Akt=Akt->DajNast();
     }
     Memo1->Lines->Add("");
     Memo1->Lines->Add("**** KONIEC ZAPISU PLIKÓW DAT ****");
   }
   else
  {
   Memo1->Lines->Add("");
   Memo1->Lines->Add("**** NIE WCZYTANO ¯ADNYCH PLIKÓW ****");
  }
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Opcje1Click(TObject *Sender)
{
 /*if(!ListaPlikow)
    {
     Memo1->Show();
     Memo1->Lines->Add("");
     Memo1->Lines->Add("**** NIE WCZYTANO ¯ADNYCH PLIKÓW ****");
     rysunek = false;
//     PageControl1->Visible=false;
     return;
    } */
  Application->CreateForm(__classid(TOknoOpcje),&OknoOpcje);
 //OknoOpcje = new TOknoOpcje(this);
 OknoOpcje->ShowModal();
 if(OknoOpcje->ModalResult==mrOk)
    Invalidate();
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------




void __fastcall TForm1::rysuj(TObject *Sender)
{
try{
  if(!rysunek) return;
  ZbiorTablic *Akt = ZbiorTablic::glowa;
  Akt=DajZb(NumerPliku);
  int Szer= ScrollBox1->ClientWidth+int(ScrollBox1->ClientWidth*PowiekszX);
  if (Szer >= Akt->LiczWierszy())
     {
       PowiekszX-=0.1;
       Szer= ScrollBox1->ClientWidth+int(ScrollBox1->ClientWidth*PowiekszX);
       MessageBeep(MB_ICONEXCLAMATION);
      // Application->MessageBox("Cz³owieku ZG£UPIA£EŒ!! nie mo¿na tak bez koñca",
      //                          "Niegrzeczna Uwaga",MB_OK);
      Application->MessageBox("Zbyt ma³o punktów albo za du¿e powiêkszenie",
                                "Uwaga",MB_OK);
     }
  Image1->Width  = Szer;
  Image1->Height = ScrollBox1->ClientHeight-ScrollBox1->HorzScrollBar->Size;
  Zaznaczenie->UstKolorLinii(clGreen);
  Zaznaczenie->UstKolorWypeln(clGreen);
  TCanvas *MBCanv;
  TCanvas *IMCanv=Image1->Canvas;
  delete MBit;
  MBit = new Graphics::TBitmap();
  MBCanv=MBit->Canvas;
  strstream ekran;
  static AnsiString Belka=Form1->Caption;
  char filename[_MAX_FNAME];
  char fileextension[_MAX_EXT];
  char path[_MAX_PATH];
  MBit->Height=Image1->ClientHeight;
  MBit->Width=Image1->ClientWidth;
  AnsiString Pan;
  Pan.printf("Zazn = %d",LiczbaZebranychPolozen);
  StatusBar1->Panels->Items[1]->Text = Pan;
  //StatusBar1->Panels->Items[2]->Text="YMax = " + IntToStr(MBit->Height);
  if(AktWykrLinii)
    { delete AktWykrLinii;
    }
  AktWykrLinii= new TJWykresyLinii(MBit);
  AktWykrLinii->UstawKolorLinii(clRed);
  AktWykrLinii->UstZakrZazn(10);
  if(!ZbiorTablic::glowa)
    {
     Memo1->Show();
     Memo1->Lines->Add("");
     Memo1->Lines->Add("**** NIE WCZYTANO ¯ADNYCH PLIKÓW ****");
     rysunek = false;
     return;
    }

  _splitpath(DajNazweZbioru(NumerPliku).c_str(),0,0,filename,fileextension);
  _makepath(path,0,0,filename,fileextension);
  ekran<<Belka.c_str()<<"   ";
  ekran<<"Aktualnie wybrany plik: "<<filename<<fileextension;
  ekran<<" numer "<<NumerPliku;
  ekran<<'\0'; ekran.seekp(0);
  Form1->Caption=ekran.str();
  double max = MINDOUBLE;
  double min = MAXDOUBLE;
  if(ToolButtonScalaGlobalLocal->Down)
           AktWykrLinii->UstMaxMinY(ZbiorTablic::GlobalMaxY,ZbiorTablic::GlobalMinY);
  else
  {
      Akt=DajZb(NumerPliku);
      for(int i=0;i<Akt->LiczWierszy();i++)
      {
       double tym = DajY(i,NumerPliku);
       if (tym>max) max = tym;
       if (tym<min) min = tym;
      }
      if( min == max)
        {
         Memo1->Lines->Add("");
         Memo1->Lines->Add("Maksimum linii jest równe jej minimum");
         Memo1->Lines->Add("Nie mogê narysowaæ takiej linii");
         Memo1->Show();
         return;
        }
        AktWykrLinii->UstMaxMinY(max,min);
  }
  TPoint Punkt = AktWykrLinii->DajWsp(0,DajY(0,NumerPliku));
  MBit->Canvas->MoveTo(Punkt.x,Punkt.y);
  if(Opcje.PokazPunkty)
        for(int i=1;i<Akt->LiczWierszy();i++)
        {
         Punkt= AktWykrLinii->DajWsp(i,DajY(i,NumerPliku));
         Doswiadczalny->Rysuj(MBit->Canvas,Punkt.x,Punkt.y);
         MBit->Canvas->LineTo(Punkt.x,Punkt.y);
        }
  else
        for(int i=1;i<Akt->LiczWierszy();i++)
        {
         Punkt= AktWykrLinii->DajWsp(i,DajY(i,NumerPliku));
         MBit->Canvas->LineTo(Punkt.x,Punkt.y);
        }
  AnsiString T, T_Kelvin;
  TPoint BB;
  int X1S,X2S;
  float SzerB;
  Punkty* tym=Akt->Pola[Akt->DajIndeks(NumerPliku)].nastepny;
 switch(Tryb)
  {
   case ZaznaczaniePolRezonansowych:
   case Nieokreslony:

            Punkt* P;
            while( P=ZebranePunkty.DajNastepny(NumerPliku) )         
            {
                Punkt =  AktWykrLinii->DajWsp(P->DajIndeks(),DajY(P->DajIndeks(),NumerPliku));
                Zaznaczenie->Rysuj(MBit->Canvas,Punkt.x,Punkt.y);
            }
         /*
            while(tym)         // Odœwie¿anie zaznaczonych punktów
            {
               if(tym->indeks < 0 && tym->indeks >= Akt->LiczKolumn())
                                       throw ErrorZakres(tym->indeks) ;
               Punkt= AktWykrLinii->DajWsp(tym->indeks,DajY(tym->indeks,NumerPliku));
               Zaznaczenie->Rysuj(MBit->Canvas,Punkt.x,Punkt.y);
               tym=tym->nastepny;
            }
            */
            break;
   case ZaznaczanieSzerokosci:
            BB=Akt->DajSzer(NumerPliku,1);

            if(BB.x>=0 && BB.y >=0)
            {
                 X1S= (BB.x<BB.y)?BB.x:BB.y ;
                 X2S= (BB.x>BB.y)?BB.x:BB.y ;
                AktWykrLinii->UstawKolorLinii(clBlue);
                Punkt= AktWykrLinii->DajWsp(X1S,DajY(X1S,NumerPliku));
                MBit->Canvas->MoveTo(Punkt.x,Punkt.y);
                for(int i=X1S; i< X2S;     i++)
                {

                 Punkt= AktWykrLinii->DajWsp(i,DajY(i,NumerPliku));
                 if(Opcje.PokazPunkty) Doswiadczalny->Rysuj(MBit->Canvas,Punkt.x,Punkt.y);
                 MBit->Canvas->LineTo(Punkt.x,Punkt.y);
                }
                AktWykrLinii->UstawKolorLinii(clRed);
                SzerB= DajX(X2S,NumerPliku) - DajX(X1S,NumerPliku);
                T.printf("Szerokosc = %g",SzerB);
                          StatusBar1->Panels->Items[2]->Text=T;
            }
            if(BB.x>=0)
             { Punkt= AktWykrLinii->DajWsp(BB.x,DajY(BB.x,NumerPliku));
              Zaznaczenie->Rysuj(MBit->Canvas,Punkt.x,Punkt.y);}
            if(BB.y>=0)
             { Punkt= AktWykrLinii->DajWsp(BB.y,DajY(BB.y,NumerPliku));
              Zaznaczenie->Rysuj(MBit->Canvas,Punkt.x,Punkt.y);
              }
             if(BB.x <0 || BB.y <0)
             {
                 T.printf("Szerokosc = %g",0);
                          StatusBar1->Panels->Items[2]->Text=T  ;
             }
            break;

  }
  using namespace PrzestrzenOpcji;

  float f =Akt->DajWartosc(NumerPliku);
  switch (Opcje.ZapisywaneDane)
  {
    case TemperaturaK:
    case TemperaturaC:
                        if(  f>-5.96  && f <19.63)
                        {
                          T_Kelvin.printf("T = %.2f K",termopara.TempKelvin(f));
                          StatusBar1->Panels->Items[3]->Text=T_Kelvin;
                          T.printf("T = %.2f °C",termopara.TempCelsjusz(f));
                          StatusBar1->Panels->Items[4]->Text=T;
                          StatusBar1->Panels->Items[2]->Text="Temperatury";
                        }
                        else
                        {
                         StatusBar1->Panels->Items[4]->Text=" Nie Znana Temperatura ";
                         if(Application->MessageBox("Pojawi³a siê wartoœæ spoza zakresu.\n"
                                                    " \n \n"
                                                    "Czy zhcesz zmieniæ opcje zapisu?",
                                                    "Uwaga", MB_YESNO)==IDYES)
                             Form1->Opcje1Click(Sender);
                        }
                        break;
    case Anizotropia:
                          StatusBar1->Panels->Items[3]->Text="Aniz";
                          T.printf("K¹t = %g",f);
                          StatusBar1->Panels->Items[2]->Text=T;
                          StatusBar1->Panels->Items[4]->Text="";
                         break;

    case        Inne:
                       // if(  f<-5.96  || f >19.63)
                          StatusBar1->Panels->Items[3]->Text="Inne";
                          StatusBar1->Panels->Items[2]->Text=" ";
                          StatusBar1->Panels->Items[4]->Text=" ";
                         break;
         default : ;
                        break;
  }
  if(FormularzOpcjiPrezentacji->CheckBox1->Checked)
  {
      MBit->Canvas->Font=LabelDane->Font;
      if(Opcje.ZapisywaneDane==TemperaturaK)
      {
            LabelDane->Caption=T_Kelvin;
            MBit->Canvas->TextOutA(LabelDane->Left,LabelDane->Top,T_Kelvin);

      }
      else
      {
            MBit->Canvas->TextOutA(LabelDane->Left,LabelDane->Top,T);
            LabelDane->Caption=T;
      }

  }
  Image1->Canvas->Draw(0,0,MBit);
 // LabelDane->Caption=T;
 // LabelDane->BringToFront();
      if(PrzycPoziomZaznaczania->Down)
         Form1->RysujPoziomZaznaczania();
  //Image1->Picture->Assign(MBit);
 }
 catch(ErrorDajY)
 {
  Application->MessageBox("Nie prawid³owy numer pliku",
                              "Uwaga!!",MB_OK);
 }
 catch(ErrorNazwaZb)
 {
  Application->MessageBox("Nie prawid³owa nazwa pliku",
                              "Uwaga!!",MB_OK);
 }
 catch(ErrorDajZb)
 {
  Application->MessageBox("Nie prawid³owy wskaŸnik pliku",
                              "Uwaga!!",MB_OK);
 }
 catch(ErrorZakres Z)
 {
   AnsiString T,T1;
   switch(Z.Rodzaj)
   {
     case ErrorZakres::X: T1="Indeks X-sowy ";
                break;
     case ErrorZakres::Y: T1="Indeks Y-kowy ";
                break;
   }
   T.printf("tablicy powoduj¹cy b³¹d =%d",Z.indeks);
   T=T1+T;
   Application->MessageBox(T.c_str(),"B³¹d",MB_OK);
 }
 catch(...)
 {
    Application->MessageBox("Nie wiadomo co w funkcji Rysuj",
                              "Uwaga!!",MB_OK);
 }
}


//---------------------------------------------------------------------------




void __fastcall TForm1::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
 if(rysunek)
 {
  double pole;
  pole = DajX(int(X/WspX),NumerPliku); //TablicaZbiorcza[int(X/WspX)][0];
  //StatusBar1->SimplePanel;
  StatusBar1->Panels->Items[0]->Text=AnsiString("Pole magnetyczne wynosi " + FloatToStrF(pole,ffFixed,15,3)+ " [mT] ");

 }
}
//---------------------------------------------------------------------------

//Funkcja rysuj¹ca poziom¹ liniê aby bylo latwiej trzymac poziom zaznaczania pol rezonansowych
void TForm1::RysujPoziomZaznaczania(int YY) //Domyslnym parametrem jest zero, wtedy rysuje w poprzednim
{
    static poziom = YY;
    if(YY!=0) poziom = YY;
    Image1->Canvas->MoveTo(0,poziom);
    Image1->Canvas->LineTo(Image1->ClientWidth,poziom);
}
void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    ZbiorTablic* Akt=DajZb(NumerPliku);
    int Indeks;
    if(PrzycPoziomZaznaczania->Down && Shift.Contains(ssAlt) /*PrzycWstPunkt->Down &&  !PrzycWstSzer->Down*/) // Mo¿na to zmienic u¿ywaj¹c trybow??? Tryb.
    {
       Invalidate();
       RysujPoziomZaznaczania(Y);
       return;
    }
    if(rysunek && Tryb.DajTryb()==ZaznaczaniePolRezonansowych)
   {

    switch (Button)
    {
    case mbLeft:
         { int Indeks;
           if((Indeks=SzukajNajblIndeksu(Akt,X,Y))>=0)
           {
             Punkt P(NumerPliku,Indeks,Akt->DajWartosc(NumerPliku),DajX(Indeks,NumerPliku));
             if(ZebranePunkty.DodajPunkt(P));
             if (Akt->Pola[Akt->DajIndeks(NumerPliku)].Dodaj(Indeks))
                         LiczbaZebranychPolozen++;
           }
           else
             return;
           break;
         }
    case mbRight:
         {
           if(!Akt->Pola[Akt->DajIndeks(NumerPliku)].Kasuj(int(X/WspX)))
                 return;  // Pole[0] dotyczy kolumny 1 TablicyZbiorczej
           else
               if( LiczbaZebranychPolozen>0) LiczbaZebranychPolozen--;
            break;
         }
    }
    Invalidate();
   }
   if(rysunek && Tryb.DajTryb()==ZaznaczanieSzerokosci)
   {
    static bool pierwsze=true;
    static bool ustawione = false;
    static int  OstNr = NumerPliku;
    switch (Button)
    {
    case mbLeft:
           if((Indeks=SzukajNajblIndeksu(Akt,X,Y))>=0) //Akt->DodajMaxMin(NumerPliku,Indeks)
           {
              if(pierwsze && !ustawione)
              {
                    UstPunktBMax(NumerPliku,Indeks);
                    pierwsze = false;
                    OstNr = NumerPliku;
              }
              else if(OstNr ==NumerPliku && !ustawione)
                   {
                     UstPunktBMin(NumerPliku,Indeks);
                     ustawione = true;
                     pierwsze = true;
                   }
                   else
                    return;
           }
           else
             return;
           break;

    case mbRight:
         {
            ustawione = false;
            pierwsze = true;
            UstPunktBMax(NumerPliku,-1);
            UstPunktBMin(NumerPliku,-1);
          /*
           if((Indeks=SzukajNajblIndeksu(Akt,X,Y))>=0 &&
                Akt->DodajMaxMin(NumerPliku,Indeks));
           else
             return;
            break;
            */
         }
    }
    Invalidate();
   }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::Wypisz1Click(TObject *Sender)
{
Punkty* tym;

strstream ekran;
if(!rysunek)
{
 for(int i=1; i<=LiczbaWczytPlikow;i++)
 {
   ZbiorTablic* Akt=DajZb(i);
   if(tym=Akt->Pola[Akt->DajIndeks(i)].nastepny)
   {
      ekran<<"Dla Pliku "<<DajNazweZbioru(i).c_str();
      ekran<<" odczytane pola :";
      ekran<<'\0'; ekran.seekp(0);
      Memo1->Lines->Add(ekran.str());
      int j=1;
      while(tym)
       {
          ekran<<"Pole nr "<<j++<<" = ";
          ekran<<DajX(tym->indeks,i)<< " [mT] ";
          ekran<<'\0'; ekran.seekp(0);
          Memo1->Lines->Add(ekran.str());
          tym=tym->nastepny;
       }
   }
 }
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormResize(TObject *Sender)
{
 if(rysunek) rysuj(Sender);
 //Invalidate();
 //rysuj(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormPaint(TObject *Sender)
{
if(rysunek /*!LabelDane->Visible*/)
   rysuj(Sender);
}
//---------------------------------------------------------------------------





void __fastcall TForm1::Wykres1Click(TObject *Sender)
{
if(!ListaPlikow)
    {
     Memo1->Show();
     Memo1->Lines->Add("");
     Memo1->Lines->Add("**** NIE WCZYTANO ¯ADNYCH PLIKÓW ****");
     rysunek = false;
     Image1->Visible=false;
     return;
    }
  static ustaw = 0;
  strstream ekran;
  if(ustaw)
  {
    ustaw=0;
    Memo1->Show();
    Memo1->Invalidate();
    rysunek = false;
    Image1->Visible=false;
    return;
  }
  ustaw = 1;
  Memo1->Hide();
  Image1->Visible=true;
  rysunek = true;
  rysuj(this);
}
//---------------------------------------------------------------------------


void UsunSzpilki(int Kolumna)
{

 double roznica=0;
 double maximum=1;
 ZbiorTablic* Akt=DajZb(Kolumna);
 if(DajY(0, Kolumna)>=1024)   //Je¿eli pierwszy punkt "wyskakuje" to arbitralnie
                              // przypisz 0   
 {
   Akt->PrzypiszStTab(0,Akt->DajIndeks(Kolumna)+1,0);
 }
 for(int i=1;i<Akt->LiczWierszy();i++)
 {
  int a,b;
  a=DajY(i, Kolumna);
  b=DajY(i-1,Kolumna);
  roznica = fabs(a>b?a-b:b-a);//fabs(DajY(i, Kolumna)-DajY(i-1, Kolumna));

 if(roznica>1024)     //arbitralnie przyjêta definicja wadliwego wpisu
  {
    Akt->PrzypiszStTab(i,Akt->DajIndeks(Kolumna)+1,b); //UWAGA! Niezgodno
    roznica = maximum;  // skasuj duza roznice, aby procedura mogla dzialac
                      // dla nastepnych przypadkow (np dwa piki).
    a=DajY(i, Kolumna);
    b=DajY(i-1,Kolumna);
    continue;
  }



 // if (roznica >(20*maximum)|| (DajY(i, Kolumna)==1024)) // Maksymalna wartoœæ sygna³u 1024
  if (roznica >(20*maximum)|| (a==1024))
  {
    if(fabs(DajY(i,Kolumna)>fabs(DajY(i-1, Kolumna))))
    {
//     Akt->PrzypiszStTab(i,Akt->DajIndeks(Kolumna),Akt->Tablica(i-1,Akt->DajIndeks(Kolumna)));

           Akt->PrzypiszStTab(i,Akt->DajIndeks(Kolumna)+1,b); //UWAGA! Niezgodno
    }
    else
    {
//      Akt->PrzypiszStTab(i-1, Akt->DajIndeks(Kolumna)+1,Akt->Tablica(i,Akt->DajIndeks(Kolumna)));
        Akt->PrzypiszStTab(i-1, Akt->DajIndeks(Kolumna)+1,Akt->Tablica(i,Akt->DajIndeks(Kolumna)));
    }
    roznica = maximum;  // skasuj duza roznice, aby procedura mogla dzialac
                      // dla nastepnych przypadkow (np dwa piki).
    a=DajY(i, Kolumna);
    b=DajY(i-1,Kolumna);
  }

  if (roznica > maximum) maximum=roznica;

 }
}

void __fastcall TForm1::AktualnaLinia1Click(TObject *Sender)
{
  UsunSzpilki(NumerPliku);
  Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::WszystkieLinie1Click(TObject *Sender)
{
 for(int i = 1;i<=LiczbaWczytPlikow;i++)
  UsunSzpilki(i);
 Invalidate();
}
//---------------------------------------------------------------------------



void __fastcall TForm1::UpDown2Click(TObject *Sender, TUDBtnType Button)
{
 if(!rysunek) return;
 NumerPliku= TablicaNrPlikow[UpDown2->Position - 1];  // tablica zaczyna siê od zera.
//NumerPliku = UpDown2->Position;
Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Wypisz2Click(TObject *Sender)
{
    AnsiString S1, S2;
    ZbiorTablic* Akt=ZbiorTablic::glowa;
    double dotych;
     while(Akt)
     {
        int Licz = Akt->LiczKolumn();
        for(int i=1;i<Licz;i++)
        {
            S2=Akt->NazwaPliku(i-1);
            S1="Wypisujê aktualnie plik: "+S2;
            dotych=Akt->DajWartosc(Akt->SzukajNr(i-1));
            Memo1->Lines->Add("Wartoœæ stowarzyszona = " + String(dotych));
            Memo1->Lines->Add(S1);
            S1.printf("T[C] = %f \t\t  T[K] = %f",termopara.TempCelsjusz(dotych),
                                termopara.TempKelvin(dotych));
            Memo1->Lines->Add(S1);
            for(int j=0;j<5;j++)
            {
              S1=Akt->Tablica(j,0);
              S1=S1+"  ";
              S2=Akt->Tablica(j,i);
              Memo1->Lines->Add(S1+S2);
             // if( (Akt->LiczWierszy()> 2048) && (CheckBox1->Checked)) j++; //wczytaj co drug¹ liczbê
            }                                                              //je¿eli plik zwiera co najmniej 2048 wierszy


        }
        Akt=Akt->DajNast();
     }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Termopara1Click(TObject *Sender)
{
 String a;
 Memo1->Lines->Add("Wypisujê wzorzec termopary");
 int i=termopara.zacznij_iteracje();
 do
 {
   a.printf("T[C] = %.1f  \t\t  U[mV] = %.2f",
                              termopara.DajTemp(i),
                              termopara.DajNapiecie(i));
   Memo1->Lines->Add(a);
 }
 while(i=termopara.iteruj()) ;
}
//---------------------------------------------------------------------------




void __fastcall TForm1::EksportujListy1Click(TObject *Sender)
{
     enum {Matematika=1,PlikiDat=2,PlikiSzerDat=3,PlikiIntens=5,InnePliki=4} TypPliku;
//     enum {Matematika=1,PlikiDat=2,PlikiSzerDat=3,InnePliki=4} TypPliku;
     using namespace PrzestrzenOpcji;

     char bufor[25]; // Bufor na liczby
     strstream tymczas(bufor,25); //strstream tymczas(bufor,25,0);
     strstream ekran;
     EksportDialog->FilterIndex=Matematika;
      if(EksportDialog->Execute())
      {
       StatusBar1->Panels->Items[0]->Text="Wybrany Numer Indeksu ="+IntToStr(EksportDialog->FilterIndex);
       TypPliku=EksportDialog->FilterIndex;

       ofstream wyj(EksportDialog->FileName.c_str());
       switch(TypPliku)
       {
         case Matematika   : EksportujMathematica(EksportDialog->FileName.c_str());
                             break;
         case PlikiDat     : EksportujDat(EksportDialog->FileName.c_str());
                             break;
         case PlikiSzerDat : EksportujSzerDat( EksportDialog->FileName.c_str());
                             break;
         case PlikiIntens  : EksportujIntensywnoscDat(EksportDialog->FileName.c_str());
                             break;
         default           : EksportujDat(EksportDialog->FileName.c_str());
                             break;
       }

      }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormMouseWheel(TObject *Sender, TShiftState Shift,
      int WheelDelta, TPoint &MousePos, bool &Handled)
{
 if(!Memo1->Visible)
 {
  int tym =UpDown2->Position;
  //(WheelDelta>0 )? NumerPliku++:NumerPliku--;
  if(WheelDelta>0 )
        UpDown2->Position=tym+1;
  else
        UpDown2->Position=tym-1;
                        /*if(NumerPliku>LiczbaWczytPlikow)
                           {
                             NumerPliku=1;
                           }
                        if(NumerPliku < 1)
                          {
                           NumerPliku=LiczbaWczytPlikow;
                          }
                        UpDown2->Position=NumerPliku; */
 /* if(UpDown2->Position>LiczbaWczytPlikow)
     {
       UpDown2->Position=1;
     }
  if(UpDown2->Position < 1)
    {
     UpDown2->Position=LiczbaWczytPlikow;
    } */
  NumerPliku=TablicaNrPlikow[UpDown2->Position - 1];
  Invalidate();
 }
}
//---------------------------------------------------------------------------






void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
 rysunek=false;
switch (Opcje.SeparatorKolumn)
 {
   case StructOpcje::Przecinek:

				   rejestr->zapiszStr("Separator Kolumn",",");
				   rejestr->zapiszInt("NrSep",0);
				   break;
   case StructOpcje::Tabulacja :
						rejestr->zapiszStr("Separator Kolumn","\t");
						rejestr->zapiszInt("NrSep",1);
						break;
   case StructOpcje::Spacja    :
						rejestr->zapiszStr("Separator Kolumn"," ");
						rejestr->zapiszInt("NrSep",2);
						break;
   case StructOpcje::Inne      :
						rejestr->zapiszStr("Separator Kolumn",Opcje.TekstInne);
						rejestr->zapiszInt("NrSep",3);

                                             //   break;
}
   rejestr->zapiszInt("Indeks Dodatkowych Danych",Opcje.ZapisywaneDane);
   rejestr->zapiszInt("Aktywna Zak³adka",Opcje.AktywnaZakladka);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ToolButton1Click(TObject *Sender)
{
  ZbiorTablic* Akt=DajZb(NumerPliku);
  if(Akt) //Je¿eli zlokalizowano zbiór
  {
     int i= Akt->DajIndeks(NumerPliku);
     Punkty TymZ=Akt->Pola[i];
     LiczbaZebranychPolozen-=TymZ.UsunWszystkie();
     Invalidate();
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PrzycWstPunktClick(TObject *Sender)
{
  if(PrzycWstPunkt->Down)
  {
    Tryb=ZaznaczaniePolRezonansowych;
    Image1->Cursor = crCross;
  }
  else
  {
    Tryb =Nieokreslony;
    Image1->Cursor = crDefault;
  }

}
//---------------------------------------------------------------------------


void __fastcall TForm1::ToolButton4Click(TObject *Sender)
{
 if(rysunek)
 {
  if(!PowiekszX)
        PowiekszX=0.1;
  else
    PowiekszX+=0.1;
  Invalidate();
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton5Click(TObject *Sender)
{
  if(rysunek)
 {
  if(PowiekszX<0)
  {
        PowiekszX=0;
        return;
  }
  else
     PowiekszX-=0.1;
  Invalidate();
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton6Click(TObject *Sender)
{
  PowiekszX=0;
  Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PrzycWstSzerClick(TObject *Sender)
{
  if(PrzycWstSzer->Down)
  {
    Tryb=ZaznaczanieSzerokosci;
    Image1->Cursor = crCross;
   // Invalidate();
  }
  else
  {
    Tryb=Nieokreslony;
    Image1->Cursor = crDefault;
  }

}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        // Application->MessageBox("Nacisnieto klawisz","Informacja",MB_OK);
        if(!rysunek) return;
        int tym =UpDown2->Position;
        switch(Key)
        {
          case VK_UP:
          case VK_RIGHT: UpDown2->Position=tym+1;
                        break;
          case VK_LEFT:
          case VK_DOWN: UpDown2->Position=tym-1;
                        break;
          default:  return;                             //¿aden klawisz to nie rób nic
        }
        NumerPliku=TablicaNrPlikow[UpDown2->Position - 1];
        Invalidate();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::WykresPunkty1Click(TObject *Sender)
{
 if(Tryb.DajTryb()!=ZaznaczaniePolRezonansowych) return;
 if(LiczbaZebranychPolozen==0) return;
   if(!WykresPktForm->Visible)
   {
        WykresPktForm->Polozenia->Update(LiczbaZebranychPolozen);
        WykresPktForm->Polozenia->rysuj();
        WykresPktForm->Visible=true;
   }
   else
   {
         WykresPktForm->BringToFront();
        //WykresPktForm->Visible=false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Nazwy1Click(TObject *Sender)
{
 if(!ListaPlikow) {
        Application->MessageBox("Nie wczytano ¿adnych plików", "Uwaga",MB_OK);
        return;
 }
 if(!ListBox1 )
 {
       // Panelek = new TPanel(this);
       // Panelek->Parent=this;
       // Panelek->Top=Height/2;
      //  Panelek->Left=Width/2;
        ListBox1 = new TListBox(this);
        ListBox1->Parent=this;
        ListBox1->Visible=true;
        int szer= ListBox1->Font->Height;
        if (szer<0)
          szer= ListBox1->Font->Size * ListBox1->Font->PixelsPerInch / 72;

        ListBox1->Top=(Height-ListBox1->Height)/2;
        ListBox1->Width= ListaPlikow->Strings[0].Length()*szer;
        if(ListBox1->Width<Width)
            ListBox1->Left=(Width -ListBox1->Width)/2;
        else
          {
                    ListBox1->Left= 4;
                    ListBox1->Width=Width-8;
          }
        if(ListaPlikow) ListBox1->Items =ListaPlikow;
        ListBox1->BringToFront();
 }
 else
 {
   delete ListBox1;
   delete Panelek;
   Panelek=0;
   ListBox1=0;
 }
}
// -------------------------------------------------------------------------

void __fastcall TForm1::ToolButton9Click(TObject *Sender)
{
  TPrzelicznik* Przelicz = new TPrzelicznik(this);
  Przelicz->Show();              
}
//---------------------------------------------------------------------------


void __fastcall TForm1::PunktyDoswiadczalneClick(TObject *Sender)
{
try{
 if(PunktyDoswiadczalne->Down)
 {
        Opcje.PokazPunkty=true;
        if(OknoOpcje)
                OknoOpcje->PokazPunkty->Checked=true;
 }
 else
 {
        Opcje.PokazPunkty=false;
        if(OknoOpcje)
                OknoOpcje->PokazPunkty->Checked=false;
 }
 Invalidate();
 }
 catch (EConvertError& d) //EConvertError
      {
       Application->MessageBox("Komunikat PunktyDoswiadczalneClick"," Opcja nie zawiera w³aœciwego formatu liczby ",MB_OK);
     //  NumerPliku=1;
       return;
      }
 catch(...)
 {
	 Application->MessageBox("Komunikat PunktyDoswiadczalneClick"," ?????????????? ",MB_OK);
	 //  NumerPliku=1;
	   return;
 }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::btnZapisSesjiClick(TObject *Sender)
{
  ZapisSesji();   // Funkcja znajduje siê w zbiorze ZapisOdczytSesji.cpp
				  // Zapis zbiorów i punktów biro¹cych udzia³ w sesji
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OtwarcieSesjiClick(TObject *Sender)
{
		if(!ListaPlikow)
			ListaPlikow = new TStringList;     // Dodaj do ListyPlikow
		else
		{
		 if( Application->MessageBox("Chwilowo odczyt sesji jest mo¿liwy jedynie wtedy gdy \n"
						"¿adne pliki nie s¹ wczytane. \nZAKOÑCZYÆ PROGRAM?","Uwaga",MB_YESNO )==IDYES)
				 Application->Terminate();
		 else
				 return;
		}

//  katalog=rejestr->odczytStr("KatalogSesji");
  OpenSesje->InitialDir=rejestr->odczytStr("KatalogSesji");
  if(Form1->OpenSesje->Execute())
  {
		OtwarcieZapisanejSesji(OpenSesje->FileName.c_str());
  }
}
//---------------------------------------------------------------------------






void __fastcall TForm1::PrzycPoziomZaznaczaniaMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  Invalidate();        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButtonScalaGlobalLocalClick(TObject *Sender)
{
 Invalidate();        
}
//---------------------------------------------------------------------------


void __fastcall TForm1::LabelDaneMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 if(Button == mbLeft)
 {
  TLabel *alfa=(TLabel *)Sender;
  alfa->BeginDrag(true,-1);

    X0=X; // Ofset dla labela :-)
    Y0=Y;

 }
 else
 {
   FontDialog1->Font=LabelDane->Font;
   if(FontDialog1->Execute())
   {
        LabelDane->Font=FontDialog1->Font;
   }
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image1DragOver(TObject *Sender, TObject *Source,
      int X, int Y, TDragState State, bool &Accept)
{
     if(Source->InheritsFrom(__classid(TLabel)) )
  {
    Accept=true;
  }
  TLabel *alfa=(TLabel *)Source;
  alfa->Top=Y-Y0;
  alfa->Left=X-X0;
}
//---------------------------------------------------------------------------



void __fastcall TForm1::ToolButton11Click(TObject *Sender)
{

   if(ToolButton11->Down)           // Edycja danych - ikona Etykiety (A).
   {
     LabelDane->Visible=true;
    // LabelDane->Enabled=true;
     LabelDane->DragMode = dmManual;
   }
   else
   {
     LabelDane->Visible=false;
     LabelDane->DragMode = dmAutomatic;
    // LabelDane->Enabled=false;
   }
   Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton12Click(TObject *Sender)
{
   if(ToolButton12->Down)
   {
        FormularzOpcjiPrezentacji->Timer1->Enabled=true;
   }
   else
   {
        FormularzOpcjiPrezentacji->Timer1->Enabled=false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton13Click(TObject *Sender)
{
  if(!FormularzOpcjiPrezentacji->Visible)
    FormularzOpcjiPrezentacji->Visible=true;
  else
    FormularzOpcjiPrezentacji->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image1EndDrag(TObject *Sender, TObject *Target,
      int X, int Y)
{
       Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Kszta1Click(TObject *Sender)
{
  if(!Form3->sprawdz())
  {
   Application->MessageBox("Nale¿y zaznaczyæ TYLKO JEDNO pole rezonansowe","Uwaga",MB_OK);
    return;
  }
  Form3->Visible=true;
  Form3->co_rysowac();
  Form3->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Zarejestruj1Click(TObject *Sender)
{
  char drive[_MAX_DRIVE];
  char directory[_MAX_DIR];
  char filename[_MAX_FNAME];
  char extension[_MAX_EXT];
  char path[_MAX_PATH];
  _splitpath(ParamStr(0).c_str(),drive,directory,filename,extension);
  _makepath(path,drive,directory,0,0);
  char ShortPath[_MAX_PATH];
  GetShortPathName(ParamStr(0).c_str(),ShortPath,_MAX_PATH);
  AnsiString KrotkaNazwa = ShortPath;
  rejestr->RootKey=HKEY_CLASSES_ROOT;
  if(rejestr->KeyExists(".met"))
  {
        rejestr->OpenKey(".met",true);
        rejestr->WriteString("","MetamorfozaFile");
        rejestr->OpenKey("\\MetamorfozaFile",true);
        rejestr->WriteString("","Dokument programu do obróbki widm EPR (Metamorfoza)");
        rejestr->CreateKey("DefaultIcon");
        rejestr->OpenKey("DefaultIcon",false);
        rejestr->WriteString("",KrotkaNazwa+",0");
        rejestr->OpenKey("\\MetamorfozaFile\\shell",true);
        rejestr->OpenKey("open",true);
        rejestr->OpenKey("command",true);
        rejestr->WriteString("",AnsiString("\"")+KrotkaNazwa+AnsiString("\" \"%1\""));
        rejestr->RootKey=HKEY_LOCAL_MACHINE;
        rejestr->OpenKey("\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths",false);
        rejestr->CreateKey(AnsiString(filename)+AnsiString(extension));
        rejestr->OpenKey(AnsiString(filename)+AnsiString(extension),false);
        rejestr->WriteString("",KrotkaNazwa);
        rejestr->WriteString("Path",KrotkaNazwa);
  }

}
//---------------------------------------------------------------------------

