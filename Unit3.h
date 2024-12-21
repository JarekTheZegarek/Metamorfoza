//---------------------------------------------------------------------------

#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
        TPaintBox *PaintBox1;
        TStatusBar *StatusBar1;
        TToolBar *ToolBar1;
        TToolButton *ToolButton1;
        TImageList *ImageList1;
        TToolButton *LL;
        TToolButton *LP;
        TToolButton *GL;
        TToolButton *GP;
        TToolButton *wykres;
        TToolButton *Powieksz;
        TToolButton *ToolButton2;
        TToolButton *prosta;
        TPanel *Panel1;
        TEdit *Edit1;
        TUpDown *UpDown1;
        TLabel *Label1;
        TToolButton *Opcje;
        TButton *Button1;
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall ToolButton1Click(TObject *Sender);
        void __fastcall PaintBox1MouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall PaintBox1DragOver(TObject *Sender, TObject *Source,
          int X, int Y, TDragState State, bool &Accept);
        void __fastcall PowiekszClick(TObject *Sender);
        void __fastcall PaintBox1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton2Click(TObject *Sender);
        void __fastcall PaintBox1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall OpcjeClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
         void rysuj(void);
         void co_rysowac(void);
         bool sprawdz();
         void rysujLL(void);
         void UstawInd();
         void rysujGL();
         void rysujPG();
         void rysujPL();
        __fastcall TForm3(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
