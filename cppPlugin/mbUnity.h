#pragma once
#include <map>
#define DllExport   __declspec( dllexport )
#include "mbLattice.h"

extern "C" int DllExport setupLattice(unsigned int xDim, unsigned int yDim, 
	unsigned int zDim, float xScale, float yScale, float zScale);
extern "C" void DllExport removeLattice(int latticeID);
extern "C" void DllExport setLatticeXDim(int latticeID, unsigned int xDim);
extern "C" void DllExport setLatticeYDim(int latticeID, unsigned int yDim);
extern "C" void DllExport setLatticeZDim(int latticeID, unsigned int zDim);
extern "C" unsigned int DllExport getLatticeXDim(int latticeID);
extern "C" unsigned int DllExport getLatticeYDim(int latticeID);
extern "C" unsigned int DllExport getLatticeZDim(int latticeID);
extern "C" void DllExport addChargeToLattice(int latticeID, float x, float y, float z, float charge);
extern "C" float DllExport *getLatticeCharges(int latticeID);
extern "C" void DllExport deallocLatticeChargeReturn(int latticeID);
extern "C" unsigned int DllExport getCurrentLatticeSize(int latticeID);
extern "C" float DllExport *getChargeLattice(int latticeID);
extern "C" void DllExport clearLatticeCharges(int latticeID);
extern "C" void DllExport updateLattice(int latticeID);

class mbLatticeHandler {
public:
	static mbLatticeHandler& getInstance();
	std::map <int, mbChargeLattice*> latticeMap;
	int latticeCount = 0;
	int activeLattices = 0;
	int generateLattice(unsigned int xDim, unsigned int yDim, unsigned int zDim,
		float xScale, float yScale, float zScale);
	void removeLattice(int latticeID);

private:
	mbLatticeHandler(){};
	mbLatticeHandler(mbLatticeHandler const&) {};
	mbLatticeHandler& operator=(mbLatticeHandler const&) {};
	static mbLatticeHandler* realInstance;
};
