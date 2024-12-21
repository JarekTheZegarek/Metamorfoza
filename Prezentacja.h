//---------------------------------------------------------------------------

#ifndef PrezentacjaH
#define PrezentacjaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormularzOpcjiPrezentacji : public TForm
{
__published:	// IDE-managed Components
        TEdit *Edit1;
        TLabel *Label1;
        TUpDown *UpDown1;
        TTimer *Timer1;
        TCheckBox *CheckBox1;
        TButton *Button1;
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormularzOpcjiPrezentacji(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormularzOpcjiPrezentacji *FormularzOpcjiPrezentacji;
//---------------------------------------------------------------------------
#endif
