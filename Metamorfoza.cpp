//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("metamorfozis_w5.cpp", Form1);
USEFORM("OkienkoSave.cpp", Saves);
USEFORM("OkienkoPowitalne.cpp", Form2);
USEFORM("RamkaOpcje.cpp", OknoOpcje);
USEFORM("WykresPunktowy.cpp", WykresPktForm);
USEFORM("UnitPrzelicznik.cpp", Przelicznik);
USEFORM("Prezentacja.cpp", FormularzOpcjiPrezentacji);
USEFORM("ksztalt.cpp", Form3);
USEFORM("Test.cpp", FormTest);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                Application->Initialize();
                Application->Title = "Metamorfoza 3.0";
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->CreateForm(__classid(TFormTest), &FormTest);
                 Application->CreateForm(__classid(TSaves), &Saves);
                 Application->CreateForm(__classid(TForm2), &Form2);
                 Application->CreateForm(__classid(TWykresPktForm), &WykresPktForm);
                 Application->CreateForm(__classid(TFormularzOpcjiPrezentacji), &FormularzOpcjiPrezentacji);
                 Application->CreateForm(__classid(TForm3), &Form3);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
