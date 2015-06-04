#pragma once

enum Ksztalty { Figura, Kolko, Kwadrat, Trojkat };

class Klocuszek
{
	int pozX;
	int	pozY;
	int ktoryNaWiezy;
	bool czyZlapany;
	bool moznaRuszyc;
	Ksztalty ksztalt;

public:
	// akcesory 
	int getX() { return pozX; }
	int getY() { return pozY; }
	int getKNW() { return ktoryNaWiezy; }
	bool get_moznaRuszyc() { return moznaRuszyc; }
	bool get_czyZlapany() { return czyZlapany; }
	Ksztalty getKsztalt() { return ksztalt; }
	// mutarory
	int setX(int X) { pozX=X; return pozX; }
	int setY(int Y) { pozY=Y; return pozY; }
	int set_KNW(int KNW) { ktoryNaWiezy=KNW; return ktoryNaWiezy; }
	bool set_moznaRuszyc(bool mR) { moznaRuszyc=mR; return moznaRuszyc; }
	bool set_czyZlapany(bool czyZ) { czyZlapany=czyZ; return czyZlapany; }
	// konstruktory
	Klocuszek(void);
	Klocuszek(Ksztalty, int, int);
	// destruktor
	~Klocuszek(void);
};

