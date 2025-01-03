//---------------------------------------------------------------------------

#ifndef WykresPunktowyH
#define WykresPunktowyH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "Wykres.h"
#include <Menus.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TWykresPktForm : public TForm
{
__published:	// IDE-managed Components
        TPaintBox *PaintBoxPkt;
        TMainMenu *MainMenu1;
        TMenuItem *Pliki1;
        TMenuItem *Polecenia1;
        TMenuItem *Od1;
        TCoolBar *CoolBar1;
        TActionList *ActionList1;
        TImageList *ImageList1;
        TAction *Action1;
        TToolBar *ToolBar1;
        TToolButton *ToolButton1;
        TToolButton *ToFront;
        TStatusBar *StatusBar1;
        TToolButton *Zazn;
        TToolBar *ToolBar2;
        TToolButton *ToolButton2;
        TPanel *Panel1;
        TEdit *Edit1;
        TEdit *Edit2;
        TLabel *Label1;
        TLabel *Label2;
        TColorDialog *ColorDialog1;
        TShape *Shape1;
        TButton *Button1;
        TEdit *Edit3;
        TSplitter *Splitter1;
        TSplitter *Splitter2;
        TLabel *Label3;
        TUpDown *UpDown1;
        TButton *Button2;
        TLabel *Label4;
        TLabel *Label5;
        TEdit *Edit4;
        TToolButton *ToolButton3;
        TSaveDialog *SaveDialog1;
        TEdit *ETheta;
        TLabel *Label6;
        TLabel *Label7;
        TEdit *EPhi;
        TEdit *Efrq;
        TLabel *Label8;
        TRadioGroup *StalyKat;
        TGroupBox *GroupBox1;
        TCheckBox *RadioZaznWiel;
        TToolButton *ToolButtonKasujAktSerie;
        TToolButton *ToolButtonKasujWszystko;
        void __fastcall PaintBoxPktPaint(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall Od1Click(TObject *Sender);
        void __fastcall PaintBoxPktMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall PaintBoxPktMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall ZaznClick(TObject *Sender);
        void __fastcall Shape1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton2Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall UpDown1Click(TObject *Sender, TUDBtnType Button);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Label5Click(TObject *Sender);
        void __fastcall UpDown1Changing(TObject *Sender,
          bool &AllowChange);
        void __fastcall Edit4Change(TObject *Sender);
        void __fastcall ToolButton3Click(TObject *Sender);
        void __fastcall ToolButtonKasujAktSerieClick(TObject *Sender);
        void __fastcall ToolButtonKasujWszystkoClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        TJWykresyPunktow* Polozenia;
        __fastcall TWykresPktForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TWykresPktForm *WykresPktForm;
//---------------------------------------------------------------------------
#endif
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               Polozenia->AktualnaLista->punkty.erase(iter);
                   /*if(!RadioZaznWiel->Checked)*/
                   // iter==iterKoniec gdy nic nie usuni�to
                     if(iter!=iterKoniec) Polozenia->Odznacz(t); //by�o Zaznacz(t,0);
                   iter=Polozenia->AktualnaLista->punkty.begin();
                   //for(int i=0;i < Polozenia->lista.size() ;i++)// != Polozenia->lista.end())
                    // Form1->Memo1->Lines->Add(Polozenia->lista.pop_front());
                   while(iter!=Polozenia->AktualnaLista->punkty.end())
                   {
                     Form1->Memo1->Lines->Add(*iter++);
                   }
                   Polozenia->rysuj();
//                   Invalidate();
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
        T.printf("Aktualne Po�o�enie %d %d (%.3f, %.3f)",X,Y,Polozenia->DajWartX(X),Polozenia->DajWartY(Y));
        StatusBar1->SimpleText=T;
        StatusBar1->SimplePanel=true;
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::ZaznClick(TObject *Sender)
{
     if(Polozenia->NumerPrzejscia==0)
     {
       Application->MessageBox("Zdefiniuj Przejscie","Uwaga",MB_OK);
       Zazn->Down=false;
       return;
     }
      ListSklad tym;
      Polozenia->Przejscia.insert(Polozenia->AktualnaLista,tym);

}
//---------------------------------------------------------------------------


void __fastcall TWykresPktForm::Shape1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
if( ColorDialog1->Execute())
       Shape1->Brush->Color=ColorDialog1->Color;        
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::ToolButton2Click(TObject *Sender)
{
  if(Panel1->Visible)
      Panel1->Visible=false;
  else
      Panel1->Visible=true;



}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::Button1Click(TObject *Sender)
{
  try
  {
     try
     {
      ListSklad TymList;
      static OstNumer=0;
      std::list<ListSklad>::iterator ItSklad;
      ItSklad=Polozenia->Przejscia.begin();
      while(ItSklad!=Polozenia->Przejscia.end())     // Szukamy czy ju� wpisane
      {
        if(ItSklad->Przejscie1==Edit1->Text.ToInt() &&
           ItSklad->Przejscie2==Edit2->Text.ToInt())
        {
       //   TymList.Kolor=Shape1->Brush->Color;
          Polozenia->AktualnaLista= ItSklad;
          Polozenia->AktualnaLista->Kolor=Shape1->Brush->Color;
          Polozenia->NumerPrzejscia==ItSklad->numer;
          Edit4->Text=ItSklad->numer;
    //      Polozenia->lista.assign(ItSklad.begin(),ItSklad.end());
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
      Polozenia->Przejscia.insert(Polozenia->Przejscia.begin(),TymList);    //wstaw now� list� na pocz�tek
      Polozenia->AktualnaLista= Polozenia->Przejscia.begin();               // ustaw iterator r�wnie� na pocz�tek
      Polozenia->NumerPrzejscia=OstNumer;
      Edit4->Text=OstNumer;
      Edit1->Enabled=false;
      Edit2->Enabled=false;
      Panel1->Visible=false;
       Edit3->Font->Color=Shape1->Brush->Color;
      Edit3->Text= Edit1->Text +" -> " + Edit2->Text;
     }
     catch( ... )
     {
        ShowMessage("Prawdopodobnie: nie podano numer�w przej��\n"
                    "Ich zdefiniowanie umo�liwa przycisk \"Nowy\" ");
      //  Panel1->Visible=false;
     }
 }
 __finally
 {
   Panel1->Visible=false;
 }
}
//---------------------------------------------------------------------------


void __fastcall TWykresPktForm::UpDown1Click(TObject *Sender,
      TUDBtnType Button)
{
  //  static MaxNr=-MaxInt;
  //  static MinNr=1;
  //  int poz;
   // std:list<ListSklad>::iterator iter=Polozenia->Przejscia.begin();
 //   if(Polozenia->NumerPrzejscia>MaxNr) MaxNr=Polozenia->NumerPrzejscia;
 //   if(Polozenia->NumerPrzejscia<MinNr) MinNr=Polozenia->NumerPrzejscia;
 //   UpDown1->Min=MinNr;
//    UpDown1->Max=MaxNr;
 //   if(UpDown1->Position==0) UpDown1->Position=Polozenia->NumerPrzejscia;
 /*   poz=UpDown1->Position;
    switch(Button)
     {
        case btNext:  poz++;
                     if( poz> MaxNr)
                     {
                       poz=MinNr;
                      // UpDown1->Position=MinNr;
                     }
                     break;

        case btPrev: poz--;
                     if(poz< MinNr)
                     {
                       poz=MaxNr;
                      // UpDown1->Position=MaxNr;
                     }
                        break;

     }
     Label4->Caption=poz;
     UpDown1->Position=poz;
     while(iter!=Polozenia->Przejscia.end())
     {
      if(iter->numer==poz)
      {
        Polozenia->AktualnaLista=iter;
        Shape1->Brush->Color=iter->Kolor;
        Edit1->Text=iter->Przejscie1;
        Edit2->Text=iter->Przejscie2;
       // prba= Edit1->Text +" -> " + Edit2->Text;
        Edit3->Text= Edit1->Text +" -> " + Edit2->Text;
        break;
      }
      iter++;
     }
        */
}
//---------------------------------------------------------------------------


void __fastcall TWykresPktForm::Button2Click(TObject *Sender)
{
  Edit1->Enabled=true;
  Edit2->Enabled=true;

}
//---------------------------------------------------------------------------


void __fastcall TWykresPktForm::Label5Click(TObject *Sender)
{
   Label5->Caption=UpDown1->Position;
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::UpDown1Changing(TObject *Sender,
      bool &AllowChange)
{
  if(Polozenia->NumerPrzejscia==0) AllowChange=false;
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::Edit4Change(TObject *Sender)
{
  static MaxNr=-MaxInt;
  static MinNr=MaxInt;
    int poz;
    std:list<ListSklad>::iterator iter=Polozenia->Przejscia.begin();
   if(Polozenia->NumerPrzejscia>MaxNr) MaxNr=Polozenia->NumerPrzejscia;
   if(Polozenia->NumerPrzejscia<MinNr) MinNr=Polozenia->NumerPrzejscia;
   UpDown1->Min=MinNr;
   UpDown1->Max=MaxNr;
    if(UpDown1->Position==0) UpDown1->Position=Polozenia->NumerPrzejscia;
    poz=UpDown1->Position;

     Label4->Caption=poz;
    // UpDown1->Position=poz;
     while(iter!=Polozenia->Przejscia.end())
     {
      if(iter->numer==poz)
      {
        Polozenia->AktualnaLista=iter;
        Shape1->Brush->Color=iter->Kolor;
        Edit1->Text=iter->Przejscie1;
        Edit2->Text=iter->Przejscie2;
       // prba= Edit1->Text +" -> " + Edit2->Text;
        Edit3->Font->Color=iter->Kolor;
        Edit3->Text= Edit1->Text +" -> " + Edit2->Text;
        break;
      }
      iter++;
     }

}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::ToolButton3Click(TObject *Sender)
{
  if(Polozenia->NumerPrzejscia!=0 && SaveDialog1->Execute())
  {
    FILE* zapis;
    zapis=fopen(SaveDialog1->FileName.c_str(), "wt");
    std::list<int>::iterator iterWew;
    std::list<ListSklad>::iterator iter;
    iter=Polozenia->Przejscia.begin();
    //Liczenie og�lnej liczby przej��
    int LiczbaPrzejsc = 0;
    while(iter!=Polozenia->Przejscia.end())
     {
       iterWew=iter->punkty.begin();
       while(iterWew!=iter->punkty.end())
       {
        LiczbaPrzejsc++;
        iterWew++;
       }
      iter++;
    }
    fprintf(zapis,"Liczba wprowadzonych punkt�w\n%d \n\n\n",LiczbaPrzejsc);
    iter=Polozenia->Przejscia.begin();
    double theta,phi,frq;
    theta=ETheta->Text.ToDouble();
    phi=EPhi->Text.ToDouble();
    frq=Efrq->Text.ToDouble();

     while(iter!=Polozenia->Przejscia.end())
     {
       iterWew=iter->punkty.begin();
       while(iterWew!=iter->punkty.end())
       {
       switch(StalyKat->ItemIndex)
       {
        case 0: fprintf(zapis,"%.2f  %.2f  1  %.2f",theta, // sta�a Theta
                Polozenia->DajDaneX(*iterWew)+phi,frq); // 1 = numer bloku
                break;
        case 1 :  fprintf(zapis,"%.2f  %.2f  1  %.2f",
                Polozenia->DajDaneX(*iterWew)+theta,phi,frq);
                // 1 = numer bloku, sta�e Phi
                break;
       }
        fprintf(zapis,"\n1 \n");        // liczba przejsc
        fprintf(zapis,"%d %d %.2f 1\n\n",iter->Przejscie1,
              iter->Przejscie2, Polozenia->DajPole(*iterWew)*10); //
        iterWew++;
       }
      iter++;
    }
    fclose(zapis);
  }
}
//---------------------------------------------------------------------------


void __fastcall TWykresPktForm::ToolButtonKasujAktSerieClick(TObject *Sender)
{
 
 if(Application->MessageBox("Czy naprawd� chcesz usun�� ca�� seri� danych?",
                             "Uwaga!!!",MB_YESNO)==IDNO) return;
    std::list<int>::iterator iterWew;
    iterWew=Polozenia->AktualnaLista->punkty.begin();
       while(iterWew!=Polozenia->AktualnaLista->punkty.end())
       {
         Polozenia->Odznacz(*iterWew);
         iterWew++;
       }
       Polozenia->AktualnaLista->punkty.erase(Polozenia->AktualnaLista->punkty.begin(),Polozenia->AktualnaLista->punkty.end());
       Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TWykresPktForm::ToolButtonKasujWszystkoClick(
      TObject *Sender)
{
 if(Application->MessageBox("Czy naprawd� chcesz usun�� wszystkie dane?",
                             "Uwaga!!!",MB_YESNO)==IDNO) return;
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
    Invalidate();
}
//---------------------------------------------------------------------------


