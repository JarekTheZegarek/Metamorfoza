//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ksztalt.h"
#include "metamorfozis_w5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

TPoint ProstaStart;
TPoint StaraProsta;
static bool start_prosta=false;
static bool zaczynaj_zazn=false;
static double start=0;
static double stop=0;
static startIndeks=0;
static stopIndeks=0;
static float SzerPP;
struct ProstaKsztalt {
    double a;
    double b;
};

ProstaKsztalt LLorentz,LGauss, PLorentz,PGauss;

extern bool rysunek;
static int indeks0;

class AktWsp{
    TPaintBox* MBit;
    double A;
    double B ;
    double C ;
    double D;
    double minX,maxX,minY, maxY;
    int Rama;
public:
    bool CzyWsp;
    AktWsp():Rama(35),CzyWsp(false){};
    void ZmienWsp(){CzyWsp=false;}
    void UstRame(int R){if(Rama<=60 && Rama>0) Rama=R;  } //ograniczenia ramki
    int Xe(double Xrz);
    int Ye(double Yrz);
    double Xrz(int Xe),Yrz(int Ye);
    void OblWsp(double MinX, double MaxX, double MinY, double MaxY, TPaintBox *M);
    void OblWsp();
};

void AktWsp::OblWsp(double MinX, double MaxX, double MinY, double MaxY, TPaintBox *M )
{
  MBit=M;
  minX=MinX; maxX=MaxX; minY=MinY; maxY=MaxY;
  A=(MBit->Width-2*Rama)/(MaxX-MinX);
  B=-A*MinX;
  C=(MBit->Height-2*Rama)/(MinY-MaxY);
  D=-C*MaxY;
  CzyWsp=true;
}

void AktWsp::OblWsp()
{
  A=(MBit->Width-2*Rama)/(maxX-minX);
  B=-A*minX;
  C=(MBit->Height-2*Rama)/(minY-maxY);
  D=-C*maxY;
  CzyWsp=true;
}

double AktWsp::Xrz(int Xe)
{
 double X;
 //Xe=Xe-Rama;
 if(!CzyWsp) OblWsp();
 if(Xe<Rama)
   return minX;
 X=(Xe-B-Rama)/A;
 if (X>maxX)
   return maxX;
 else
   return X;
}

double AktWsp::Yrz(int Ye)
{
  double Y;
  //Ye=Ye-Rama;
  if(!CzyWsp) OblWsp();
  if(Ye<Rama)
        return minY;
  Y=(Ye-C-Rama)/D;
  if (Y>maxY)
        return maxY;
  else
        return Y;
}

int AktWsp::Xe(double Xrz)
{
  if(!CzyWsp) OblWsp();
    return A*Xrz+B + Rama;
}

int AktWsp::Ye(double Yrz)
{
  if(!CzyWsp) OblWsp();
    return C*Yrz+D+Rama;
}

static  AktWsp Wsp;
bool* TabKsztalt_L=0;
double* Tab[3];

void TForm3::UstawInd()
{
  ZbiorTablic *Akt = DajZb(NumerPliku);
  Punkty* tym=Akt->Pola[Akt->DajIndeks(NumerPliku)].nastepny;
  int i=0;
  if(stop!=0 || start!=0)
  {
    while(DajX(i,NumerPliku)<start)
    {
      i++;
    }
    startIndeks=--i;
    while(DajX(i,NumerPliku)<stop)
    {
      i++;
    }
    stopIndeks=i;
  }
  else
  {
   startIndeks=0;
   stopIndeks=Akt->LiczWierszy();
  }

}

