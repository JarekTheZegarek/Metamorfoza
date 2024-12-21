//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Eksport.h"
#include "metamorfozis_w5.h"
#include "Punkty.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
TJKonwersje konwersja;
int MnoznikJedn=1;

extern ObszarPunkty::TListaPunktow ZebranePunkty;




static void Inicjowanie()
{
  switch(Opcje.Jedn)
  {
      case mT: MnoznikJedn=1;
               break;
      case Gs: MnoznikJedn=10;
                break;
  }
}

void EksportujMathematica(const char* nazwa)
{
       Inicjowanie();
       ofstream wyjscie(nazwa);
       wyjscie<<"PolaRez= {";
       ObszarPunkty::Punkt* Pkt;
       int koniec = ZebranePunkty.DajLiczbePunktow();
       for (int i = 0; i<koniec; i++)
       {
          if((Pkt=ZebranePunkty.DajNastepny())!=0 )
          {
                  wyjscie<<"{"<<Pkt->DajWartosc()<<",";
                  wyjscie<<Pkt->DajPole()*MnoznikJedn<< "}";
                  if ( i<koniec-1 )
                        wyjscie<<',';
           }
       }
       wyjscie<<"}";
       wyjscie.close();

/*
       using namespace PrzestrzenOpcji;
       Punkty* tym;
       typedef float (TJKonwersje::*Tfun)(float);
       Tfun fun;
       switch(Opcje.ZapisywaneDane)
          {
          case TemperaturaK : fun=&TJKonwersje::TempKelvin;//(double
                              break;
          case TemperaturaC : fun=&TJKonwersje::TempCelsjusz;
                              break;
          default           :  fun=&TJKonwersje::PrzepDouble; //dotych;   // Dla k¹tów podajemy odczytan¹ wartoœæ
                              break;
          }
       float dotych, KolumnaX;
       int NrPliku;




       bool pierwszy = false;
       for(int i=0; i<LiczbaWczytPlikow;i++)
       {
         NrPliku=TablicaNrPlikow[i];
         ZbiorTablic* Akt=DajZb(NrPliku);

         if((tym=Akt->Pola[Akt->DajIndeks(NrPliku)].nastepny)!=0)
         {
            if(pierwszy)
           {
              pierwszy = false;
              wyjscie<<",";
            }

            dotych=static_cast<float>(Akt->DajWartosc(NrPliku));
            KolumnaX=(konwersja.*fun)(dotych);
            while(tym)
             {
                wyjscie<<"{"<<KolumnaX<<",";
                wyjscie<<DajX(tym->indeks,NrPliku)*MnoznikJedn<< "}";
                if( (tym=tym->nastepny)!=0) wyjscie<<",";
             }
             pierwszy=true;
             //wyjscie<<"}"<<endl;
         }
       }
       wyjscie<<"}";
       wyjscie.close();
*/
}

//--------------------------------------------------------------------
//float PrzepDouble(float d) {return d;}

void EksportujDat(const char* nazwa)
{
       Inicjowanie();
       ofstream wyjscie(nazwa);
       ObszarPunkty::Punkt* Pkt;
        while((Pkt=ZebranePunkty.DajNastepny())!=0 )
        {
                wyjscie<<Pkt->DajWartosc()<<"\t";
                wyjscie<<Pkt->DajPole()*MnoznikJedn <<endl;
         }
       wyjscie.close();

/*
       using namespace PrzestrzenOpcji;
       Punkty* tym;
       typedef float (TJKonwersje::*Tfun)(float);
       Tfun fun;
       switch(Opcje.ZapisywaneDane)
          {
          case TemperaturaK :  fun=&TJKonwersje::TempKelvin;//(double);
                              break;
          case TemperaturaC :  fun=&TJKonwersje::TempCelsjusz;
                              break;
          default           :  fun=&TJKonwersje::PrzepDouble; //dotych;   // Dla k¹tów podajemy odczytan¹ wartoœæ
                              break;
          }
       float dotych, KolumnaX;
       int NrPliku;
       for(int i=0; i<LiczbaWczytPlikow;i++)
       {
         NrPliku=TablicaNrPlikow[i];
         ZbiorTablic* Akt=DajZb(NrPliku);
         if((tym=Akt->Pola[Akt->DajIndeks(NrPliku)].nastepny)!=0) //Ustawia Pierwsz¹ Wartosc
         {
           dotych=static_cast<float>(Akt->DajWartosc(NrPliku));
           KolumnaX=(konwersja.*fun)(dotych);
           wyjscie<<KolumnaX<<"\t";
            while(tym)               // Iteracja po nastepnych watosciach
             {
              wyjscie<<DajX(tym->indeks,NrPliku)*MnoznikJedn<< "\t";
              tym=tym->nastepny ;
             }
             wyjscie<<endl;
         }
       }
       wyjscie.close();
*/
}


