#include "StdAfx.h"
#include "Klocuszek.h"


Klocuszek::Klocuszek(void)
{
	Klocuszek(Figura, 0, 0);
}

Klocuszek::Klocuszek(Ksztalty Ksztalt, int X, int Y) : ksztalt(Ksztalt), pozX(X), pozY(Y+43)
{
	czyZlapany=false;
	ktoryNaWiezy=0;
	moznaRuszyc=true;
}

Klocuszek::~Klocuszek(void)
{
}
