//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USERES("Metamorfoza5.res");
USEFORM("metamorfozis_w5.cpp", Form1);
USEFORM("OkienkoSave.cpp", Saves);
USEFORM("OkienkoPowitalne.cpp", Form2);
USEUNIT("Funkcje.cpp");
USE("Metamorfoza5.todo", ToDo);
USEUNIT("ZbiorTablic.cpp");
USEUNIT("termopara.cpp");
USEUNIT("Wykres.cpp");
USEUNIT("StrukturaOpcje.cpp");
USEFORM("RamkaOpcje.cpp", OknoOpcje);
USEUNIT("Eksport.cpp");
USEUNIT("Definicje.cpp");
USEFORM("WykresPunktowy.cpp", WykresPktForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                Application->Initialize();
                Application->Title = "Metamorfoza 2.0";
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->CreateForm(__classid(TSaves), &Saves);
                 Application->CreateForm(__classid(TForm2), &Form2);
                 Application->CreateForm(__classid(TWykresPktForm), &WykresPktForm);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