void EksportujSzerDat(const char* nazwa)
{
       Inicjowanie();
       ofstream wyjscie(nazwa);
       using namespace PrzestrzenOpcji;
       Punkty* tym;
       typedef float (TJKonwersje::*Tfun)(float);
       Tfun fun;
       switch(Opcje.ZapisywaneDane)
          {
          case TemperaturaK : /*KolumnaX*/ fun=&TJKonwersje::TempKelvin;//(double/*dotych*/);
                              break;
          case TemperaturaC : /*KolumnaX*/ fun=&TJKonwersje::TempCelsjusz;
                              break;
          default           : /*KolumnaX*/ fun=&TJKonwersje::PrzepDouble; //dotych;   // Dla k¹tów podajemy odczytan¹ wartoœæ
                              break;
          }
       float dotych, KolumnaX,SzerB;
       int X1S,X2S;
       TPoint BB;
       int NrPliku;
       for(int i=0; i<LiczbaWczytPlikow;i++)
       {
        NrPliku=TablicaNrPlikow[i];
        ZbiorTablic* Akt=DajZb(NrPliku);
         BB=Akt->DajSzer(NrPliku,1);
         if(BB.x<0 || BB.y < 0) continue; // Pomiñ puste szerokosci
            if(BB.x>=0 && BB.y >=0)
            {
                 X1S= (BB.x<BB.y)?BB.x:BB.y ;
                 X2S= (BB.x>BB.y)?BB.x:BB.y ;
            }

       //  dotych=static_cast<float>(Akt->DajWartosc(NrPliku));
         dotych = TablicaWart[1][NrPliku-1];
         KolumnaX=(konwersja.*fun)(dotych);
         SzerB= DajX(X2S,NrPliku) - DajX(X1S,NrPliku);
         wyjscie<<KolumnaX<<"\t"<<SzerB*MnoznikJedn<< "\t"<<endl;
       //  if(tym=Akt->Pola[Akt->DajIndeks(i)].nastepny) //Ustawia Pierwsz¹ Wartosc
        // {
         //   while(tym)               // Iteracja po nastepnych watosciach
         //    {
         //     tym=tym->nastepny ;
         //    }

        // }
       }
       wyjscie.close();
}

void EksportujIntensywnoscDat(const char* nazwa)
{
       Inicjowanie();
       ofstream wyjscie(nazwa);
       using namespace PrzestrzenOpcji;
       Punkty* tym;
       typedef float (TJKonwersje::*Tfun)(float);
       Tfun fun;
       switch(Opcje.ZapisywaneDane)
          {
          case TemperaturaK : /*KolumnaX*/ fun=&TJKonwersje::TempKelvin;//(double/*dotych*/);
                              break;
          case TemperaturaC : /*KolumnaX*/ fun=&TJKonwersje::TempCelsjusz;
                              break;
          default           : /*KolumnaX*/ fun=&TJKonwersje::PrzepDouble; //dotych;   // Dla k¹tów podajemy odczytan¹ wartoœæ
                              break;
          }
       float dotych, KolumnaX,Intens;
       int X1S,X2S;
       TPoint BB;
       int NrPliku;
       for(int i=0; i<LiczbaWczytPlikow;i++)
       {
        NrPliku=TablicaNrPlikow[i];
        ZbiorTablic* Akt=DajZb(NrPliku);
         BB=Akt->DajSzer(NrPliku,1);
         if(BB.x<0 || BB.y < 0) continue; // Pomiñ puste szerokosci
            if(BB.x>=0 && BB.y >=0)
            {
                 X1S= (BB.x<BB.y)?BB.x:BB.y ;
                 X2S= (BB.x>BB.y)?BB.x:BB.y ;
            }

       //  dotych=static_cast<float>(Akt->DajWartosc(NrPliku));
         dotych = TablicaWart[1][NrPliku-1];
         KolumnaX=(konwersja.*fun)(dotych);
         Intens= fabs(DajY(X2S,NrPliku) - DajY(X1S,NrPliku));
         wyjscie<<KolumnaX<<"\t"<<Intens<< "\t"<<endl;
       //  if(tym=Akt->Pola[Akt->DajIndeks(i)].nastepny) //Ustawia Pierwsz¹ Wartosc
        // {
         //   while(tym)               // Iteracja po nastepnych watosciach
         //    {
         //     tym=tym->nastepny ;
         //    }

        // }
       }
       wyjscie.close();
}


double Konwertuj(double Wart)
{

  typedef float (TJKonwersje::*Tfun)(float);
  Tfun fun;
   switch(Opcje.ZapisywaneDane)  // fun zawiera wskaŸnik do odpowiedniej funkcji konwersji
      {
      case TemperaturaK : /*KolumnaX*/ fun=&TJKonwersje::TempKelvin;//(double/*dotych*/);
                          break;
      case TemperaturaC : /*KolumnaX*/ fun=&TJKonwersje::TempCelsjusz;
                          break;
      default           : /*KolumnaX*/ fun=&TJKonwersje::PrzepDouble; //dotych;   // Dla k¹tów podajemy odczytan¹ wartoœæ
                          break;
      }
     float dotych;

     dotych=static_cast<float>(Wart);                   //Wartosc jest zapamietywana jako double
     return (konwersja.*fun)(dotych);

}
