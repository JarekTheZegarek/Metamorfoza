//---------------------------------------------------------------------------


#pragma hdrstop
#include <map>
#include "Wykres.h"
#include "Eksport.h"
#include "WykresPunktowy.h"
#include "Punkty.h"
#include "Test.h"
#include "StrukturaOpcje.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TJWykresyLinii *AktWykrLinii;
extern int popr,NumerPliku;
extern ObszarPunkty::TListaPrzejsc ListaPrzejsc;       // Lista zdefiniowanych przejsc dla Weila
extern ObszarPunkty::TListaPunktow ZebranePunkty;      // Lista zebranych pol rezonansowych
//extern Symbol* Punkt::Ksztalt = 0;       // "statyczny" kszta³t zebranych punków.

//ZbiorTablic* DajZb(int);

TPoint TJWykresyLinii::DajWsp(double B,double I)  // B jest indeksem tablicy zawieraj¹cej pola
{
  int wysokosc=DajRozmiarPl().y; //
  popr =int(wysokosc*0.02);  // Zmniejszenie o 2% wysokosci
  wysokosc =wysokosc-popr;
  ZbiorTablic *Akt=DajZb(NumerPliku);
  WspX = double(DajRozmiarPl().x)/Akt->LiczWierszy();
  ::WspX=WspX;
  int x_e,y_e;
  x_e=int(WspX*B);
  double WspY_B = double(wysokosc)/(fabs(MaxWartY)+fabs(MinWartY));
  double WspY_C = WspY_B*MaxWartY;        // Okreœla liniê zerow¹
  y_e=int(WspY_C-I*WspY_B);
  y_e=y_e+popr/2;
  return TPoint(x_e,y_e);
}

double TJWykresyLinii::DajWartX(int X)
{
  ZbiorTablic *Akt=DajZb(NumerPliku);
  double WspX_Ind = double(DajRozmiarPl().x)/Akt->LiczWierszy();
  //::WspX=WspX;
  int Indeks_B = int(X/WspX_Ind);
  return DajX(Indeks_B,NumerPliku);

}

double TJWykresyLinii::DajWartY(int Y)
{
  int wysokosc=DajRozmiarPl().y; //
  popr =int(wysokosc*0.02);  // Zmniejszenie o 2% wysokosci
  wysokosc =wysokosc-popr;
  double WspY_B = double(wysokosc)/(fabs(MaxWartY)+fabs(MinWartY));
  double WspY_C = WspY_B*MaxWartY;        // Okreœla liniê zerow¹
  double I = (WspY_C-Y - popr/2)/WspY_B;
  return I;
}



void Kolo::RysujKolo(TCanvas *Pl)
{
  RysujKoloN(Pl);
  return;
 /* Inicjuj(Pl);
  int x1=Srodek.x-rozmiar;
  int y1=Srodek.y-rozmiar;
  int x2=Srodek.x+rozmiar;
  int y2=Srodek.y+rozmiar;
  Pl->Ellipse(x1,y1,x2,y2);
  Zwolnij();  */
}

void Kolo::RysujKoloN(TCanvas *Pl)
{
  Inicjuj(Pl);
  int x1=Srodek.x-rozmiar;
  int y1=Srodek.y-rozmiar;
 // int x2=Srodek.x+rozmiar;
 //int y2=Srodek.y+rozmiar;
  if(Odswierz)    //budowanie mapy bitowej symbolu
  {
     if(KanwaSym) delete KanwaSym;
     KanwaSym = new Graphics::TBitmap();
     KanwaSym->Width=2*rozmiar;

     KanwaSym->Height=2*rozmiar;
     KanwaSym->Canvas->Pen->Color=KolorLinii;
     KanwaSym->Canvas->Brush->Color=KolorWyp;
    //  Pl->Ellipse(x1,y1,x2,y2);
    KanwaSym->Canvas->Ellipse(0,0,2*rozmiar,2*rozmiar);
    Odswierz=false;
  }
  Pl->CopyMode=cmSrcAnd;
  Pl->Draw(x1,y1,KanwaSym);
  Zwolnij();
}
void Kwadrat::RysujKwadrat(TCanvas *Pl)
{
    RysujKwadratN(Pl);
    return;
 /*   int Bok=rozmiar;
    Inicjuj(Pl);
    int x1=Srodek.x-Bok/2;
    int y1=Srodek.y-Bok/2;
    int x2=Srodek.x+Bok/2;
    int y2=Srodek.y+Bok/2;
    Pl->Rectangle(x1,y1,x2,y2);
    Zwolnij();
 */
}

