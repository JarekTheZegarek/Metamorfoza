//---------------------------------------------------------------------------

#ifndef RamkaOpcjeH
#define RamkaOpcjeH
#include "StrukturaOpcje.h"
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

class TOknoOpcje : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *Zapisywanie;
        TCheckBox *LiczDanychCheckBox;
        TGroupBox *GroupAlternatywne;
        TLabel *LabelAlternatywa;
        TRadioGroup *SeparatoryKolumn;
        TEdit *EditInneZnaki;
        TTabSheet *Wykres;
        TLabel *Label1;
        TEdit *EditNumerPliku;
        TUpDown *UpDown1;
        TGroupBox *GroupBox1;
        TCheckBox *PokazPunkty;
        TGroupBox *GroupBox2;
        TComboBox *SymbDoswiadczalny;
        TComboBox *RozmDoswiadczalny;
        TGroupBox *GroupBox3;
        TComboBox *SymbZaznaczony;
        TComboBox *RozmZaznaczony;
        TButton *ButtonOpcje_Ok;
        TButton *ButtonAnuluj_Opcje;
        TButton *ButtonPomoc_Opcje;
        TRadioGroup *OpDodatDanych;
        TTabSheet *TabSheet1;
        TEdit *Czulosc;
        TLabel *Label2;
        TUpDown *UpDown2;
        TRadioGroup *Jednostki;
        void __fastcall ButtonOpcje_OkClick(TObject *Sender);
        void __fastcall ButtonAnuluj_OpcjeClick(TObject *Sender);
        void __fastcall ButtonPomoc_OpcjeClick(TObject *Sender);
        void __fastcall SeparatoryKolumnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TOknoOpcje(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOknoOpcje *OknoOpcje;
//---------------------------------------------------------------------------
#endif
