#pragma once
#include <vector>

struct mbChargeNode {
	float x, y, z, charge;
};


class mbChargeLattice {
public:
	unsigned int xDim, yDim, zDim;
	bool chargeAllocated = false;
	float xScale, yScale, zScale;
	float latticeMag;
	float *chargeLattice = NULL;
	std::vector<mbChargeNode> chargeNodes;
	mbChargeLattice(unsigned int xDimInit, unsigned int yDimInit,
		unsigned int zDimInit, float xScaleInit, float yScaleInit, float zScaleInit);
	~mbChargeLattice();
	void clearChargeNodes();
	void allocLattice();
	void deallocLattice();
	unsigned int getNodeCount();
	void addCharge(float x, float y, float z, float charge);
	void calculateCharge();
	inline float chargeAtLatticePoint(unsigned int x, unsigned int y, unsigned int z);
	float getContributionToPointCharge(mbChargeNode node, float xLoc, float yLoc, float zLoc);
	float* getCharges();
	float getChargeScalar(float magnitude);
	void deallocChargeReturn();
	unsigned int mbChargeLattice::getLatticeSize();
	float* mbChargeLattice::getChargeLattice();
private:
	float* chargeReturn = NULL;
};