void Kwadrat::RysujKwadratN(TCanvas *Pl)
{
  //  int Bok=rozmiar;
    Inicjuj(Pl);
    int x1=Srodek.x-rozmiar;
    int y1=Srodek.y-rozmiar;
    if(Odswierz)    //budowanie mapy bitowej symbolu
    {
       if(KanwaSym) delete KanwaSym;
       KanwaSym = new Graphics::TBitmap();
       KanwaSym->Width=2*rozmiar;
       KanwaSym->Height=2*rozmiar;
       KanwaSym->Canvas->Pen->Color=KolorLinii;
       KanwaSym->Canvas->Brush->Color=KolorWyp;
       KanwaSym->Canvas->Rectangle(0,0,2*rozmiar,2*rozmiar);
       Odswierz=false;
    }
    Pl->CopyMode=cmSrcAnd;
    Pl->Draw(x1,y1,KanwaSym);
    Zwolnij();
}


extern StructOpcje Opcje;
void TJWykresyPunktow::inicjuj()
   {
    Czulosc=10; Dane=0; ZaznSymbol=0;
    AktSymbol= Form1->Zaznaczenie;
    NumerPrzejscia=0;
    AktualnaLista=Przejscia.begin();
    otwarty=false;
   }



TJKonwersje konwersja_pkt;

