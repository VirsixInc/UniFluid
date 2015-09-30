#include "stdafx.h"
#include "mbLattice.h"
#include <vector>
#include "mbVector3D.h"
#include "math.h"

mbChargeLattice::mbChargeLattice(unsigned int xDimInit, unsigned int yDimInit,
	unsigned int zDimInit, float xScaleInit, float yScaleInit, float zScaleInit)
	:xDim(xDimInit), yDim(yDimInit), zDim(zDimInit), xScale(xScaleInit), yScale(yScaleInit),
	zScale(zScaleInit), latticeMag(xScaleInit)
{
	allocLattice();
};

void mbChargeLattice::allocLattice() {
	deallocLattice();
	chargeLattice = new float[xDim*yDim*zDim];
};

void mbChargeLattice::deallocLattice() {
	if (chargeLattice)
	{
		delete[] chargeLattice;
		chargeLattice = NULL;
	};
};

mbChargeLattice::~mbChargeLattice() {
	deallocLattice();
	deallocChargeReturn();
};

unsigned int mbChargeLattice::getNodeCount() {
	return chargeNodes.size();
};

void mbChargeLattice::clearChargeNodes() {
	chargeNodes.clear();
};

void mbChargeLattice::addCharge(float x, float y, float z, float charge) {
	chargeNodes.push_back({ x, y, z, charge });
}

void mbChargeLattice::deallocChargeReturn() {
	if (chargeReturn)
	{
		delete[] chargeReturn;
		chargeReturn = NULL;
	};
};

float* mbChargeLattice::getCharges() {
	mbChargeNode node;
	deallocChargeReturn();
	chargeReturn = new float[getNodeCount() * 4];
	int i, index;
	for (i = 0; i < getNodeCount(); i++)
		{
			node = chargeNodes[i];
			index = i * 4;
			chargeReturn[index] = node.x;
			chargeReturn[index + 1] = node.y;
			chargeReturn[index + 2] = node.z;
			chargeReturn[index + 3] = node.charge;
		};
	return chargeReturn;
};

float* mbChargeLattice::getChargeLattice() {
	return chargeLattice;
};

unsigned int mbChargeLattice::getLatticeSize() {
	return xDim*yDim*zDim;
}

void mbChargeLattice::calculateCharge() {
	unsigned int yOffset = xDim;
	unsigned int zOffset = yOffset*(yDim);
	unsigned int x, y, z;
	int i;
	float pwr;
	int size = chargeNodes.size();
	mbChargeNode node;
	unsigned int location;
	for (z = 0; z < zDim; z++)
		for (y = 0; y < yDim; y++)
			for (x = 0; x < xDim; x++)
			{
				pwr = 1.0f;
				for (i = 0; i < size; i++)
				{
					pwr += 1.0f;
				}
				//chargeLattice[z*zOffset + y*yOffset + x] = chargeAtLatticePoint(x, y, z);
				chargeLattice[z*zOffset + y*yOffset + x] = pwr;
			}

};

float mbChargeLattice::getContributionToPointCharge(mbChargeNode node, float xLoc, float yLoc, float zLoc)
{
	mbVector3D posVec = mbVector3D(xLoc, yLoc, zLoc);
	mbVector3D nodeVec = mbVector3D(node.x, node.y, node.z);
	//We want to scale the magnitude to an interval of 0., 1., 
	//latticeMag should be the maximum distance between 2 points in the lattice.
	return getChargeScalar(mbSquaredMag((nodeVec - posVec) / latticeMag));

}

float mbChargeLattice::getChargeScalar(float magnitude) {
	return 1.0f/magnitude;
}

inline float mbChargeLattice::chargeAtLatticePoint(unsigned int x, unsigned int y, unsigned int z) {
	//const float xSpacing = xScale / xDim;
	//const float ySpacing = yScale / yDim;
	//const float zSpacing = zScale / zDim;
	//const float xLoc = x*xSpacing - xScale / 2.0f;
	//const float yLoc = y*ySpacing - yScale / 2.0f;
	//const float zLoc = z*zSpacing - zScale / 2.0f;
	//mbChargeNode node;
	//float contribution;
	//int i;
	//float summedContributions = 0.0f;
	//for (i = 0; i < getNodeCount(); i++)
	//{
	//	node = chargeNodes[i];
	//	//contribution = getContributionToPointCharge(node, xLoc, yLoc, zLoc);
	//	//summedContributions += node.charge;
	//}
	return 1.0f;
};
