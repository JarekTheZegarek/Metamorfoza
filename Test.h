//---------------------------------------------------------------------------

#ifndef TestH
#define TestH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormTest : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo1;
        TLabel *Label1;
private:	// User declarations
public:		// User declarations
        __fastcall TFormTest(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTest *FormTest;
//---------------------------------------------------------------------------
#endif
