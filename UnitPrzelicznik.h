//---------------------------------------------------------------------------

#ifndef UnitPrzelicznikH
#define UnitPrzelicznikH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TPrzelicznik : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *StrGrTabelka;
        TButton *Przelicz;
        TRadioGroup *Opcja;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall OpcjaClick(TObject *Sender);
        void __fastcall PrzeliczClick(TObject *Sender);
        void __fastcall StrGrTabelkaSetEditText(TObject *Sender, int ACol,
          int ARow, const AnsiString Value);
private:	// User declarations
public:		// User declarations
        __fastcall TPrzelicznik(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPrzelicznik *Przelicznik;
//---------------------------------------------------------------------------
#endif
