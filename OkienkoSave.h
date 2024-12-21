//---------------------------------------------------------------------------
#ifndef OkienkoSaveH
#define OkienkoSaveH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\FileCtrl.hpp>
#include <vcl\Buttons.hpp>
//---------------------------------------------------------------------------
class TSaves : public TForm
{
__published:	// IDE-managed Components
        TDirectoryListBox *Katalogi;
        TDriveComboBox *DriveComboBox1;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TLabel *Label1;
        TFilterComboBox *FilterComboBox1;
        void __fastcall DriveComboBox1Change(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TSaves(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TSaves *Saves;
//---------------------------------------------------------------------------
#endif
