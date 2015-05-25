#pragma once
#include <string>
using namespace std;

class Bloczek
{
	string Ksztalt;
	int pozX, pozY;
	bool zlapany;
public:
	Bloczek(void);	
	Bloczek(string, int, int);
	string getKsztalt() { return Ksztalt; }
	bool getZlapany() { return zlapany; }
	int getX() { return pozX; }
	int getY() { return pozY; }
	int zmienX(int X) { pozX=X; return X; }
	int zmienY(int Y) { pozY=Y; return Y; }
	bool zmienZlapany(bool z);

	~Bloczek(void);
};

