#pragma once
#include <string>
using namespace std;

enum Ksztalty { Trojkat, Kolko, Kwadrat };

class Bloczek
{
	Ksztalty ksztalt;
	int pozX, pozY;
	bool zlapany;
public:
	Bloczek(void);	
	Bloczek(Ksztalty, int, int);
	Ksztalty getKsztalt() { return ksztalt; }
	bool getZlapany() { return zlapany; }
	int getX() { return pozX; }
	int getY() { return pozY; }
	int zmienX(int X) { pozX=X; return X; }
	int zmienY(int Y) { pozY=Y; return Y; }
	bool zmienZlapany(bool z);

	~Bloczek(void);
};

