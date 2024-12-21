//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "termopara.h"
#include "metamorfozis_w5.h"
#include <stdio.h>
#include <iostream.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

// Zao¿enie: dane s¹ odczytwyane z pliku term.txt
// W pliku tym w pierwszej kolumnie znajduj¹ siê temeratury a
// w drugiej napiêcia. Dane s¹ uporz¹dkowane rosn¹co.
// Zak³ada siê, ¿e temperatury s¹ w skali Celsjusza

Term termopara;


bool Term::wczytaj_dane()
{
    FILE* wczytaj;
    char drive[_MAX_DRIVE];
    char directory[_MAX_DIR];
    char filename[_MAX_FNAME];
    char extension[_MAX_EXT];
    char path[_MAX_PATH];
    _splitpath(ParamStr(0).c_str(),drive,directory,filename,extension);
    _makepath(path,drive,directory,"term",".txt");

   if(!(wczytaj = fopen(path,"rt")))
                return false;
   char tym[200];      //200 maksymalna liczba znaków w pojedynczej linii
   char* aktznak= tym;
   while(fgets(tym,200,wczytaj))
   {
      if(tym[0]=='#')
      {
         tym[0]=' ';
         while(*aktznak!='\n') aktznak++;
         *aktznak=' ';
         aktznak=tym;
         Form1->Memo1->Lines->Add(tym);  // Wypisz komentarz na Formularzu
         continue; //linia zaczynaj¹ca siê od # jestkomentarzem
      }
           ilosc++;
   }
   fseek(wczytaj,0,SEEK_SET);
   int CC;
  try{
     Dane  = new float*[ilosc];
     for(int i=0;i< ilosc;i++)
       {
          while((CC=fgetc(wczytaj))=='#')      // pocz¹tek kometnarza
               fgets(tym,200,wczytaj);
          ungetc(CC,wczytaj);
          Dane[i]= new float[2];
          fscanf(wczytaj,"%f %f",&Dane[i][0],&Dane[i][1]);
       }
     return true;
  }
  catch(std::bad_alloc)
  {
       Application->MessageBox("B³¹d przydzia³u pamiêci","Struktura termopara",MB_OK);
       return false;
  }
}

float Term::CelsjuszNap(float T)
{
     if( T<DajTemp(0)  || T >DajTemp(ilosc-1))
  {
       //Application->MessageBox("Wartoœæ temperatury poza zakresem","Metoda Term::CelsjuszNap",MB_OK);
       throw ZakresNap();
       return -3000;
  }
  zacznij_iteracje();
  int i;
  while(i=iteruj())
  {
     if(T<=DajTemp(i))
     {
        return  (DajNapiecie(i)-DajNapiecie(i-1))*(T-DajTemp(i-1))/(DajTemp(i)-DajTemp(i-1)) + DajNapiecie(i-1);
     }
  }
    ShowMessage("Z jakichœ powodów nie powiod³o siê przeliczanie temperatury na napiêcie");
  return -3000;  // w razie niepowodzenia;
}


float Term::TempCelsjusz(float Nap)
{
 // if( Nap<-5.96  || Nap >19.63)
   if( Nap<DajNapiecie(0)  || Nap >DajNapiecie(ilosc-1))
  {
       Application->MessageBox("Wartoœæ napiêcia poza zakresem","Metoda Term::TempCelsjusz",MB_OK);
       return -3000;
  }
  zacznij_iteracje();
  int i;
  while(i=iteruj())
  {
     if(Nap<=DajNapiecie(i))
     {
        return  (DajTemp(i)-DajTemp(i-1))*(Nap-DajNapiecie(i-1))/(DajNapiecie(i)-DajNapiecie(i-1)) + DajTemp(i-1);
     }
  }
  return -3000;  // w razie niepowodzenia;
}

 /*
 If ((c.Value < -5.96) Or (c.Value > 19.63)) Then
c.Select
MsgBox ("Wartoœæ poza zakresem!")
Else
    For Each komórka In Worksheets("Arkusz1").Range("B3:B63").Cells
         If c.Value <= komórka.Value Then
         '10 jest d³ugoœci¹ przedzia³u (temperatury s¹ dane co 10)
            TemperaturaC = 10 * Abs(c.Value - komórka.Offset(-1, 0).Value) / Abs(komórka.Value - komórka.Offset(-1, 0).Value) + komórka.Offset(-1, -1).Value
            TemperautraK = TemperaturaC + 273.15
            c.Offset(0, 1).Value = TemperaturaC
            c.Offset(0, 2).Value = TemperautraK
          Exit For
          End If
    Next
End If
Next
Zakres.Select
End Sub
 */
