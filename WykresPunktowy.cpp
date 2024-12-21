//---------------------------------------------------------------------------

#include <vcl.h>
#include <algorithm>
#pragma hdrstop

#include "WykresPunktowy.h"
#include "metamorfozis_w5.h"
#include "Punkty.h"
#include "Test.h"
#include <map>
using namespace std;
extern ObszarPunkty::TListaPrzejsc ListaPrzejsc;       // Lista zdefiniowanych przejsc dla Weila
extern ObszarPunkty::TListaPunktow ZebranePunkty;      // Lista zebranych pol rezonansowych
extern std::map<std::string, int> NazwySymboli;

//extern map<std::string, int> NazwySymboli;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TWykresPktForm *WykresPktForm;

//---------------------------------------------------------------------------
__fastcall TWykresPktForm::TWykresPktForm(TComponent* Owner)
        : TForm(Owner)
{
    Polozenia = new TJWykresyPunktow(PaintBoxPkt);
    UpDown1->Min=0;
    UpDown1->Max=0;
    InicjujNazwySymboli();
    CzyNowePrzejscie=false;
    PaintBoxPkt->ClientWidth=ClientWidth;
    PaintBoxPkt->ClientHeight=ClientHeight;
}
//---------------------------------------------------------------------------
void __fastcall TWykresPktForm::PaintBoxPktPaint(TObject *Sender)
{
    Polozenia->rysuj();
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::FormResize(TObject *Sender)
{
  if(Polozenia->CzyWykresBylJuzOtwierany())
  {
            if(PaintBoxPkt->ClientWidth!=ScrollBox1->ClientWidth)
            {
               PaintBoxPkt->ClientWidth=ScrollBox1->ClientWidth;
            }
            if(PaintBoxPkt->ClientHeight!=ScrollBox1->ClientHeight)
            {
               PaintBoxPkt->ClientHeight=ScrollBox1->ClientHeight;
            }
            Polozenia->UstawRozmiarPl(PaintBoxPkt);
  }
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::Od1Click(TObject *Sender)
{
        Polozenia->Update(LiczbaZebranychPolozen);
        Polozenia->rysuj();

}
//---------------------------------------------------------------------------
TParaOgl<int> DajWspInt(double Wart, double B)
{
        return DajWsp(Wart, B,*(WykresPktForm->Polozenia));
}

void __fastcall TWykresPktForm::PaintBoxPktMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int Plik = Polozenia->SzukajNrPliku(X,Y);
  std::list<int>::iterator iter,iterUn,iterKoniec;
  std::list<ListSklad>::iterator iterZew;
  ObszarPunkty::Punkt* ptrP;
  double W = Polozenia->DajWartX(X);
  double BB= Polozenia->DajWartY(Y);
  int Xp,Yp;
  ptrP=ZebranePunkty.ZnajdzPunkt(W, BB,  DajWspInt );
  switch (Button)
  {
  case  mbLeft:
        /* TODO 1 : Dodaæ kasowanie */
     try{
          if(ToolButtonPowieksz->Down)
          {
            Xp = PaintBoxPkt->ClientWidth;
            Yp = PaintBoxPkt->ClientHeight;
           if(Shift.Contains(ssCtrl))
           {
              //PaintBoxPkt->Cursor =static_cast<TCursor>(crCursorMagnMinus);
              PaintBoxPkt->ClientWidth=Xp/1.1;
              PaintBoxPkt->ClientHeight=Yp/1.1;
              if(PaintBoxPkt->ClientWidth<ScrollBox1->ClientWidth)
              {
                 PaintBoxPkt->ClientWidth=ScrollBox1->ClientWidth;
              }
              if(PaintBoxPkt->ClientHeight<ScrollBox1->ClientHeight)
              {
                 PaintBoxPkt->ClientHeight=ScrollBox1->ClientHeight;
              }
              Polozenia->Update(LiczbaZebranychPolozen);
              Polozenia->rysuj();
               return;
           }
            //PaintBoxPkt->Cursor =static_cast<TCursor>(crCursorMagnPlus);
            PaintBoxPkt->ClientWidth=Xp+Xp*0.1;
            PaintBoxPkt->ClientHeight=Yp+Yp*0.1;
            Polozenia->Update(LiczbaZebranychPolozen);
            Polozenia->rysuj();
            return;

          }
          if(KasujPunkty->Down)
          {
             StosKasowania.umiesc(*ptrP);
             ZebranePunkty.UsunPunkt(*ptrP);
             Polozenia->rysuj();
             Form1->Invalidate();
             return;
          }
        } // Koniec try
        catch(...)
        {
              ShowMessage("Zbyt du¿e powiêkszenie");
              Xp = PaintBoxPkt->ClientWidth;
              Yp = PaintBoxPkt->ClientHeight;
              PaintBoxPkt->ClientWidth=Xp/1.1;
              PaintBoxPkt->ClientHeight=Yp/1.1;
              return;
        }
        if (Plik>0)
         {
            NumerPliku=Plik;
            //Prze³¹czanie Form1 na "klikniêty" plik
            for(int i=0;i<LiczbaWczytPlikow;i++)
              if (TablicaNrPlikow[i]==Plik)
              {
                  Form1->UpDown2->Position = i+1;
                  break;
              }
          if(ToFront->Down) Form1->BringToFront();
            Form1->Invalidate();
          TPoint Punkt;
          if(Zazn->Down /*&& (t=Polozenia->SzukajNajblIndeksu(X,Y))>=0 */)
          {
               /* TODO : Dodaæ zaznaczanie pojedyncze */

               ZebranePunkty.DodajPrzejscie(Polozenia->NumerPrzejscia,ptrP);
             //  ptrP->DodajPrzejscie(Polozenia->NumerPrzejscia);
               Polozenia->rysuj();
            /*
              if(!RadioZaznWiel->Checked)
                  if(Polozenia->Zaznaczony(t))
                                break;
               Polozenia->AktualnaLista->punkty.insert(Polozenia->AktualnaLista->punkty.begin(),t);
               Polozenia->AktualnaLista->punkty.sort();
               iterKoniec =Polozenia->AktualnaLista->punkty.end();
               Polozenia->AktualnaLista->punkty.unique();
               iterUn=Polozenia->AktualnaLista->punkty.end();
               //Nowe zaznaczenie, je¿eli operacja uniqe() nie spowodowa³a zmniejszenia listy
               if(iterUn==iterKoniec)        // Je¿eli nowy punkt
               {
                     Polozenia->Zaznacz(t);
                     Polozenia->rysuj();
               }
               iter=Polozenia->AktualnaLista->punkty.begin();
               while(iter!=Polozenia->AktualnaLista->punkty.end())
               {
                 Form1->Memo1->Lines->Add(*iter++);
               }
            */
          }
         }
         break;
  case mbRight:
    if (Plik>0)
             {

              if(Zazn->Down && Polozenia->SzukajNajblIndeksu(X,Y)>=0)
                {
                    /* TODO : Dorobiæ skasujprzejsæie */
                   //ptrP->SkasujPrzejscie(Polozenia->NumerPrzejscia);

                    ObszarPunkty::Punkt::TIntIter it=ptrP->Przejscia.begin();
                    ptrP->Przejscia.erase(find(ptrP->Przejscia.begin(),ptrP->Przejscia.end(),Polozenia->NumerPrzejscia));


                  /*
                   iter=find(Polozenia->AktualnaLista->punkty.begin(),Polozenia->AktualnaLista->punkty.end(),t);
                   iterKoniec=Polozenia->AktualnaLista->punkty.end();
                   Polozenia->AktualnaLista->punkty.erase(iter);
                   if(iter!=iterKoniec)
                                Polozenia->Odznacz(t); //by³o Zaznacz(t,0);
                   iter=Polozenia->AktualnaLista->punkty.begin();
                   while(iter!=Polozenia->AktualnaLista->punkty.end())
                   {
                     Form1->Memo1->Lines->Add(*iter++);
                   }
                   */
                   Polozenia->rysuj();
                }
             }
             break;
    }

}
//---------------------------------------------------------------------------





void __fastcall TWykresPktForm::PaintBoxPktMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
        String T;
        T.printf("Aktualne Po³o¿enie %d %d (%.3f, %.3f)",X,Y,Polozenia->DajWartX(X),Polozenia->DajWartY(Y));
        StatusBar1->SimpleText=T;
        StatusBar1->SimplePanel=true;
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::ZaznClick(TObject *Sender)
{
     if(Polozenia->NumerPrzejscia==0)
     {
       Application->MessageBox("Zdefiniuj Przejœcie","Uwaga",MB_OK);
       Zazn->Down=false;
       return;
     }
      ListSklad tym;
      Polozenia->Przejscia.insert(Polozenia->AktualnaLista,tym);
      if(KasujPunkty->Down && Zazn->Down)
        KasujPunkty->Down=false;

}
//---------------------------------------------------------------------------


void __fastcall TWykresPktForm::ShapeZaznaczenieMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
ColorDialog1->Color=ShapeZaznaczenie->Brush->Color;
if( ColorDialog1->Execute())
       ShapeZaznaczenie->Brush->Color=ColorDialog1->Color;
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::ToolButton2Click(TObject *Sender)  // Otworz opcje.
{
  if(Panel1->Visible)
      Panel1->Visible=false;
  else
  {
       using namespace ObszarPunkty;
       // Ustawianie Combo Boxow dostêpnymi symbolami
       int tym;
       std::map<std::string, int>::iterator  it;
       for (it = NazwySymboli.begin();it!=NazwySymboli.end();it++)
       {
            tym = it->second;
            ComboBoxPunkty->Items->Strings[tym] = it->first.c_str();
            ComboBoxZaznaczenia->Items->Strings[tym] = it->first.c_str();
       }



       //Ustalanie Akutalnego Przejscia i parametrow symboli

       Symbol* TymSymZazn;
       if(ListaPrzejsc.DajLiczbeZdefiniowanychPrzejsc()!=0)
       {
           Przejscie* Przej = ListaPrzejsc.DajPrzejscie(Polozenia->NumerPrzejscia);
           int P1, P2;
           P1 = Przej->DajPoziom(1);
           P2 = Przej->DajPoziom(2);

           UpDown1->Min=1;
           UpDown1->Max=ListaPrzejsc.DajLiczbeZdefiniowanychPrzejsc();
           UpDown1->Position = Polozenia->NumerPrzejscia;
           if( P1!=0 && P2!= 0 )
           {
                Edit1->Text=P1;
                Edit2->Text=P2;
           }
           TymSymZazn = Przej->DajKsztalt();
       }
       else
       {
           TymSymZazn = Polozenia->DajSymbolZaznaczania();
       }
       Symbol* TymSymPkt =  Polozenia->DajSymbolPunktow();   // Punkt::DajSymbol();
       ShapePunkt->Brush->Color = TymSymPkt->WezKolorWypeln();
       KolorPunktu->Brush->Color = TymSymPkt->WezKolorLinii();
       UpDown2->Position = TymSymPkt->DajRozmiar();
       SymbolePol->Text = TymSymPkt->DajRozmiar();

       KolorPrzej->Brush->Color = TymSymZazn->WezKolorLinii();
       ShapeZaznaczenie->Brush->Color = TymSymZazn->WezKolorWypeln();
       UpDown3->Position =      TymSymZazn->DajRozmiar();
       SymbolePrzejsc->Text =   TymSymZazn->DajRozmiar();

       ComboBoxPunkty->ItemIndex = NazwySymboli[TymSymPkt->DajTypSymbolu()];
       ComboBoxZaznaczenia->ItemIndex = NazwySymboli[TymSymZazn->DajTypSymbolu()];
       ComboBoxPunkty->OnChange(this);
       ComboBoxZaznaczenia->OnChange(this);

       Panel1->Visible=true;
  }


}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::Button1Click(TObject *Sender)
{
 using namespace ObszarPunkty;

  try
  {

     try
     {
     if(!Edit1->Text.IsEmpty())
     {
         int P1 = Edit1->Text.ToInt();
         int P2 = Edit2->Text.ToInt();
         int NrPrzejscia;
         Przejscie* P;
         NrPrzejscia=ListaPrzejsc.DajNumerPrzejscia(P1,P2) ;
         if( CzyNowePrzejscie && NrPrzejscia==0)  //tzn. ¿e to przejscie nie istnieje;
              P = new Przejscie(P1,P2);
         else
         {
              P = ListaPrzejsc.DajPrzejscie(NrPrzejscia);
              CzyNowePrzejscie = false;
         }

         Symbol* Akt = Polozenia->DajSymbolZaznaczania();
         Akt = NowySymbol(ComboBoxZaznaczenia->Text.c_str());
         Akt->UstRozmiar(SymbolePrzejsc->Text.ToInt());
         // new Kolo(SymbolePrzejsc->Text.ToInt());
         P->UstawKsztalt(Akt);
         P->UstawKolor(KolorPrzej->Brush->Color);
         P->UstawKolorWyp(ShapeZaznaczenie->Brush->Color);
         Polozenia->UstawSymbolZaznaczania(P->DajKsztalt());
         if(CzyNowePrzejscie)
         {
            CzyNowePrzejscie = false;
            ListaPrzejsc.DodajPrzejscie(P) ;
            Polozenia->NumerPrzejscia=P->DajNumer();
         }
         else
         {
            Polozenia->NumerPrzejscia=NrPrzejscia; // *** Okresla akutalne przejscie
         }
     }
     Symbol* Pkt = Punkt::DajSymbol();
     if(Pkt!=Form1->Zaznaczenie)         // zapobiega skasowaniu punktow rysowanych na wykresie linii
         delete Pkt;
     if((Pkt =  NowySymbol(ComboBoxPunkty->Text.c_str()))!=0)
     {
         Pkt->UstKolorLinii(KolorPunktu->Brush->Color);
         Pkt->UstKolorWypeln(ShapePunkt->Brush->Color);
         Pkt->UstRozmiar(SymbolePol->Text.ToInt());
         Punkt::UstawSymbol(Pkt);
         Polozenia->UstawSymbolPunktow(Pkt);
      }
      else
         {
           ShowMessage("Nie udal sie przydzial pamieci dla symbolu pól");
         }

     ZebranePunkty.ZaznaczanieWielokrotne = RadioZaznWiel->Checked;
     Splitter1->Invalidate();

     /*
     if( ListaPrzejsc.DodajPrzejscie(P) )
     {
       Polozenia->NumerPrzejscia=P->DajNumer();
     }
     else
     {
        Polozenia->UstawSymbolZaznaczania( P->DajKsztalt());
        // tu bêdzie wyciek pamieci ale chyba nie bardzo mozna kasowac wyznaczone punkty
     }
     */

     Edit1->Enabled=false;
     Edit2->Enabled=false;
     Panel1->Visible=false;
    // Edit3->Font->Color=ShapeZaznaczenie->Brush->Color;
     Label5->Caption = Edit1->Text +" -> " + Edit2->Text;
  /*
      ListSklad TymList;
      static OstNumer=0;
      std::list<ListSklad>::iterator ItSklad;
      ItSklad=Polozenia->Przejscia.begin();
      while(ItSklad!=Polozenia->Przejscia.end())     // Szukamy czy ju¿ wpisane
      {
        if(ItSklad->Przejscie1==Edit1->Text.ToInt() &&
           ItSklad->Przejscie2==Edit2->Text.ToInt())
        {
          Polozenia->AktualnaLista= ItSklad;
          Polozenia->AktualnaLista->Kolor=Shape1->Brush->Color;
          Polozenia->NumerPrzejscia==ItSklad->numer;
          Edit4->Text=ItSklad->numer;
          Panel1->Visible=false;
          Edit1->Enabled=false;
          Edit2->Enabled=false;
          Edit3->Font->Color=Shape1->Brush->Color;
          Edit3->Text= Edit1->Text +" -> " + Edit2->Text;
          return;
        }
        ItSklad++;
      }
      TymList.Przejscie1=Edit1->Text.ToInt();
      TymList.Przejscie2=Edit2->Text.ToInt();
      TymList.Kolor=Shape1->Brush->Color;
      TymList.numer=++OstNumer;
      Polozenia->Przejscia.insert(Polozenia->Przejscia.begin(),TymList);    //wstaw now¹ listê na pocz¹tek
      Polozenia->AktualnaLista= Polozenia->Przejscia.begin();               // ustaw iterator równie¿ na pocz¹tek
      Polozenia->NumerPrzejscia=OstNumer;
      Edit4->Text=OstNumer;
      Edit1->Enabled=false;
      Edit2->Enabled=false;
      Panel1->Visible=false;
       Edit3->Font->Color=Shape1->Brush->Color;
      Edit3->Text= Edit1->Text +" -> " + Edit2->Text;
  */
     }
     catch(ErrorPunkt& Er)
     {
         ShowMessage(Er.komunikat);
     }
     catch(EConvertError& Er)
     {
        AnsiString T ="Prawdopodobnie wpisano nieprawid³ow¹ liczbê. \n\n";
        T+=Er.Message;
        ShowMessage(T);
     }
     catch( ... )
     {
        ShowMessage("Prawdopodobnie: nie podano numerów przejœæ\n"
                    "Ich zdefiniowanie umo¿liwa przycisk \"Nowy\" ");
     }
 }
 __finally
 {
   Panel1->Visible=false;
 }
}
//---------------------------------------------------------------------------




void __fastcall TWykresPktForm::Button2Click(TObject *Sender)
{
  CzyNowePrzejscie=true ;
  Edit1->Enabled=true;
  Edit2->Enabled=true;
}
//---------------------------------------------------------------------------


void __fastcall TWykresPktForm::Label5Click(TObject *Sender)
{
   //Label5->Caption=UpDown1->Position;
}
//---------------------------------------------------------------------------



void __fastcall TWykresPktForm::ToolButton3Click(TObject *Sender)
{
  if(Polozenia->NumerPrzejscia!=0 && SaveDialog1->Execute())
  {


    ZapisWeil(0,SaveDialog1->FileName.c_str());
  /*
    Punkt* Pkt;
    int LiczbaPrzejsc = 0;
    while( (Pkt=ZebranePunkty.DajNastepny())!=0 )
    {
       LiczbaPrzejsc += Pkt->Przejscia.size();
    }
    fprintf(zapis,"Liczba wprowadzonych punktów\n%d \n\n\n",LiczbaPrzejsc);
    double theta,phi,frq, pole, wartosc;
    theta=ETheta->Text.ToDouble();
    phi=EPhi->Text.ToDouble();
    frq=Efrq->Text.ToDouble();
    Przejscie* Przej;
    while((Przej=ListaPrzejsc.DajNastepny())!=0)
    {
        TParaOgl<int> Poziom = Przej->DajPoziomy();
        while( (Pkt=ZebranePunkty.DajNastepny())!=0)
        {
         if(Pkt->CzyZawieraPrzejscie(Przej) )
         {
             pole = Pkt->DajPole();
             wartosc= Pkt->DajWartosc();

             switch(StalyKat->ItemIndex)
             {
              case 0: fprintf(zapis,"%.2f  %.2f  %d  %.2f",theta, // sta³a Theta
                      wartosc+phi,NBlock->Text.ToInt(),frq); // 1 = numer bloku
                      break;
              case 1 :  fprintf(zapis,"%.2f  %.2f  %d  %.2f",
                      wartosc+theta,phi, NBlock->Text.ToInt(), frq);
                      break;
             }
             fprintf(zapis,"\n1 \n");        // liczba przejsc
             fprintf(zapis,"%d %d %.2f 1\n\n",Poziom.X,
                    Poziom.Y, pole*10); //

         }
        }
    }
    fclose(zapis);
*/
  }
}
//---------------------------------------------------------------------------

void TWykresPktForm::ZapisWeil(int NrPrzejscia, AnsiString Nazwa)
{

    using namespace ObszarPunkty;
    FILE* zapis;
    zapis=fopen(Nazwa.c_str(), "wt");
    Punkt* Pkt;
    int LiczbaPrzejsc = 0;
    if(NrPrzejscia!=0)
    {
        while( (Pkt=ZebranePunkty.DajNastepny())!=0 )
        {
          if(Pkt->CzyZawieraPrzejscie(NrPrzejscia))
              LiczbaPrzejsc++ ;
        }
    }
    else
    {
        while( (Pkt=ZebranePunkty.DajNastepny())!=0 )
        {
           LiczbaPrzejsc += Pkt->Przejscia.size();
        }
    }
    fprintf(zapis,"Liczba wprowadzonych punktów\n%d \n\n\n",LiczbaPrzejsc);
    double theta,phi,frq, pole, wartosc;
    theta=ETheta->Text.ToDouble();
    phi=EPhi->Text.ToDouble();
    frq=Efrq->Text.ToDouble();
    Przejscie* Przej;
    int Nr, koniec = ListaPrzejsc.DajLiczbeZdefiniowanychPrzejsc();
    for(int i=0;i<koniec; i++)
    {
       Nr=ListaPrzejsc.DajNumerKolejny(i);
       Przej = ListaPrzejsc.DajPrzejscie(Nr);
       if(NrPrzejscia!=0)
        {
                Przej=ListaPrzejsc.DajPrzejscie(NrPrzejscia);
                i=koniec; //zakoñczenie pêtli
        }

        TParaOgl<int> Poziom = Przej->DajPoziomy();
        while( (Pkt=ZebranePunkty.DajNastepny())!=0)
        {
           if(Pkt->CzyZawieraPrzejscie(Przej) )
           {
               pole = Pkt->DajPole();
               wartosc= Pkt->DajWartosc();

               switch(StalyKat->ItemIndex)
               {
                case 0: fprintf(zapis,"%.2f  %.2f  %d  %.2f",theta, // sta³a Theta
                        wartosc+phi,NBlock->Text.ToInt(),frq); // 1 = numer bloku
                        break;
                case 1 :  fprintf(zapis,"%.2f  %.2f  %d  %.2f",
                        wartosc+theta,phi, NBlock->Text.ToInt(), frq);
                        break;
               }
               fprintf(zapis,"\n1 \n");        // liczba przejsc
               fprintf(zapis,"%d %d %.2f 1\n\n",Poziom.X,
                      Poziom.Y, pole*10); //

           }
        }


    }
/*
    bool Stop = false;
    while((Przej=ListaPrzejsc.DajNastepny())!=0 && !Stop)
    {
        if(NrPrzejscia!=0)
        {
                Przej=ListaPrzejsc.DajPrzejscie(NrPrzejscia);
                Stop=true;
        }
        TParaOgl<int> Poziom = Przej->DajPoziomy();
        while( (Pkt=ZebranePunkty.DajNastepny())!=0)
        {
           if(Pkt->CzyZawieraPrzejscie(Przej) )
           {
               pole = Pkt->DajPole();
               wartosc= Pkt->DajWartosc();

               switch(StalyKat->ItemIndex)
               {
                case 0: fprintf(zapis,"%.2f  %.2f  %d  %.2f",theta, // sta³a Theta
                        wartosc+phi,NBlock->Text.ToInt(),frq); // 1 = numer bloku
                        break;
                case 1 :  fprintf(zapis,"%.2f  %.2f  %d  %.2f",
                        wartosc+theta,phi, NBlock->Text.ToInt(), frq);
                        break;
               }
               fprintf(zapis,"\n1 \n");        // liczba przejsc
               fprintf(zapis,"%d %d %.2f 1\n\n",Poziom.X,
                      Poziom.Y, pole*10); //

           }
        }
    }
*/
    fclose(zapis);

}


void TWykresPktForm::ZapisText(int NrPrzejscia, AnsiString Nazwa)
{

    using namespace ObszarPunkty;
    FILE* zapis;
    zapis=fopen(Nazwa.c_str(), "wt");
    Punkt* Pkt;
    int LiczbaPrzejsc = 0;
    if(NrPrzejscia!=0)
    {
        while( (Pkt=ZebranePunkty.DajNastepny())!=0 )
        {
          if(Pkt->CzyZawieraPrzejscie(NrPrzejscia))
              LiczbaPrzejsc++ ;
        }
    }
    else
    {
        while( (Pkt=ZebranePunkty.DajNastepny())!=0 )
        {
           LiczbaPrzejsc += Pkt->Przejscia.size();
        }
    }

    double theta,phi,frq, pole, wartosc;
    theta=ETheta->Text.ToDouble();
    phi=EPhi->Text.ToDouble();
    frq=Efrq->Text.ToDouble();
    Przejscie* Przej;
    int Nr, koniec = ListaPrzejsc.DajLiczbeZdefiniowanychPrzejsc();
    for(int i=0;i<koniec; i++)
    {
       Nr=ListaPrzejsc.DajNumerKolejny(i);
       Przej = ListaPrzejsc.DajPrzejscie(Nr);
       if(NrPrzejscia!=0)
        {
                Przej=ListaPrzejsc.DajPrzejscie(NrPrzejscia);
                i=koniec; //zakoñczenie pêtli
        }

        TParaOgl<int> Poziom = Przej->DajPoziomy();
        while( (Pkt=ZebranePunkty.DajNastepny())!=0)
        {
           if(Pkt->CzyZawieraPrzejscie(Przej) )
           {
               pole = Pkt->DajPole();
               wartosc= Pkt->DajWartosc();
               fprintf(zapis,"%.4f  %.4f", pole, wartosc);


           }
        }


    }

    fclose(zapis);

}



void __fastcall TWykresPktForm::ToolButtonKasujAktSerieClick(TObject *Sender)
{

 if(Application->MessageBox("Czy naprawdê chcesz usun¹æ ca³¹ seriê danych?",
                            "Uwaga!!!",MB_YESNO)==IDNO) return;

  ObszarPunkty::Punkt* P_ptr;
  int NrPrzej=Polozenia->NumerPrzejscia;
  while( (P_ptr=ZebranePunkty.DajNastepny())!=0 )          //Wartosc = wczytane z pliku T lub Kat
  {

      ObszarPunkty::Punkt::TIntIter it=P_ptr->Przejscia.begin();
      P_ptr->Przejscia.erase(find(P_ptr->Przejscia.begin(),P_ptr->Przejscia.end(),NrPrzej));

  }

   /* std::list<int>::iterator iterWew;
    iterWew=Polozenia->AktualnaLista->punkty.begin();
       while(iterWew!=Polozenia->AktualnaLista->punkty.end())
       {
         Polozenia->Odznacz(*iterWew);
         iterWew++;
       }
       Polozenia->AktualnaLista->punkty.erase(Polozenia->AktualnaLista->punkty.begin(),Polozenia->AktualnaLista->punkty.end());
    */

       Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::ToolButtonKasujWszystkoClick(
      TObject *Sender)
{
 if(Application->MessageBox("Czy naprawdê chcesz usun¹æ wszystkie dane?",
                             "Uwaga!!!",MB_YESNO)==IDNO) return;
  ObszarPunkty::Punkt* P_ptr;
  int NrPrzej, Koniec= ListaPrzejsc.DajLiczbeZdefiniowanychPrzejsc();
  for(int i=0;i<Koniec;i++)
  {
      NrPrzej=ListaPrzejsc.DajNumerKolejny(i);
      while( (P_ptr=ZebranePunkty.DajNastepny())!=0 )          //Wartosc = wczytane z pliku T lub Kat
      {

          ObszarPunkty::Punkt::TIntIter it=P_ptr->Przejscia.begin();
          P_ptr->Przejscia.erase(find(P_ptr->Przejscia.begin(),P_ptr->Przejscia.end(),NrPrzej));

      }  // Koniec while
  } // Koniec for
 /*

 std::list<int>::iterator iterWew;
 std::list<ListSklad>::iterator iter;
 iter=Polozenia->Przejscia.begin();
 while(iter!=Polozenia->Przejscia.end())
     {
       iterWew=iter->punkty.begin();
       while(iterWew!=iter->punkty.end())
       {
         Polozenia->Odznacz(*iterWew);
         iterWew++;
       }
       iter->punkty.erase(iter->punkty.begin(),iter->punkty.end());
       iter++;
     }

 */
    Invalidate();
}
//---------------------------------------------------------------------------




void __fastcall TWykresPktForm::Test1Click(TObject *Sender)
{
    FormTest->Show();    
    FormTest->BringToFront();

}
//---------------------------------------------------------------------------


void __fastcall TWykresPktForm::ShapePunktMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    ColorDialog1->Color=ShapePunkt->Brush->Color;
    if( ColorDialog1->Execute())
       ShapePunkt->Brush->Color=ColorDialog1->Color;
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::KolorPunktuMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
     ColorDialog1->Color=KolorPunktu->Brush->Color;
     if( ColorDialog1->Execute())
     {
       KolorPunktu->Brush->Color=ColorDialog1->Color;
       ShapePunkt->Pen->Color=ColorDialog1->Color;
     }
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::KolorPrzejMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
     ColorDialog1->Color=KolorPrzej->Brush->Color;
     if( ColorDialog1->Execute())
     {
       KolorPrzej->Brush->Color=ColorDialog1->Color;
       ShapeZaznaczenie->Pen->Color=ColorDialog1->Color;
     }
}
//---------------------------------------------------------------------------


void __fastcall TWykresPktForm::ComboBoxPunktyChange(TObject *Sender)
{
   if(ComboBoxPunkty->Text=="Kwadrat")
        ShapePunkt->Shape = stSquare;
   else
        ShapePunkt->Shape = stCircle;
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::ComboBoxZaznaczeniaChange(TObject *Sender)
{
if(ComboBoxZaznaczenia->Text=="Kwadrat")
        ShapeZaznaczenie->Shape = stSquare;
   else
        ShapeZaznaczenie->Shape = stCircle;
}
//---------------------------------------------------------------------------




void __fastcall TWykresPktForm::FormActivate(TObject *Sender)
{
   if(ListaPrzejsc.DajLiczbeZdefiniowanychPrzejsc()!=0)
      {
        ObszarPunkty::Przejscie* Przej = ListaPrzejsc.DajPrzejscie(Polozenia->NumerPrzejscia);
        int P1, P2;
        P1 = Przej->DajPoziom(1);
        P2 = Przej->DajPoziom(2);
        AnsiString T = P1;
        T+=" -> ";
        T+=P2;
        Label5->Caption = T;
      }
}
//---------------------------------------------------------------------------


void __fastcall TWykresPktForm::Splitter1Paint(TObject *Sender)
{
  Symbol* sym = Polozenia->DajSymbolZaznaczania();
  if(sym)
  {
   sym->Rysuj(Splitter1->Canvas,Splitter1->ClientWidth/2,Splitter1->ClientHeight/2);
  }
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::Zpodzia1Click(TObject *Sender)
{
       ObszarPunkty::Przejscie* Przej;
       AnsiString T;
       while((Przej=ListaPrzejsc.DajNastepny())!=0)   // Wype³nianie ComboBox'a
       {
          T = Przej->DajPoziom(1);
          T+=" -> ";
          T+=Przej->DajPoziom(2);
          ComboBoxPrzejscia->Items->Add(T);
          MapaPrzejsc[T]=Przej->DajNumer();
       }
       Przej=ListaPrzejsc.DajPrzejscie(Polozenia->NumerPrzejscia);
       if(Przej!=0)
       {
         TParaOgl<int> Poziomy = Przej->DajPoziomy();
         T=Poziomy.X;
         T+=" -> ";
         T+=Poziomy.Y;
         Label5->Caption = T;
         ComboBoxPrzejscia->Text = T;      // Ustawienie takiego samego tekstu
                                           // jak jest na liscie wybiera go
         PaintBoxSymbol->Invalidate();
       }
       PanelZapisWielokrotny->Visible=true;

}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::UpDown1Click(TObject *Sender,
      TUDBtnType Button)
{
if(ListaPrzejsc.DajLiczbeZdefiniowanychPrzejsc()!=0)
      {
           ObszarPunkty::Przejscie* Prz;
       /*
           if(Button == btNext)
           {
             Prz=ListaPrzejsc.DajPrzejscie(UpDown1->Position + 1);
           }
           else
           {
             Prz=ListaPrzejsc.DajPrzejscie(UpDown1->Position - 1);
           }
       */
           /* TODO : Uogólniæ pobieranie odpowiedniego przejœcia */
           Prz=ListaPrzejsc.DajPrzejscie(UpDown1->Position);
           if(Prz!=0)
           {
             TParaOgl<int> Poziomy = Prz->DajPoziomy();
             using namespace ObszarPunkty;
             Symbol* TymSymZazn =     Prz->DajKsztalt();
             KolorPrzej->Brush->Color = TymSymZazn->WezKolorLinii();
             ShapeZaznaczenie->Pen->Color=KolorPrzej->Brush->Color;
             ShapeZaznaczenie->Brush->Color = TymSymZazn->WezKolorWypeln();
             SymbolePrzejsc->Text =TymSymZazn->DajRozmiar();
             ComboBoxZaznaczenia->ItemIndex = NazwySymboli[TymSymZazn->DajTypSymbolu()];
             ComboBoxZaznaczenia->OnChange(this);
             Edit1->Text=Poziomy.X;
             Edit2->Text=Poziomy.Y;
             Polozenia->NumerPrzejscia=Prz->DajNumer();
           }
      }
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::ButtonZapisWielokrotnyClick(
      TObject *Sender)
{
 ObszarPunkty::Przejscie* Prz;
 Prz=ListaPrzejsc.DajPrzejscie(MapaPrzejsc[ComboBoxPrzejscia->Text]);
 if(RadioGroupLiczbaPrzejsc->ItemIndex==0)  // 1 - oznacza wszystkie 0 - pojedynczy
 {
     if(Prz->DajNumer()!=0 && SaveDialog1->Execute())
     {
        ZapisWeil(Prz->DajNumer(),SaveDialog1->FileName.c_str());
     }
 }
 else
 {
    if(SaveDialog1->Execute())
    {

         AnsiString TymStr,Ext, NazwaBazowa= SaveDialog1->FileName;
         Ext = ExtractFileExt(NazwaBazowa);
         NazwaBazowa.Delete(NazwaBazowa.Pos(Ext),Ext.Length()); //Wydobycie nazwy pliku
         while((Prz=ListaPrzejsc.DajNastepny())!=0)
         {
            TParaOgl<int> Poziomy = Prz->DajPoziomy();
            TymStr = NazwaBazowa;
            TymStr+=Poziomy.X;
            TymStr+="_";
            TymStr+=Poziomy.Y;
            TymStr+=Ext;
            ZapisWeil(Prz->DajNumer(),TymStr.c_str());
         }
    }
 }



 PanelZapisWielokrotny->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::UpDownMenuZmianaPrzejscClick(
      TObject *Sender, TUDBtnType Button)
{
UpDownMenuZmianaPrzejsc->Min=1;
UpDownMenuZmianaPrzejsc->Max=ListaPrzejsc.DajLiczbeZdefiniowanychPrzejsc();
if(UpDownMenuZmianaPrzejsc->Max!=0)
      {
           ObszarPunkty::Przejscie* Prz;
           Prz=ListaPrzejsc.DajPrzejscie(UpDownMenuZmianaPrzejsc->Position);
           AnsiString T;
           if(Prz!=0)
           {
             TParaOgl<int> Poziomy = Prz->DajPoziomy();
             T=Poziomy.X;
             T+=" -> ";
             T+=Poziomy.Y;
             Label5->Caption = T;
             Polozenia->NumerPrzejscia=Prz->DajNumer();
             Polozenia->UstawSymbolZaznaczania(Prz->DajKsztalt());
             Splitter1->Invalidate();
           }
      }
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::PaintBoxSymbolPaint(TObject *Sender)
{
  ObszarPunkty::Przejscie* Prz;
  Prz=ListaPrzejsc.DajPrzejscie(MapaPrzejsc[ComboBoxPrzejscia->Text]);
  PaintBoxSymbol->Color=clWhite;
  Symbol* sym = Prz->DajKsztalt();
  if(sym)
  {
   sym->Rysuj(PaintBoxSymbol->Canvas,   PaintBoxSymbol->ClientWidth/2,
                                        PaintBoxSymbol->ClientHeight/2);
  }
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::ButtonAnulujZapisWielokrotnyClick(
      TObject *Sender)
{
    PanelZapisWielokrotny->Visible=false;
}
//---------------------------------------------------------------------------


void __fastcall TWykresPktForm::ComboBoxPrzejsciaChange(TObject *Sender)
{
 PaintBoxSymbol->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::Cofnijoperacj1Click(TObject *Sender)
{
      if( StosKasowania.wez(&ZebranePunkty))
      {
        Invalidate();
      }
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(Shift.Contains(ssCtrl) )
    {
      //  AnsiString Klawisz =UpperCase(Key);
        if(Key=='z' || Key =='Z' )
        {
            if( StosKasowania.wez(&ZebranePunkty))
              {
                Invalidate();
              }
            return;
        }

    }
    if(Shift.Contains(ssCtrl) && ToolButtonPowieksz->Down)
    {
            PaintBoxPkt->Cursor =static_cast<TCursor>(crCursorMagnMinus);
    }
    else if (ToolButtonPowieksz->Down)
    {
            PaintBoxPkt->Cursor =static_cast<TCursor>(crCursorMagnPlus);
    }

}
//---------------------------------------------------------------------------


void __fastcall TWykresPktForm::KasujPunktyClick(TObject *Sender)
{
  if(Zazn->Down && KasujPunkty->Down)
        Zazn->Down=false;
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::ToolButtonPowiekszClick(TObject *Sender)
{
  if(ToolButtonPowieksz->Down)
  {

     PaintBoxPkt->Cursor =static_cast<TCursor>(crCursorMagnPlus);
  }
  else
  {
    Tryb =Nieokreslony;
    PaintBoxPkt->Cursor = crDefault;
  }
}
//---------------------------------------------------------------------------


void __fastcall TWykresPktForm::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   if (ToolButtonPowieksz->Down && Key==VK_CONTROL )
    {
            PaintBoxPkt->Cursor =static_cast<TCursor>(crCursorMagnPlus);
    }
}
//---------------------------------------------------------------------------


void __fastcall TWykresPktForm::Pojedynczyplik2Click(TObject *Sender)
{
if(Polozenia->NumerPrzejscia!=0 && SaveDialog1->Execute())
  {


    ZapisText(0,SaveDialog1->FileName.c_str());

  }
}
//---------------------------------------------------------------------------

