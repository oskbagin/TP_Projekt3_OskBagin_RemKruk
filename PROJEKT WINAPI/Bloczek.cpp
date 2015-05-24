#include "stdAfx.h"
#include "Bloczek.h"

Bloczek::Bloczek(void)
{
	Bloczek("", 0, 0, 0);
}

Bloczek::Bloczek(string shape, int X, int Y, int ilosc)
{
	zlapany=false;
	pozX=X;	pozY=Y;
	Ksztalt=shape;
	wieza=ilosc;

}

bool Bloczek::zmienZlapany(bool z)
{
	this->zlapany=z;
	return z;
}

Bloczek::~Bloczek(void)
{
}