void TJWykresyPunktow::Update(int r=LiczbaZebranychPolozen)
{

  if(StareDane && Dane){
    for (int i=0;i<4;i++)
        delete[] StareDane[i];
     StareDane=Dane;
     Stary_rozmiar=rozmiar;
  }
  else
  {

     StareDane =  new double*[4];
     for(int i=0; i<4 ; i++) //Tworzymy Miejsce na Dane
          StareDane[i]=new double[r];
          Stary_rozmiar=r;
  }
  Dane = new double*[4];
  for(int i=0; i<4 ; i++) //Tworzymy Miejsce na Dane
   Dane[i]=new double[r];
    rozmiar = r;

   float dotych, KolumnaX;
   int NrPliku;
    int j=0;     // indeks wczytanych punktow
   MaxWartY=MaxWartX=   -MAXDOUBLE;
   MinWartY=MinWartX=    MAXDOUBLE;
   double PoleT, WartoscT;
   #ifdef odpluskwianie
   Form1->Memo1->Lines->Add("Wart\tPole\tNr Pliku");
   String TymStr;
   #endif
   ObszarPunkty::Punkt* Pkt;
   while((Pkt=ZebranePunkty.DajNastepny())!=0 )
   {
     PoleT =Pkt->DajPole();
     WartoscT = Pkt->DajWartosc();
     NrPliku =  Pkt->DajNumerPliku();

     if(MinWartY>PoleT)MinWartY=PoleT;
     if(MaxWartY<PoleT)MaxWartY=PoleT;
     if(MinWartX>WartoscT)MinWartX=WartoscT;
     if(MaxWartX<WartoscT)MaxWartX=WartoscT;

     Dane[0][j]= WartoscT ;
     Dane[1][j]= PoleT ;
     Dane[2][j]= NrPliku;
     Dane[3][j]= 0;
                     // 0 - oznacza, ze dany punkt nie zostal jeszcze zaznaczony
                     // inna - ile razy ju¿ by³ wybrany
     j++ ;
   }
   UstawRozmiarPl(Sztaluga);
    if(ListaPrzejsc.DajLiczbeZdefiniowanychPrzejsc()!=0)
    {
           ObszarPunkty::Przejscie* Przej = ListaPrzejsc.DajPrzejscie(NumerPrzejscia);
           ZaznSymbol = Przej->DajKsztalt();
    }

//*************************  Stary Kod *****************************************
/*
   j=0
   using namespace PrzestrzenOpcji;
   Punkty* tym;
   typedef float (TJKonwersje::*Tfun)(float);
   Tfun fun;
   // Ustawianie odpowiednich wartosci odcietych (X-sów)
   switch(Opcje.ZapisywaneDane)  // fun zawiera wskaŸnik do odpowiedniej funkcji konwersji
      {
      case TemperaturaK : fun=&TJKonwersje::TempKelvin;//(double);
                          break;
      case TemperaturaC :  fun=&TJKonwersje::TempCelsjusz;
                          break;
      default           :  fun=&TJKonwersje::PrzepDouble; //dotych;   // Dla k¹tów podajemy odczytan¹ wartoœæ
                          break;
      }
   for(int i=0; i< LiczbaWczytPlikow;i++)
   {
     NrPliku=TablicaNrPlikow[i];
     ZbiorTablic* Akt=DajZb(NrPliku);

     if((tym=Akt->Pola[Akt->DajIndeks(NrPliku)].nastepny)!=0)
     {
        dotych=static_cast<float>(Akt->DajWartosc(NrPliku));
        KolumnaX=(konwersja_pkt.*fun)(dotych);
        while(tym)
         {
           Dane[0][j]=KolumnaX;
           Dane[1][j]=DajX(tym->indeks,NrPliku);
           if(MinWartY>Dane[1][j])MinWartY=Dane[1][j];
           if(MaxWartY<Dane[1][j])MaxWartY=Dane[1][j];
           Dane[2][j]=NrPliku;
           Dane[3][j]=0;        // 0 - oznacza, ze dany punkt nie zostal jeszcze zaznaczony
                                // inna - ile razy ju¿ by³ wybrany
           #ifdef odpluskwianie
           TymStr.printf("%.3f\t%.3f\t%.0f",Dane[0][j],Dane[1][j],Dane[2][j]);
           Form1->Memo1->Lines->Add(TymStr);
           #endif
           j++;
           tym=tym->nastepny;
         }

     }
   }
   MaxWartX=Dane[0][j-1];
   MinWartX=Dane[0][0];
   UstawRozmiarPl(Sztaluga);
   map<int,int>  Porownanie;
   //Tutaj wstawic porownanie starych i nowych danych.
   for(int i=0;i<rozmiar;i++)
     for(int j=0;j<Stary_rozmiar;j++)
     {
        if(Dane[1][i]==StareDane[1][j] && Dane[2][i]==StareDane[2][j])
            if(i!=j)  Porownanie[j]=i;
     }
   map<int,int>::const_iterator it;
   Form1->Memo1->Lines->Add("Stary Indeks \t \t Nowy indeks");
   for(it=Porownanie.begin();it!=Porownanie.end();it++)
   {
     Form1->Memo1->Lines->Add(AnsiString(it->first)+AnsiString("  ")+AnsiString(it->second));
   }
*/
// ************************* Stary Kod *******************************************
}


