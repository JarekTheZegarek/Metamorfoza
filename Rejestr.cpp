//---------------------------------------------------------------------------


#pragma hdrstop

#include "Rejestr.h"
#include "metamorfozis_w5.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//#include "metamorfozis_w5.cpp "


TRejestr *rejestr = new TRejestr;

TRejestr::TRejestr(): TRegistry()
{

  char drive[_MAX_DRIVE];
  char directory[_MAX_DIR];
  char filename[_MAX_FNAME];
  char extension[_MAX_EXT];
  char path[_MAX_PATH];
  _splitpath(ParamStr(0).c_str(),drive,directory,filename,extension);
  _makepath(path,drive,directory,0,0);
  char ShortPath[_MAX_PATH];
  GetShortPathName(ParamStr(0).c_str(),ShortPath,_MAX_PATH);
  AnsiString KrotkaNazwa = ShortPath;
  RootKey=HKEY_CLASSES_ROOT;
  if(!KeyExists(".met"))
  {
		OpenKey(".met",true);
		WriteString("","MetamorfozaFile");
		OpenKey("\\MetamorfozaFile",true);
		WriteString("","Dokument programu do obróbki widm EPR (Metamorfoza)");
		CreateKey("DefaultIcon");
		OpenKey("DefaultIcon",false);
		WriteString("",KrotkaNazwa+",0");
		OpenKey("\\MetamorfozaFile\\shell",true);
		OpenKey("open",true);
		OpenKey("command",true);
		WriteString("",AnsiString("\"")+KrotkaNazwa+AnsiString("\" \"%1\""));
		RootKey=HKEY_LOCAL_MACHINE;
		OpenKey("\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths",false);
		CreateKey(AnsiString(filename)+AnsiString(extension));
		OpenKey(AnsiString(filename)+AnsiString(extension),false);
		WriteString("",KrotkaNazwa);
		WriteString("Path",KrotkaNazwa);
  }
  RootKey=HKEY_CURRENT_USER;
  if(KeyExists("Metamorfoza"))
  {
	 OpenKey("Metamorfoza",false);
	if(!ValueExists("Obsolete"))
	{
	  MoveKey("\\Metamorfoza","\\Software\\Metamorfoza",false);
	  OpenKey("\\Metamorfoza",false);
	  WriteString("Obsolete","Yes");
	  CloseKey();
	}
  }
  OpenKey("\\Software",false);
  OpenKey("Metamorfoza",true);
  klucz="\\Software\\Metamorfoza";
  klucz_bazowy=klucz;
 };

 void TRejestr::otworz(bool baza)
 {
	RootKey=HKEY_CURRENT_USER;
	if(baza)
		OpenKey(klucz_bazowy,false);
	else
		OpenKey(klucz,true);
 }

 void TRejestr::zamknij()
 {
	CloseKey();
 }

 AnsiString  TRejestr::odczytStr(AnsiString kl)
 {
	 otworz();
	 AnsiString tym = ReadString(kl);
	 zamknij();
	 return tym;
 }

 int  TRejestr::odczytInt(AnsiString kl)
 {
         int tym;
         AnsiString tymStr;
	 otworz();
         try{
	 tym = ReadInteger(kl);
         }
         catch(ERegistryException&)
         {
           if(ValueExists(kl))
           {
             try {
                        tymStr = ReadString(kl);
                        tym = tymStr.ToInt();
                        DeleteValue(kl);
                        WriteInteger(kl,tym);
             }
             catch(EConvertError&)
             {
                tymStr = "Niestety odczyt z rejestru wartoœci ca³kowitej\""+kl;

                tymStr+="\" nie powiód³ siê. \nProszê siê skontaktowaæ z twórc¹ programu";
                tymStr+="Mo¿na te¿ wpisaæ wartoœæ domyœln¹ 0 (zalecane). \n";
                tymStr+="\nCzy wpisaæ wartoœæ domyœln¹? ";
                if(Application->MessageBox(tymStr.c_str(),"Uwaga",MB_YESNO)== ID_YES)
                {
                       WriteInteger(kl,0);
                }
                else
                        throw;
             }
           }
           else
           {
               tymStr = "Niestety podana wartoœci ca³kowita\""+kl;

                tymStr+="\" nie jest zapisana w rejestrze. \nProszê siê skontaktowaæ z twórc¹ programu";
                tymStr+="\nMo¿na te¿ wpisaæ wartoœæ domyœln¹ 0 (zalecane). \n";
                tymStr+="\nCzy wpisaæ wartoœæ domyœln¹? ";
                if(Application->MessageBox(tymStr.c_str(),"Uwaga",MB_YESNO)== ID_YES)
                {
                       WriteInteger(kl,0);
                }
                else
                        throw;
           }
        }
	 zamknij();
	 return tym;
 }

 void  TRejestr::zapiszStr(AnsiString kl, AnsiString wartosc)
 {
	 otworz();
	 WriteString(kl,wartosc);;
	 zamknij();
 }

 void  TRejestr::zapiszInt(AnsiString kl, int wartosc)
 {
	 otworz();
	 WriteInteger(kl,wartosc);;
	 zamknij();
 }
void TRejestr::UstKlucz(AnsiString k)
{
	if(k.Pos("\\")==1 )  // czyli klucz zaczyna siê od ukoœnika podwójnego, czyli trzeba za³o¿yæ ca³kiem nowy klucz (a nie kontynuacjê)
	{
		klucz = k;
	}
	else
	{
		klucz+="\\";
		klucz+=k;
	}
}
 //TRejestr *rejestr = new TRejestr;
