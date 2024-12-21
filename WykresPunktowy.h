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
#include <map>
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
        TButton *Button1;
        TSplitter *Splitter1;
        TSplitter *Splitter2;
        TLabel *Label3;
        TUpDown *UpDown1;
        TButton *Button2;
        TLabel *Label4;
        TLabel *Label5;
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
        TGroupBox *GroupBox2;
        TLabel *Label9;
        TLabel *Label10;
        TEdit *SymbolePol;
        TEdit *SymbolePrzejsc;
        TUpDown *UpDown2;
        TUpDown *UpDown3;
        TGroupBox *GroupBox3;
        TEdit *NBlock;
        TLabel *Label11;
        TToolButton *KasujPunkty;
        TComboBox *ComboBoxPunkty;
        TComboBox *ComboBoxZaznaczenia;
        TMenuItem *Test1;
        TShape *ShapeZaznaczenie;
        TShape *ShapePunkt;
        TShape *KolorPunktu;
        TShape *KolorPrzej;
        TMenuItem *Opcjeprzej1;
        TMenuItem *ZapisWeil1;
        TMenuItem *ZapiszPodzia1;
        TMenuItem *PojedynczyPlik1;
        TMenuItem *Zpodzia1;
        TMenuItem *Pojedynczyplik2;
        TMenuItem *Zpodziaemnaprzejcia1;
        TPanel *PanelZapisWielokrotny;
        TComboBox *ComboBoxPrzejscia;
        TGroupBox *GroupBoxZapisWiel;
        TButton *ButtonZapisWielokrotny;
        TUpDown *UpDownMenuZmianaPrzejsc;
        TPaintBox *PaintBoxSymbol;
        TButton *ButtonAnulujZapisWielokrotny;
        TRadioGroup *RadioGroupLiczbaPrzejsc;
        TMenuItem *Edycja1;
        TMenuItem *Cofnijoperacj1;
        TScrollBox *ScrollBox1;
        TToolButton *ToolButtonPowieksz;
        void __fastcall PaintBoxPktPaint(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall Od1Click(TObject *Sender);
        void __fastcall PaintBoxPktMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall PaintBoxPktMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall ZaznClick(TObject *Sender);
        void __fastcall ShapeZaznaczenieMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton2Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Label5Click(TObject *Sender);
        void __fastcall ToolButton3Click(TObject *Sender);
        void __fastcall ToolButtonKasujAktSerieClick(TObject *Sender);
        void __fastcall ToolButtonKasujWszystkoClick(TObject *Sender);
        void __fastcall Test1Click(TObject *Sender);
        void __fastcall ShapePunktMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall KolorPunktuMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall KolorPrzejMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ComboBoxPunktyChange(TObject *Sender);
        void __fastcall ComboBoxZaznaczeniaChange(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall Splitter1Paint(TObject *Sender);
        void __fastcall Zpodzia1Click(TObject *Sender);
        void __fastcall UpDown1Click(TObject *Sender, TUDBtnType Button);
        void __fastcall ButtonZapisWielokrotnyClick(TObject *Sender);
        void __fastcall UpDownMenuZmianaPrzejscClick(TObject *Sender,
          TUDBtnType Button);
        void __fastcall PaintBoxSymbolPaint(TObject *Sender);
        void __fastcall ButtonAnulujZapisWielokrotnyClick(TObject *Sender);
        void __fastcall ComboBoxPrzejsciaChange(TObject *Sender);
        void __fastcall Cofnijoperacj1Click(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall KasujPunktyClick(TObject *Sender);
        void __fastcall ToolButtonPowiekszClick(TObject *Sender);
        void __fastcall FormKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Pojedynczyplik2Click(TObject *Sender);
private:	// User declarations
        bool CzyNowePrzejscie;         // true, jezeli jest proba dopisania nowego przejscia
        std::map<AnsiString, int> MapaPrzejsc; // Napis w ComboBoxie, numer przejscia;
public:		// User declarations
        TJWykresyPunktow* Polozenia;
        void ZapisWeil(int i, AnsiString Nazwa);   // i - numer przejscia 0 oznacza wszystkie;
        __fastcall TWykresPktForm(TComponent* Owner);
       void ZapisText(int i, AnsiString Nazwa);   // i - numer przejscia 0 oznacza wszystkie;
};
//---------------------------------------------------------------------------
extern PACKAGE TWykresPktForm *WykresPktForm;
//---------------------------------------------------------------------------
#endif
