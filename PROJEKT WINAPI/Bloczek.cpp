#include "stdAfx.h"
#include "Bloczek.h"

Bloczek::Bloczek(void)
{
	zlapany = false;
}

Bloczek::Bloczek(string shape, int X, int Y)
{
	zlapany=false;
	pozX=X;	pozY=Y;
	Ksztalt=shape;
}

bool Bloczek::zmienZlapany(bool z)
{
	this->zlapany=z;
	return z;
}

Bloczek::~Bloczek(void)
{
}
