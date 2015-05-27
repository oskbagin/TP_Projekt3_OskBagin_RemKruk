#include "stdAfx.h"
#include "Bloczek.h"

Bloczek::Bloczek(void)
{
	Bloczek(Trojkat, 0, 0);
}

Bloczek::Bloczek(Ksztalty shape, int X, int Y)
{
	zlapany=false;
	pozX=X;	pozY=Y;
	ksztalt=shape;
}

bool Bloczek::zmienZlapany(bool z)
{
	this->zlapany=z;
	return z;
}

Bloczek::~Bloczek(void)
{
}