bool TForm3::sprawdz()
{
  ZbiorTablic *Akt = DajZb(NumerPliku);
  Punkty* tym=Akt->Pola[Akt->DajIndeks(NumerPliku)].nastepny;
  if(!tym) return false;
  if(tym->nastepny) return false;
   double max = MINDOUBLE;
  double min = MAXDOUBLE;
  Akt=DajZb(NumerPliku);
  UstawInd();
  double tymX;
  for(int i=startIndeks;i<stopIndeks;i++)
  {
       tymX = DajX(i,NumerPliku);
       if (tymX>max) max = tymX;
       if (tymX<min) min = tymX;
  }
  indeks0=tym->indeks;
  double B0=DajX(tym->indeks,NumerPliku);
  TPoint BB;
  int X1S,X2S,I01,I02;



  BB=Akt->DajSzer(NumerPliku,1);
  if(BB.x>=0 && BB.y >=0)
  {
       X1S= (BB.x<BB.y)?BB.x:BB.y ;
       X2S= (BB.x>BB.y)?BB.x:BB.y ;
       SzerPP= DajX(X2S,NumerPliku) - DajX(X1S,NumerPliku);
       I01=fabs(DajY(X1S,NumerPliku));      // lewa amplituda linii
       I02=fabs(DajY(X2S,NumerPliku));      //  prawa amplituda linii
  }
  double B,Czyn,Amp,Lor,Gaus,I;
  if(TabKsztalt_L) delete TabKsztalt_L; // je¿eli ju¿ zainicjowano tablicê
  TabKsztalt_L = new bool[Akt->LiczWierszy()+1];
  TPoint Punkt;
  for(int i=0;i<3;i++ )
  {
        Tab[i]=new double[Akt->LiczWierszy()+1];
  }
      //for(int i=1;i<Akt->LiczWierszy();i++)
      for(int i=startIndeks+1;i<tym->indeks;i++)    //LEWA STRONA
        {
         I =DajY(i, NumerPliku);
         I=fabs(I);
         Amp= fabs(I/I01);
         B=DajX(i,NumerPliku);
         I+=1;   //Przesuniêcie ze wzglêdu na zero.
         Tab[0][i]=(B-B0)*(B-B0);
         Tab[1][i]=sqrt((B0-B)/I);
         Tab[2][i]=log(I/(B0-B));
         Czyn=fabs(B0-B)/SzerPP;
         Lor=(16.0/9)*Czyn*pow((1+4*Czyn*Czyn/3.0),-2); //Teoretyczny Stosunek Amplitud
         Gaus=pow(exp(1),0.5)*Czyn*exp(-2*Czyn*Czyn);
         Lor=fabs(Lor);
         Gaus=fabs(Gaus);
         TabKsztalt_L[i]=(((Gaus-Amp)*(Gaus-Amp)) > ((Lor-Amp)*(Lor-Amp)))?true:false;
        }
        for(int i=tym->indeks+1;i<stopIndeks;i++)             //Prawa Strona
        {
         I =DajY(i, NumerPliku);
         I=fabs(I);
         Amp= fabs(I/I02);
         B=DajX(i,NumerPliku);

         Czyn=fabs(B0-B)/SzerPP;
         Lor=(16.0/9)*Czyn*pow((1+4*Czyn*Czyn/3.0),-2); //Teoretyczny Stosunek Amplitud
         Gaus=pow(exp(1),0.5)*Czyn*exp(-2*Czyn*Czyn);
         Lor=fabs(Lor);
         Gaus=fabs(Gaus);
         TabKsztalt_L[i]=(((Gaus-Amp)*(Gaus-Amp)) > ((Lor-Amp)*(Lor-Amp)))?true:false;
         Tab[0][i]=(B-B0)*(B-B0);
       //  if(I==0) I=0.1;
         I+=1;                   //+1  przesuniêcie     ze wzglêdu na zero
         Tab[1][i]=sqrt((B-B0)/I);
         Tab[2][i]=log(I/(B-B0));
        }
        Tab[0][0]=B0;
        LLorentz.b=(3.0/4)*sqrt(SzerPP/I01);
        LLorentz.a=pow(SzerPP,-1.5)/sqrt(I01);
        LGauss.a=-2.0/(SzerPP*SzerPP);
        LGauss.b=log(I01*sqrt(exp(1))/SzerPP);
        PLorentz.b=(3.0/4)*sqrt(SzerPP/I02);
        PLorentz.a=pow(SzerPP,-1.5)/sqrt(I02);
        PGauss.a=-2.0/(SzerPP*SzerPP);
        PGauss.b=log(I02*sqrt(exp(1))/SzerPP);
  return true;
}

