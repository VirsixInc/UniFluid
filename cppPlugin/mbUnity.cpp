// mbUnity.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "mbUnity.h"
#include "mbLattice.h"
#include "mbVector3D.h"

int setupLattice(unsigned int xDim, unsigned int yDim, unsigned int zDim,
	float xScale, float yScale, float zScale) {
	mbLatticeHandler& latticeHandler = mbLatticeHandler::getInstance();
	return latticeHandler.generateLattice(xDim, yDim, zDim, xScale, yScale, zScale);
};

void removeLattice(int latticeID) {
	mbLatticeHandler& latticeHandler = mbLatticeHandler::getInstance();
	latticeHandler.removeLattice(latticeID);
};

mbChargeLattice& getLatticeFromID(int latticeID) {
	return (*mbLatticeHandler::getInstance().latticeMap[latticeID]);
};

void updateCharge(int latticeID) {
	mbChargeLattice& lattice = getLatticeFromID(latticeID);
	lattice.calculateCharge();
};

void setLatticeXDim(int latticeID, unsigned int xDim) {
	mbChargeLattice& lattice = getLatticeFromID(latticeID);
	lattice.xDim = xDim;
	lattice.allocLattice();
};

void setLatticeYDim(int latticeID, unsigned int yDim) {
	mbChargeLattice& lattice = getLatticeFromID(latticeID);
	lattice.yDim = yDim;
	lattice.allocLattice();
};

void setLatticeZDim(int latticeID, unsigned int zDim) {
	mbChargeLattice& lattice = getLatticeFromID(latticeID);
	lattice.zDim = zDim;
	lattice.allocLattice();
};

unsigned int getLatticeXDim(int latticeID) {
	mbChargeLattice& lattice = getLatticeFromID(latticeID);
	return lattice.xDim;
};

unsigned int getLatticeYDim(int latticeID) {
	mbChargeLattice& lattice = getLatticeFromID(latticeID);
	return lattice.yDim;
};

unsigned int getLatticeZDim(int latticeID) {
	mbChargeLattice& lattice = getLatticeFromID(latticeID);
	return lattice.zDim;
};

void addChargeToLattice(int latticeID, float x, float y, float z, float charge) {
	mbChargeLattice& lattice = getLatticeFromID(latticeID);
	lattice.addCharge(x, y, z, charge);
};

float* getLatticeCharges(int latticeID) {
	mbChargeLattice& lattice = getLatticeFromID(latticeID);
	return lattice.getCharges();
}

unsigned int getCurrentLatticeSize(int latticeID) {
	mbChargeLattice& lattice = getLatticeFromID(latticeID);
	return lattice.getLatticeSize();
}

float* getChargeLattice(int latticeID) {
	mbChargeLattice& lattice = getLatticeFromID(latticeID);
	return lattice.getChargeLattice();
}

void deallocLatticeChargeReturn(int latticeID) {
	mbChargeLattice& lattice = getLatticeFromID(latticeID);
	lattice.deallocChargeReturn();
}

void addCharge(int latticeID, float x, float y, float z, float charge) {
	getLatticeFromID(latticeID).addCharge(x, y, z, charge);
};

void clearLatticeCharges(int latticeID) {
	getLatticeFromID(latticeID).clearChargeNodes();
}

mbLatticeHandler* mbLatticeHandler::realInstance = NULL;

mbLatticeHandler& mbLatticeHandler::getInstance() {
	if (!realInstance)
		realInstance = new mbLatticeHandler;
	return (*realInstance);
};

int mbLatticeHandler::generateLattice(unsigned int xDim, unsigned int yDim, unsigned int zDim,
	float xScale, float yScale, float zScale) {
	int newIndex = latticeCount;
	latticeMap[newIndex] = new mbChargeLattice(xDim, yDim, zDim, xScale, yScale, zScale);
	latticeCount += 1;
	activeLattices += 1;
	return newIndex;
};

void mbLatticeHandler::removeLattice(int latticeID) {
	latticeMap.erase(latticeID);
	activeLattices -= 1;
	if (activeLattices == 0)
		latticeCount = 0;
};
