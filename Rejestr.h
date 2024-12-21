//---------------------------------------------------------------------------

#ifndef RejestrH
#define RejestrH
//#include "metamorfozis_w5.h"
#include <vcl\registry.hpp>
class TRejestr:public TRegistry
{

	void otworz(bool baza=false);
	void zamknij();
	AnsiString klucz;
	AnsiString klucz_bazowy;
public:
	TRejestr();
	void open(bool baza=false){
		otworz(baza);
	}
	AnsiString odczytStr(AnsiString klucz);
	int odczytInt(AnsiString klucz);
	void zapiszStr(AnsiString klucz,AnsiString wartosc);
	void zapiszInt(AnsiString klucz,int wartosc);
	void UstKlucz(AnsiString kl);
};

extern TRejestr *rejestr;




//---------------------------------------------------------------------------
#endif