void  TJWykresyPunktow::rysuj()
{
try{
  otwarty=true; //Oznacza to, ¿e wykres punktów by³ ju¿ wyrysowywany
  TCanvas *MBCanv;
  OdswierzBitMap();
  if(!AktSymbol)
        AktSymbol = new Kwadrat;
//  Punkt::Ksztalt = AktSymbol;
  Symbol* SymPkt;
  if((SymPkt=ObszarPunkty::Punkt::DajSymbol())!=0 )
  {
     UstawSymbolPunktow(SymPkt);
  }
  else
  {
      ObszarPunkty::Punkt::UstawSymbol(AktSymbol);
  }
  if(!ZaznSymbol)
        ZaznSymbol = new Kolo;
  //MBCanv=B->Canvas;
  strstream ekran;
  AnsiString Pan;
  Pan.printf("Zazn = %d",LiczbaZebranychPolozen);
  TPoint Punkt;
  //AktSymbol->UstKolorLinii(clBlack);
  int SymPol,SymPrz;
  if((SymPol=WykresPktForm->SymbolePol->Text.ToInt()) <1 || SymPol>100) SymPol=4;
  if((SymPrz=WykresPktForm->SymbolePrzejsc->Text.ToInt()) <1 || SymPrz>100) SymPrz=6;
  //AktSymbol->UstRozmiar(SymPol);
 // ZaznSymbol->UstRozmiar(SymPrz);
  std::list<int>::iterator iterWew;
  std::list<ListSklad>::iterator iter;
  iter=Przejscia.begin();
/*
   while(iter!=Przejscia.end())
   {
     iterWew=iter->punkty.begin();
     ZaznSymbol->UstKolorLinii(iter->Kolor);
     while(iterWew!=iter->punkty.end())
     {
     Punkt= DajWsp(Dane[0][*iterWew],Dane[1][*iterWew]);
     ZaznSymbol->Rysuj(B->Canvas,Punkt.x,Punkt.y);
     ZaznSymbol->UstKolorLinii(iter->Kolor);
     iterWew++;
     }
     iter++;
   }

  for(int i=0;i< rozmiar;i++)
  {
   Punkt= DajWsp(Dane[0][i],Dane[1][i]);
   AktSymbol->Rysuj(B->Canvas,Punkt.x,Punkt.y);
  }
*/

  ObszarPunkty::Punkt* P_ptr;
  ObszarPunkty::Przejscie* Prz_ptr;
  int NrPrzej;
  AnsiString  T;
  FormTest->Memo1->Lines->Add(T);
  Symbol*  Sym;
  while( (P_ptr=ZebranePunkty.DajNastepny())!=0 )          //Wartosc = wczytane z pliku T lub Kat
  {
       Punkt= DajWsp(P_ptr->DajWartosc(),P_ptr->DajPole());
       //Sym=Punkt::DajSymbol();
       //Sym->Rysuj(B->Canvas,Punkt.x,Punkt.y);
       AktSymbol->Rysuj(B->Canvas,Punkt.x,Punkt.y);
     //  T="Wejscie do Pêtli";
     //  FormTest->Memo1->Lines->Add(T);
      // std::list<int>::iterator it;
      ObszarPunkty::Punkt::TIntIter it;
       for(it=P_ptr->Przejscia.begin();it!=P_ptr->Przejscia.end();it++)
        {
                NrPrzej = *it;
                Prz_ptr = ListaPrzejsc.DajPrzejscie(NrPrzej);
                Sym=Prz_ptr->DajKsztalt();
                Sym->Rysuj(B->Canvas,Punkt.x,Punkt.y);
        }

    /*
       while((NrPrzej=P_ptr->DajNastPrzejscie())!=0)
       {
           T="Numer Przejscia = ";
           T+=NrPrzej;
           FormTest->Memo1->Lines->Add(T);
           Prz_ptr = ListaPrzejsc.DajPrzejscie(NrPrzej);
           Sym=Prz_ptr->DajKsztalt();
           Sym->Rysuj(B->Canvas,Punkt.x,Punkt.y);
       }
    */
  }

  //***************************** WA¯NE  *************************
  WykresPktForm->PaintBoxPkt->Canvas->Draw(0,0,B);
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
 catch(ZakresNap Z)
 {
   Application->MessageBox("Przekroczono Zakres Mo¿liwych Napiêæ /n"
                        "Spróbuj zmieniæ opcje zapisu danych","Uwaga",ID_OK);
   Form1->Opcje1Click(Form1);
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
    Application->MessageBox("Nie wiadomo co w funkcji TJWykresyPunktow::rysuj()",
                              "Uwaga!!",MB_OK);
 }
}


TParaOgl<int> DajWsp(double X,double Y,TJWykresyPunktow& P)
{
  int x_e,y_e;
  x_e=int((X - P.MinWartX)*P.WspX);
  y_e= P.wys -int((Y-P.MinWartY)*P.WspY);
  return TParaOgl<int>(x_e+P.dX,y_e+P.dY);
}




TPoint TJWykresyPunktow::DajWsp(double X,double Y)
{
  int x_e,y_e;
  x_e=int((X - MinWartX)*WspX);
  y_e=wys-int((Y-MinWartY)*WspY);
  return TPoint(x_e+dX,y_e+dY);
}

static inline double Odl(int X1, int Y1, int X2, int Y2)
{
 return  sqrt((X1-X2)*(X1-X2)+(Y1-Y2)*(Y1-Y2));
}

int TJWykresyPunktow::SzukajNrPliku(int X, int Y)   // zwraca -1 je¿eli nie znaleziono
{
  double XP,YP;
  double delX, delY,delta,tym;
  delta = MaxInt;
  int indeks=-1;
  delX=(Czulosc*2)/WspX;
  delY=(Czulosc*2)/WspY;
  X-=dX;  Y-=dY;                // uwzglêdnienie poprawek zwracanych przez DajWsp.
  XP=float(X)/WspX + MinWartX;
  YP= double(wys-Y)/WspY + MinWartY;
  String TymStr;
  TymStr.printf("XP = %.4f \t YP = %.4f",XP, YP);
  Form1->Memo1->Lines->Add(TymStr);
  TymStr.printf("Czu³oœæ = %d",Czulosc);
  Form1->Memo1->Lines->Add(TymStr);
  for(int i=0;i<rozmiar;i++)
       if(Dane[0][i]>=XP-delX && Dane[0][i]<=XP+delX &&
          Dane[1][i]>=YP-delY && Dane[1][i]<=YP+delY)
          {
             tym=sqrt((Dane[0][i]-XP)*(Dane[0][i]-XP)+(Dane[1][i]-YP)*(Dane[1][i]-YP));
             if(tym<delta)
             {
                   delta=tym;
                   indeks=i;
             }
          }
 if(indeks>=0) Form1->Memo1->Lines->Add(Dane[2][indeks]);
 if(indeks>=0) return Dane[2][indeks];
 else return indeks;
}

double TJWykresyPunktow::DajWartX(int X)
    {
     X-=dX;                  // uwzglêdnienie poprawek zwracanych przez DajWsp.
     return double(X)/WspX + MinWartX;
    }

double TJWykresyPunktow::DajWartY(int Y)
    {
      Y-=dY;
      return double((wys-Y))/WspY + MinWartY;
    }

int TJWykresyPunktow::SzukajNajblIndeksu(int X, int Y)
{
  double XP,YP;
  double delX, delY,delta,tym;
  delta = MaxInt;
  int indeks=-1;
  delX=(Czulosc*2)/WspX;
  delY=(Czulosc*2)/WspY;
  X-=dX;  Y-=dY;                // uwzglêdnienie poprawek zwracanych przez DajWsp.
  XP=float(X)/WspX + MinWartX;
  YP=double(wys-Y)/WspY + MinWartY;
  for(int i=0;i<rozmiar;i++)
       if(Dane[0][i]>=XP-delX && Dane[0][i]<=XP+delX &&
          Dane[1][i]>=YP-delY && Dane[1][i]<=YP+delY)
          {
             tym=sqrt((Dane[0][i]-XP)*(Dane[0][i]-XP)+(Dane[1][i]-YP)*(Dane[1][i]-YP));
             if(tym<delta)
             {
                   delta=tym;
                   indeks=i;
             }
          }
   return indeks;
}

void TJWykresyPunktow::ZapisParametrowWykresu(std::ofstream& zapisz_do)
{
       if(ListaPrzejsc.DajLiczbeZdefiniowanychPrzejsc()==0)
                        return;

       zapisz_do<<"[Parametry_Wykresu_Pól_Rezonansowych]"<<endl;
       zapisz_do<< NumerPrzejscia << "\t" << WykresPktForm->Efrq->Text.c_str()<<'\t'
                << WykresPktForm->RadioZaznWiel->Checked<<'\t'
                << WykresPktForm->NBlock->Text.c_str()<<'\t'
                <<WykresPktForm->ETheta->Text.c_str()<<'\t'
                <<WykresPktForm->EPhi->Text.c_str()<<'\t'
                <<WykresPktForm->StalyKat->ItemIndex
                 ;
}


int TJWykresyPunktow::OdczytParametrowWykresu(std::ifstream& wczytaj_z)
{
    char fake;
    wczytaj_z>>fake;
    if(fake!='[')
    {
        wczytaj_z.putback(fake);
        return 0;
    }
    wczytaj_z.putback(fake);
    std::string zakladka;
    wczytaj_z>>zakladka;
    if(zakladka.compare("[Parametry_Wykresu_Pól_Rezonansowych]")) // compare daje 0 gdy napisy sa rowne
    {
      for(unsigned int i=0;i<zakladka.size();i++)
      {
        fake=zakladka[i];
        wczytaj_z.putback(fake);
      }
        return 0;
    }
    std::string frq, NrBloku, Theta, Phi;
    int indeks;
    bool czy_wiel;
    wczytaj_z>> NumerPrzejscia >> frq
             >> czy_wiel
             >>NrBloku >> Theta >> Phi
             >>indeks
             ;
    WykresPktForm->ETheta->Text = Theta.c_str();
    WykresPktForm->EPhi->Text = Phi.c_str();
    WykresPktForm->Efrq->Text = frq.c_str();
    WykresPktForm->NBlock->Text = NrBloku.c_str();
    WykresPktForm->StalyKat->ItemIndex=indeks;
    WykresPktForm->RadioZaznWiel->Checked=czy_wiel;
    return 1;  // Means wszystko OK.
}


double TJWspolnyWykres::Odleglosc(int X1, int Y1, int X2, int Y2)
{
        return sqrt( (X1-X2)*(X1-X2) + (Y1-Y2)*(Y1-Y2) );
}


void Symbol::Wypisz(std::ofstream& wyp_do)
{

  //Symbol* NowySymbol(std::string nazwa)   funkcja na podastwie nazwy ustala typ
  //dziedziczony dlatego nie ma zapisu NazwySymbolu
       wyp_do << rozmiar << '\t' <<
                static_cast<int>(KolorLinii) << '\t' << static_cast<int>(KolorWyp);

}

void Symbol::Wczytaj(std::ifstream& wczyt_z)
{
    int K1, K2;
    wczyt_z >> rozmiar >> K1 >> K2;
    /* TODO : Usun¹æ testowy kod poni¿ej */
    AnsiString T = "Rozmiar = ";
    T+=rozmiar; T+="  Kolor1 "; T+=K1;
    T+="  Kolor 2  "; T+=K2;
    FormTest->Memo1->Lines->Add(T);
    KolorLinii  = static_cast<TColor>(K1);
    KolorWyp    = static_cast<TColor>(K2);
}


//  Odczyt typu ze zbioru z dysku (pole typu) i zwrocenie odpowiedniego wskaznika

std::map<std::string, int> NazwySymboli; // Zainicjowane w konstruktorze FormularzaWykresuPunktowego

void InicjujNazwySymboli()
{
    NazwySymboli["Kolo"]=0;
    NazwySymboli["Kwadrat"]=1;
}

Symbol* NowySymbol(std::string nazwa)
{

    int Wybor = NazwySymboli[nazwa];
    switch (Wybor)
    {
      case 0: return new Kolo();
      case 1: return new Kwadrat();

      default: return 0 ;             // Nieznana wartosc
    }
}