void TForm3::rysuj()
{

try{

  if(!rysunek) return;
  UstawInd();
  ZbiorTablic *Akt = ZbiorTablic::glowa;
  Akt=DajZb(NumerPliku);
  int Szer= PaintBox1->Width ;
  TCanvas *MBCanv;
  TCanvas *IMCanv=PaintBox1->Canvas;
  Graphics::TBitmap* MBit = new Graphics::TBitmap();
  MBCanv=MBit->Canvas;
  strstream ekran;
  static AnsiString Belka=Form1->Caption;
  char filename[_MAX_FNAME];
  char fileextension[_MAX_EXT];
  char path[_MAX_PATH];
  MBit->Height=PaintBox1->Height;
  MBit->Width=PaintBox1->Width;
  if(AktWykrLinii)
    { delete AktWykrLinii;
    }
  AktWykrLinii= new TJWykresyLinii(MBit);
  AktWykrLinii->UstawKolorLinii(clRed);
  AktWykrLinii->UstZakrZazn(10);
  _splitpath(DajNazweZbioru(NumerPliku).c_str(),0,0,filename,fileextension);
  _makepath(path,0,0,filename,fileextension);
  ekran<<Belka.c_str()<<"   ";
  ekran<<"Aktualnie wybrany plik: "<<filename<<fileextension;
  ekran<<" numer "<<NumerPliku;
  ekran<<'\0'; ekran.seekp(0);
  Form3->Caption=ekran.str();
  double max = MINDOUBLE;
  double min = MAXDOUBLE;

  Akt=DajZb(NumerPliku);
 // if(start==0) start=DajY(0,NumerPliku);
  double XX;
  for(int i=startIndeks;i<stopIndeks;i++)
  {
        double tym = DajY(i,NumerPliku);
        if (tym>max) max = tym;
        if (tym<min) min = tym;
  }
  AktWykrLinii->UstMaxMinY(max,min);
  double MaxX=DajX(stopIndeks-1,NumerPliku);
  double MinX=DajX(startIndeks,NumerPliku);

  /*  double A,B,C,D;
A=(MBit->Width)/(MaxX-MinX);
2  B=-A*MinX;
  C=MBit->Height/(min-max);
  D=-C*max;
  Wsp.A=A; Wsp.B=B; Wsp.C=C; Wsp.D=D; */
  Wsp.OblWsp(MinX,MaxX,min,max,PaintBox1);


  TPoint Punkt; //AktWykrLinii->DajWsp(startIndeks+1,DajY(startIndeks+1,NumerPliku));
  Punkt.x= Wsp.Xe(DajX(startIndeks+1,NumerPliku));      //A*DajX(startIndeks+1,NumerPliku)+B;
  Punkt.y=Wsp.Ye(DajY(startIndeks+1,NumerPliku));//C*DajY(startIndeks+1,NumerPliku)+D;
  MBit->Canvas->MoveTo(Punkt.x,Punkt.y);
  if(!TabKsztalt_L) return; // je¿eli nie okreslono ksztaltu linii
  for(int i=startIndeks+1;i<stopIndeks;i++)
  {
  // Punkt= AktWykrLinii->DajWsp(i,DajY(i,NumerPliku));
  Punkt.x=Wsp.Xe(DajX(i,NumerPliku));
   Punkt.y=Wsp.Ye(DajY(i,NumerPliku));
   if(TabKsztalt_L[i])
     AktWykrLinii->UstawKolorLinii(clGreen);
     //MBit->Canvas->Pen->Color=clGreen; //Zielona linia lorentzowska
   else
     AktWykrLinii->UstawKolorLinii(clBlue);
     //MBit->Canvas->Pen->Color=clBlue;  // Niebieska linia gausowska
   MBit->Canvas->LineTo(Punkt.x,Punkt.y);
  }

  //AnsiString T, T_Kelvin;
  //TPoint BB;
  //int X1S,X2S;
  //float SzerB;
  //Punkty* tym=Akt->Pola[Akt->DajIndeks(NumerPliku)].nastepny;
  /*switch(Tryb)
  {
   case ZaznaczaniePolRezonansowych:
   case Nieokreslony:
   case ZaznaczanieSzerokosci:

            while(tym)         // Odœwie¿anie zaznaczonych punktów
            {
               if(tym->indeks < 0 && tym->indeks >= Akt->LiczKolumn())
                                       throw ErrorZakres(tym->indeks) ;
               Punkt= AktWykrLinii->DajWsp(tym->indeks,DajY(tym->indeks,NumerPliku));
               Form1->Zaznaczenie->Rysuj(MBit->Canvas,Punkt.x,Punkt.y);
               tym=tym->nastepny;
            }
  //          break;
 //  case ZaznaczanieSzerokosci:
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
               //  if(Opcje.PokazPunkty) Doswiadczalny->Rysuj(MBit->Canvas,Punkt.x,Punkt.y);
                 MBit->Canvas->LineTo(Punkt.x,Punkt.y);
                }
                AktWykrLinii->UstawKolorLinii(clRed);
                SzerB= DajX(X2S,NumerPliku) - DajX(X1S,NumerPliku);
                T.printf("Szerokosc = %g",SzerB);
                          StatusBar1->Panels->Items[2]->Text=T;
            }
            if(BB.x>=0)
             { Punkt= AktWykrLinii->DajWsp(BB.x,DajY(BB.x,NumerPliku));
              Form1->Zaznaczenie->Rysuj(MBit->Canvas,Punkt.x,Punkt.y);}
            if(BB.y>=0)
             { Punkt= AktWykrLinii->DajWsp(BB.y,DajY(BB.y,NumerPliku));
              Form1->Zaznaczenie->Rysuj(MBit->Canvas,Punkt.x,Punkt.y);
              }
             if(BB.x <0 || BB.y <0)
             {
                 T.printf("Szerokosc = %g",0);
                          StatusBar1->Panels->Items[2]->Text=T  ;
             }
            break;

  }
  */
  using namespace PrzestrzenOpcji;

 /* float f =Akt->DajWartosc(NumerPliku);
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
                             Form1->Opcje1Click(this);
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
                          T.printf("");
                          StatusBar1->Panels->Items[2]->Text=T;
                          StatusBar1->Panels->Items[4]->Text="";
                         break;
         default : ;
                        break;
  }
  */


  /*if(FormularzOpcjiPrezentacji->CheckBox1->Checked)
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
  */
  PaintBox1->Canvas->Draw(0,0,MBit);
  delete MBit;
  sprawdz();
 // LabelDane->Caption=T;
 // LabelDane->BringToFront();
 //     if(PrzycPoziomZaznaczania->Down)
 //        Form1->RysujPoziomZaznaczania();
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


void TForm3::rysujLL()
{

try{

  if(!rysunek) return;
  UstawInd();
  ZbiorTablic *Akt = ZbiorTablic::glowa;
  Akt=DajZb(NumerPliku);
  int Szer= PaintBox1->Width ;
  TCanvas *MBCanv;
  TCanvas *IMCanv=PaintBox1->Canvas;
  Graphics::TBitmap* MBit = new Graphics::TBitmap();
  MBCanv=MBit->Canvas;
  strstream ekran;
  static AnsiString Belka=Form1->Caption;
  char filename[_MAX_FNAME];
  char fileextension[_MAX_EXT];
  char path[_MAX_PATH];
  MBit->Height=PaintBox1->Height;
  MBit->Width=PaintBox1->Width;
  _splitpath(DajNazweZbioru(NumerPliku).c_str(),0,0,filename,fileextension);
  _makepath(path,0,0,filename,fileextension);
  ekran<<filename<<fileextension;
  ekran<<" numer "<<NumerPliku;
  ekran<<" Analiza Liniowa Kszta³tu - Lorentz Lewa"; ekran.seekp(0);
  Form3->Caption=ekran.str();
  double max = MINDOUBLE;
  double min = MAXDOUBLE;

  Akt=DajZb(NumerPliku);
  for(int i=1+startIndeks;i<indeks0;i++)
  {
        double tym = Tab[1][i];
        if (tym>max) max = tym;
        if (tym<min) min = tym;
  }
  double MaxX,MinX;
  MaxX=Tab[0][startIndeks+1];
  MinX=Tab[0][indeks0-1];

  Wsp.OblWsp(MinX,MaxX,min,max,PaintBox1);
  Graphics::TBitmap* M=new Graphics::TBitmap();
  MBCanv=M->Canvas;
  M->Width=6;
  M->Height=6;
  MBCanv->Pen->Color=clGreen;
  MBCanv->Brush->Color=clGreen;
  MBCanv->Ellipse(0,0,6,6);

  TPoint Punkt = TPoint(Wsp.Xe(Tab[0][startIndeks+1]),Wsp.Ye(Tab[1][startIndeks+1]));
  MBit->Canvas->MoveTo(Punkt.x,Punkt.y);
  for(int i=startIndeks+2;i<indeks0;i+=UpDown1->Position)
  {
   Punkt.x=Wsp.Xe(Tab[0][i]);
   Punkt.y=Wsp.Ye(Tab[1][i]);
   MBit->Canvas->Draw(Punkt.x,Punkt.y,M);             //MBit->Canvas->LineTo(Punkt.x,Punkt.y);
  }
  MBit->Canvas->MoveTo(Wsp.Xe(MinX),Wsp.Ye(LLorentz.a*MinX+LLorentz.b));
  MBit->Canvas->Pen->Width=2;
  MBit->Canvas->Pen->Color=clRed;
  MBit->Canvas->LineTo(Wsp.Xe(MaxX),Wsp.Ye(LLorentz.a*MaxX+LLorentz.b));
  PaintBox1->Canvas->Draw(0,0,MBit);
  delete MBit;
  delete M;
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



void TForm3::rysujGL()
{

try{

  if(!rysunek) return;
  UstawInd();
  ZbiorTablic *Akt = ZbiorTablic::glowa;
  Akt=DajZb(NumerPliku);
  int Szer= PaintBox1->Width ;
  TCanvas *MBCanv;
  TCanvas *IMCanv=PaintBox1->Canvas;
  Graphics::TBitmap* MBit = new Graphics::TBitmap();
  strstream ekran;
  char filename[_MAX_FNAME];
  char fileextension[_MAX_EXT];
  char path[_MAX_PATH];
  MBit->Height=PaintBox1->Height;
  MBit->Width=PaintBox1->Width;
  _splitpath(DajNazweZbioru(NumerPliku).c_str(),0,0,filename,fileextension);
  _makepath(path,0,0,filename,fileextension);
  ekran<<filename<<fileextension;
  ekran<<" numer "<<NumerPliku;
  ekran<<" :  Analiza Liniowa Kszta³tu - Gauss Lewa\0"; ekran.seekp(0);
  Form3->Caption=ekran.str();
  double max = MINDOUBLE;
  double min = MAXDOUBLE;

  Akt=DajZb(NumerPliku);
  for(int i=1+startIndeks;i<indeks0;i++)
  {
        double tym = Tab[2][i];
        if (tym>max) max = tym;
        if (tym<min) min = tym;
  }
  double MaxX,MinX;
  MaxX=Tab[0][startIndeks+1];
  MinX=Tab[0][indeks0-1];
  Wsp.OblWsp(MinX,MaxX,min,max,PaintBox1);
  Graphics::TBitmap* M=new Graphics::TBitmap();
  MBCanv=M->Canvas;
  M->Width=6;
  M->Height=6;
  MBCanv->Brush->Color=clBlue;
  MBCanv->Brush->Color=clBlue;
  MBCanv->Ellipse(0,0,6,6);

  TPoint Punkt = TPoint(Wsp.Xe(Tab[0][startIndeks+1]),Wsp.Ye(Tab[2][startIndeks+1]));
  MBit->Canvas->MoveTo(Punkt.x,Punkt.y);
  for(int i=startIndeks+2;i<indeks0;i+=UpDown1->Position)
  {
   Punkt.x=Wsp.Xe(Tab[0][i]);
   Punkt.y=Wsp.Ye(Tab[2][i]);
   MBit->Canvas->Draw(Punkt.x,Punkt.y,M);             //MBit->Canvas->LineTo(Punkt.x,Punkt.y);
  }
  MBit->Canvas->MoveTo(Wsp.Xe(MinX),Wsp.Ye(LGauss.a*MinX+LGauss.b));
  MBit->Canvas->Pen->Width=2;
  MBit->Canvas->Pen->Color=clRed;
  MBit->Canvas->LineTo(Wsp.Xe(MaxX),Wsp.Ye(LGauss.a*MaxX+LGauss.b));
  PaintBox1->Canvas->Draw(0,0,MBit);
  PaintBox1->Canvas->Draw(0,0,MBit);
  delete MBit;
  delete M;
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


void TForm3::rysujPL()
{

try{

  if(!rysunek) return;
  UstawInd();
  ZbiorTablic *Akt = ZbiorTablic::glowa;
  Akt=DajZb(NumerPliku);
  int Szer= PaintBox1->Width ;
  TCanvas *MBCanv;
  TCanvas *IMCanv=PaintBox1->Canvas;
  Graphics::TBitmap* MBit = new Graphics::TBitmap();
  MBCanv=MBit->Canvas;
  strstream ekran;
  static AnsiString Belka=Form1->Caption;
  char filename[_MAX_FNAME];
  char fileextension[_MAX_EXT];
  char path[_MAX_PATH];
  MBit->Height=PaintBox1->Height;
  MBit->Width=PaintBox1->Width;
  _splitpath(DajNazweZbioru(NumerPliku).c_str(),0,0,filename,fileextension);
  _makepath(path,0,0,filename,fileextension);
  ekran<<filename<<fileextension;
  ekran<<" numer "<<NumerPliku;
  ekran<<" Analiza Liniowa Kszta³tu - Lorentz strona Prawa"; ekran.seekp(0);
  Form3->Caption=ekran.str();
  double max = MINDOUBLE;
  double min = MAXDOUBLE;

  Akt=DajZb(NumerPliku);
  for(int i=1+indeks0;i<stopIndeks;i++)
  {
        double tym = Tab[1][i];
        if (tym>max) max = tym;
        if (tym<min) min = tym;
  }
  double MaxX,MinX;
  MaxX=Tab[0][stopIndeks-1];
  MinX=Tab[0][indeks0+1];

  Wsp.OblWsp(MinX,MaxX,min,max,PaintBox1);
  Graphics::TBitmap* M=new Graphics::TBitmap();
  MBCanv=M->Canvas;
  M->Width=6;
  M->Height=6;
  MBCanv->Pen->Color=clGreen;
  MBCanv->Brush->Color=clGreen;
  MBCanv->Ellipse(0,0,6,6);

  TPoint Punkt = TPoint(Wsp.Xe(Tab[0][indeks0+1]),Wsp.Ye(Tab[1][indeks0+1]));
  MBit->Canvas->MoveTo(Punkt.x,Punkt.y);
  for(int i=indeks0+2;i<stopIndeks;i+=UpDown1->Position)
  {
   Punkt.x=Wsp.Xe(Tab[0][i]);
   Punkt.y=Wsp.Ye(Tab[1][i]);
   MBit->Canvas->Draw(Punkt.x,Punkt.y,M);             //MBit->Canvas->LineTo(Punkt.x,Punkt.y);
  }
  MBit->Canvas->MoveTo(Wsp.Xe(MinX),Wsp.Ye(LLorentz.a*MinX+LLorentz.b));
  MBit->Canvas->Pen->Width=2;
  MBit->Canvas->Pen->Color=clRed;
  MBit->Canvas->LineTo(Wsp.Xe(MaxX),Wsp.Ye(LLorentz.a*MaxX+LLorentz.b));
  PaintBox1->Canvas->Draw(0,0,MBit);
  delete MBit;
  delete M;
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



void TForm3::rysujPG()
{

try{

  if(!rysunek) return;
  UstawInd();
  ZbiorTablic *Akt = ZbiorTablic::glowa;
  Akt=DajZb(NumerPliku);
  int Szer= PaintBox1->Width ;
  TCanvas *MBCanv;
  TCanvas *IMCanv=PaintBox1->Canvas;
  Graphics::TBitmap* MBit = new Graphics::TBitmap();
  strstream ekran;
  char filename[_MAX_FNAME];
  char fileextension[_MAX_EXT];
  char path[_MAX_PATH];
  MBit->Height=PaintBox1->Height;
  MBit->Width=PaintBox1->Width;
  _splitpath(DajNazweZbioru(NumerPliku).c_str(),0,0,filename,fileextension);
  _makepath(path,0,0,filename,fileextension);
  ekran<<filename<<fileextension;
  ekran<<" numer "<<NumerPliku;
  ekran<<" :  Analiza Liniowa Kszta³tu - Gauss Lewa\0"; ekran.seekp(0);
  Form3->Caption=ekran.str();
  double max = MINDOUBLE;
  double min = MAXDOUBLE;

  Akt=DajZb(NumerPliku);
  for(int i=1+indeks0;i<stopIndeks;i++)
  {
        double tym = Tab[2][i];
        if (tym>max) max = tym;
        if (tym<min) min = tym;
  }
  double MaxX,MinX;
  MaxX=Tab[0][stopIndeks-1];
  MinX=Tab[0][indeks0+1];
  Wsp.OblWsp(MinX,MaxX,min,max,PaintBox1);
  Graphics::TBitmap* M=new Graphics::TBitmap();
  MBCanv=M->Canvas;
  M->Width=6;
  M->Height=6;
  MBCanv->Brush->Color=clBlue;
  MBCanv->Brush->Color=clBlue;
  MBCanv->Ellipse(0,0,6,6);

  TPoint Punkt = TPoint(Wsp.Xe(Tab[0][indeks0+1]),Wsp.Ye(Tab[2][indeks0+1]));
  MBit->Canvas->MoveTo(Punkt.x,Punkt.y);
  for(int i=indeks0+2;i<stopIndeks;i+=UpDown1->Position)
  {
   Punkt.x=Wsp.Xe(Tab[0][i]);
   Punkt.y=Wsp.Ye(Tab[2][i]);
   MBit->Canvas->Draw(Punkt.x,Punkt.y,M);             //MBit->Canvas->LineTo(Punkt.x,Punkt.y);
  }
  MBit->Canvas->MoveTo(Wsp.Xe(MinX),Wsp.Ye(LGauss.a*MinX+LGauss.b));
  MBit->Canvas->Pen->Width=2;
  MBit->Canvas->Pen->Color=clRed;
  MBit->Canvas->LineTo(Wsp.Xe(MaxX),Wsp.Ye(LGauss.a*MaxX+LGauss.b));
  PaintBox1->Canvas->Draw(0,0,MBit);
  PaintBox1->Canvas->Draw(0,0,MBit);
  delete MBit;
  delete M;
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




void __fastcall TForm3::FormPaint(TObject *Sender)
{
   co_rysowac();
}
//---------------------------------------------------------------------------

void TForm3::co_rysowac()
{
  static int Nr_Pliku = NumerPliku;


  ZbiorTablic *Akt = DajZb(NumerPliku);
  Punkty* tym=Akt->Pola[Akt->DajIndeks(NumerPliku)].nastepny;
  if(!tym) return;
  if(tym->nastepny) return ;
  indeks0=tym->indeks;

  static int Nr_Ind_B0=indeks0;

  if (Nr_Pliku!=NumerPliku || Nr_Ind_B0!=indeks0 ) // Zmiana Linii, lub pola rezonansowego
  {
     start_prosta=false;
     zaczynaj_zazn=false;
     start=0;
     stop=0;
     startIndeks=0;
     stopIndeks=0;
     Nr_Pliku=NumerPliku;
     Nr_Ind_B0=indeks0;
     wykres->Down=true;
  }
    ProstaStart.x=0;
    ProstaStart.y=0;
    StaraProsta.x=0;
    StaraProsta.y=0;
 if(ToolButton1->Down)
 {
 }
 else if(wykres->Down)
     rysuj();
 else if(LL->Down)
     rysujLL();
 else if(GL->Down)
     rysujGL();
 else if(LP->Down)
     rysujPL();
 else if(GP->Down)
     rysujPG();
}
void __fastcall TForm3::ToolButton1Click(TObject *Sender)
{
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::PaintBox1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
 static oldX=X;
if(wykres->Down)
 {
  double pole=Wsp.Xrz(X);//(X-Wsp.B)/Wsp.A;

  StatusBar1->Panels->Items[0]->Text=AnsiString("Pole magnetyczne wynosi " + FloatToStrF(pole,ffFixed,15,3)+ " [mT] ");

 }
else if(LL->Down || GL->Down )
{
  double B2=Wsp.Xrz(X);//(X-Wsp.B)/Wsp.A;    //ekranowe na normalne
  if(B2<0) return;
  B2=sqrt(B2);                   // dostalismy B-B0
  B2=Tab[0][0]-B2;      // obliczamy B - Tab[0][0] zawiera B0
  StatusBar1->Panels->Items[0]->Text=AnsiString("Pole magnetyczne wynosi " + FloatToStrF(B2,ffFixed,15,3)+ " [mT] ");
}
else if(LP->Down || GP->Down)
{
  double B2=Wsp.Xrz(X);//(X-Wsp.B)/Wsp.A;    //ekranowe na normalne
  if(B2<0) return;
  B2=sqrt(B2);                   // dostalismy B-B0
  B2=Tab[0][0]+B2;      // obliczamy B
  StatusBar1->Panels->Items[0]->Text=AnsiString("Pole magnetyczne wynosi " + FloatToStrF(B2,ffFixed,15,3)+ " [mT] ");
}


if(prosta->Down && start_prosta)
{
  PaintBox1->Canvas->MoveTo(ProstaStart.x,ProstaStart.y);
  PaintBox1->Canvas->Pen->Mode=pmNotXor;
  PaintBox1->Canvas->LineTo(StaraProsta.x,StaraProsta.y);
  StaraProsta.x = X;  StaraProsta.y=Y;
  PaintBox1->Canvas->MoveTo(ProstaStart.x,ProstaStart.y);
  PaintBox1->Canvas->LineTo(X,Y);
  PaintBox1->Canvas->Pen->Mode=pmCopy;

}
}
//---------------------------------------------------------------------------


void __fastcall TForm3::PaintBox1DragOver(TObject *Sender, TObject *Source,
      int X, int Y, TDragState State, bool &Accept)
{

 // PaintBox1->Canvas->Brush->Style=bsClear;
 // PaintBox1->Canvas->Rectangle(start,0,X,PaintBox1->Height);

}
//---------------------------------------------------------------------------



void __fastcall TForm3::PowiekszClick(TObject *Sender)
{
      if(!wykres->Down)
      {
        Powieksz->Down=false;
        return;
      }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::PaintBox1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(prosta->Down && !start_prosta)
   {
    start_prosta=true;
    PaintBox1->Canvas->MoveTo(ProstaStart.x,ProstaStart.y);
    PaintBox1->Canvas->Pen->Mode=pmNotXor;
    PaintBox1->Canvas->LineTo(StaraProsta.x,StaraProsta.y);
    ProstaStart.x=X;
    ProstaStart.y=Y;
    StaraProsta.x=X;
    StaraProsta.y=Y;
    return;
   }
   else if(prosta->Down)
   {
    start_prosta=false;
    PaintBox1->Canvas->MoveTo(ProstaStart.x,ProstaStart.y);
    PaintBox1->Canvas->LineTo(X,Y);
    return;
   }

   if(!Powieksz->Down || !wykres->Down)
        return;
   else
   {
        if(!zaczynaj_zazn)
        {
             zaczynaj_zazn=true;
             PaintBox1->Canvas->MoveTo(X,0);
             PaintBox1->Canvas->LineTo(X,PaintBox1->Height);
             start= Wsp.Xrz(X);     //(X-Wsp.B)/Wsp.A;
        }
        else
        {
             zaczynaj_zazn=false;
             PaintBox1->Canvas->MoveTo(X,0);
             PaintBox1->Canvas->LineTo(X,PaintBox1->Height);
             stop=Wsp.Xrz(X);       //(X-Wsp.B)/Wsp.A;
             Invalidate();
        }

   }

}
//---------------------------------------------------------------------------



void __fastcall TForm3::ToolButton2Click(TObject *Sender)
{
     start=0; stop=0;
     UstawInd();
     Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TForm3::PaintBox1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

    start_prosta=false;
    return;

}
//---------------------------------------------------------------------------

void __fastcall TForm3::FormResize(TObject *Sender)
{
  Wsp.ZmienWsp();
}
//---------------------------------------------------------------------------

void __fastcall TForm3::OpcjeClick(TObject *Sender)
{
    if(Panel1->Visible)
    {

      Panel1->Visible=false;
    }
    else
    {

        Panel1->Visible=true;
        Panel1->BringToFront();

    }

}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button1Click(TObject *Sender)
{
  Panel1->Visible=false;
  Opcje->Down=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::FormCreate(TObject *Sender)
{
     int ramka=7;
     PaintBox1->Left=ramka;
     PaintBox1->Width=ClientWidth-ramka*2;
     PaintBox1->Top=ramka+ToolBar1->Height;
     PaintBox1->Height=(ClientHeight-ToolBar1->Height-StatusBar1->Height)-2*ramka;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::ToolButtonZapiszClick(TObject *Sender)
{
  enum {LorLewa,LorPrawa,GausLewa,GausPrawa,WykresLinii} Stan;
  AnsiString Plik, Linijka;
  if(GL->Down){
   Stan=GausLewa;
   Plik="GausLewa";
  }
  else if(GP->Down){
        Stan=GausPrawa;
        Plik="GausPrawa";
  }
  else if(LL->Down){
         Stan=LorLewa;
         Plik="LorLewa";
  }
  else if(LP->Down){
         Stan=LorPrawa;
         Plik="LorPrawa";
  }
  else {
        Stan=WykresLinii;
        Plik="WykresLiniiRezonansowej";
  }
  SaveDialogAnamorfoza->FileName=Plik;
  if(!rysunek) return;
  UstawInd();
  ZbiorTablic *Akt = ZbiorTablic::glowa;
  Akt=DajZb(NumerPliku);
  AnsiString PrzyklLancuch;
  if(SaveDialogAnamorfoza->Execute())
  {
        Memo1->Lines->Clear();
        PrzyklLancuch="B0= ";
        PrzyklLancuch+=DajX(indeks0,NumerPliku);
        Memo1->Lines->Add(PrzyklLancuch);
        PrzyklLancuch="Szerokoœæ_pp= ";
        PrzyklLancuch+=SzerPP;
         Memo1->Lines->Add(PrzyklLancuch);
        switch(Stan)
        {
          case WykresLinii :
                for(int i=startIndeks+1;i<stopIndeks;i++)
                {
                  Linijka=DajX(i,NumerPliku);
                  Linijka+="     ";
                  Linijka+=DajY(i,NumerPliku);
                  Memo1->Lines->Add(Linijka);
                }
                break;
          case LorLewa :
                for(int i=startIndeks+1;i<indeks0;i+=UpDown1->Position)
                {
                  Linijka=Tab[0][i];
                  Linijka+="     ";
                  Linijka+=Tab[1][i];
                  Memo1->Lines->Add(Linijka);
                }
                break;
          case GausLewa :
                for(int i=startIndeks+1;i<indeks0;i+=UpDown1->Position)
                {
                  Linijka=Tab[0][i];
                  Linijka+="     ";
                  Linijka+=Tab[2][i];
                  Memo1->Lines->Add(Linijka);
                }
                break;
          case LorPrawa :
                for(int i=indeks0+1;i<stopIndeks;i+=UpDown1->Position)
                {
                  Linijka=Tab[0][i];
                  Linijka+="     ";
                  Linijka+=Tab[1][i];
                  Memo1->Lines->Add(Linijka);
                }
                break;
          case GausPrawa :
                for(int i=indeks0+1;i<stopIndeks;i+=UpDown1->Position)
                {
                  Linijka=Tab[0][i];
                  Linijka+="     ";
                  Linijka+=Tab[2][i];
                  Memo1->Lines->Add(Linijka);
                }
                break;

        }
        Memo1->Lines->SaveToFile(SaveDialogAnamorfoza->FileName);
        return;
  }
}
//---------------------------------------------------------------------------

