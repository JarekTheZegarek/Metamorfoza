//---------------------------------------------------------------------------
#ifndef OkienkoPowitalneH
#define OkienkoPowitalneH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------

class TForm2 : public TForm
{
__published:	// IDE-managed Components
        TComboBox *ComboBox1;
        TButton *Button1b;
        TButton *Button2b;
        TButton *Button3b;
        void __fastcall Button1bClick(TObject *Sender);
        void __fastcall Button2bClick(TObject *Sender);
        void __fastcall Button3bClick(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern /*PACKAGE*/ TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
