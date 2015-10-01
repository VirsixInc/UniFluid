#include "stdafx.h"
#include "mbLattice.h"
#include <vector>
#include "mbVector3D.h"
#include "math.h"

const int X_AXIS = 0;
const int Y_AXIS = 1;
const int Z_AXIS = 2;

mbChargeLattice::mbChargeLattice(unsigned int xDimInit, unsigned int yDimInit,
	unsigned int zDimInit, float xScaleInit, float yScaleInit, float zScaleInit)
	:xDim(xDimInit), yDim(yDimInit), zDim(zDimInit), xScale(xScaleInit), yScale(yScaleInit),
	zScale(zScaleInit), latticeMag(xScaleInit) {
	allocLattice();
};

void mbChargeLattice::allocLattice() {
	deallocLattice();
	int cubeCount = xDim*yDim*zDim;
	int pointCount = (xDim + 1)*(yDim + 1)*(zDim + 1);
	int edgeCount = (cubeCount * 3) + ((2 * xDim * yDim) + (2 * xDim * zDim) + (2 * yDim * zDim)) + xDim + yDim + zDim; //Ideal Edge Count
	edgeCount += ((xDim * yDim) + (yDim * zDim) + (zDim * xDim)) * 2; //Haven't combined the edges of the 0 index borders
	chargeLattice = new float[xDim*yDim*zDim];
	latticeCubes = new mbLatticeCube[cubeCount];
	latticeEdges = new mbLatticeEdge[edgeCount];
	latticePoints = new mbLatticePoint[pointCount];
	initializeLattice();
};

void mbChargeLattice::initializeLattice() {
	currentFrame = 0;
	initalizeEdges();
	initializePoints();
	initializeCubes();
	connectLatticeObjects();
};

void mbChargeLattice::initalizeEdges() {
	mbLatticeEdge* edgePointer = latticeEdges;
	int i;
	for (i = 0; i < edgeCount; i++, edgePointer++)
	{
		edgePointer->axis = -1;
		edgePointer->lastFrameVisited = 0;
	}
};

void mbChargeLattice::initializePoints() {
	mbLatticePoint* pointPointer = latticePoints;
	int *latticeLocation;
	float *position;
	int iX, iY, iZ;
	for (iX = 0; iX < xDim; iX++)
		for (iY = 0; iY < yDim; iY++)
			for (iZ = 0; iZ < zDim; iZ++)
			{
				latticeLocation = pointPointer->latticeLocation;
				latticeLocation[0] = iX;
				latticeLocation[1] = iY;
				latticeLocation[2] = iZ;
				position = pointPointer->position;
				position[0] = (float(iX) / xDim) - .5f;
				position[1] = (float(iY) / yDim) - .5f;
				position[2] = (float(iZ) / zDim) - .5f;
				pointPointer->lastFrameVisited = 0;
				pointPointer++;
			}
};

mbLatticePoint* mbChargeLattice::getPoint(int x, int y, int z) {
	if (x < 0 || y < 0 || z < 0 || x > xDim || y > yDim || z > zDim) 
		return NULL;
	return &latticePoints[z + (y * (zDim + 1)) + (x * (zDim + 1) * (yDim + 1))];
};

mbLatticeCube* mbChargeLattice::getCube(int x, int y, int z) {
	if (x < 0 || y < 0 || z < 0 || x >= xDim || y >= yDim || z >= zDim)
		return NULL;
	return &latticeCubes[z + (y * (zDim)) + (x * (zDim)* (yDim))];
};

void mbChargeLattice::initializeCubes() {
	mbLatticeCube* cubePointer = latticeCubes;
	int i, x;
	for (i = 0; i < cubeCount; i++, cubePointer++)
	{
		cubePointer->lastFrameVisited = 0;
		for (x = 0; x < 12; x++)
		{
			cubePointer->edges[x] = NULL;
		}
	}
};

float mbChargeLattice::getChargeForPoint(mbLatticePoint* point) 
{
	float charge;
	int i;
	mbChargeNode *node;
	mbVector3D chargeVec, pointVec;
	if (point->lastFrameVisited != currentFrame)
	{
		point->lastFrameVisited = currentFrame;
		charge = 0.0f;
		for (int i = 0; i < chargeNodes.size(); i++)
		{
			node = &chargeNodes[i];
			chargeVec = mbVector3D(node->x, node->y, node->z);
			pointVec = mbVector3D(point->position[0], point->position[1], point->position[2]);
			float distance = mbSquaredMag(chargeVec - pointVec);
			if (distance < 25.0f)
				charge += (1.0f / distance)* node->charge;

		}
		point->charge = charge;
	}
	return point->charge;
};

mbVector3D mbChargeLattice::interpolateEdge(mbLatticePoint *pointA, mbLatticePoint *pointB, int axisIndex)
{
	float chargeAtA = getChargeForPoint(pointA);
	float *position = pointA->position;
	float multiplier = (isoLevel - chargeAtA) / (getChargeForPoint(pointB) - chargeAtA);
	mbVector3D returnVec = mbVector3D(position[0], position[1], position[2]);
	float interpolationOffset = position[axisIndex] + (multiplier * (pointB->position[axisIndex] - position[axisIndex]));
	if (axisIndex == X_AXIS)
		returnVec.x = interpolationOffset;
	else if (axisIndex == Y_AXIS)
		returnVec.y = interpolationOffset;
	else
		returnVec.z = interpolationOffset;
	return returnVec;
};

mbVector3D mbChargeLattice::calcNormal(mbLatticePoint *point)
{
	int i;
	mbVector3D returnVec = mbVector3D(0.0f, 0.0f, 0.0f);
	mbVector3D intermediateVec;
	float *position = point->position;
	float charge, magnitude;
	mbChargeNode *node;
	for (i = 0; i < chargeNodes.size(); i++)
	{
		node = &chargeNodes[i];
		intermediateVec = mbVector3D(position[0] - node->x, position[1] - node->y, position[2] - node->z);
		float magnitude = mbMagnitude(intermediateVec) / latticeMag;
		charge = .5f * (1.0f / (magnitude * magnitude * magnitude)) * node->charge;
		returnVec += (intermediateVec * charge);
	}
	return returnVec.normalize();
};

void mbChargeLattice::genEdge(mbLatticeCube *cube, int edgeIndex, int pointIndex, int pointIndex2)
{
	mbLatticeEdge *edge = cube->edges[edgeIndex];
	mbVector3D interpolationResult;
	float *position = edge->position;
	if (edge->lastFrameVisited != currentFrame)
	{
		edge->lastFrameVisited = currentFrame;
		interpolationResult = interpolateEdge(cube->points[pointIndex], cube->points[pointIndex2], edge->axis);
		position[0] = interpolationResult.x;
		position[1] = interpolationResult.y;
		position[2] = interpolationResult.z;
		edge->vertexIndex = vertexCount;
		//newNormal[vertP] = calcNormal(v);
		//newVertex[vertP++] = v;

	}

};

void mbChargeLattice::connectLatticeObjects() {
	int cubeIndex = 0;
	int edgeIndex = 0;
	mbLatticeCube *cubePointer, *adjacentCube;
	int* latticePosition;
	mbLatticeEdge** edges;
	mbLatticePoint** points;
	int iX, iY, iZ;
	for (iX = 0; iX < xDim; iX++)
		for (iY = 0; iY < yDim; iY++)
			for (iZ = 0; iZ < zDim; iZ++)
			{
				cubePointer = &latticeCubes[cubeIndex];
				cubeIndex++;
				latticePosition = cubePointer->latticePosition;
				latticePosition[0] = iX;
				latticePosition[1] = iY;
				latticePosition[2] = iZ;
				points = cubePointer->points;
				points[0] = getPoint(iX, iY, iZ);
				points[1] = getPoint(iX + 1, iY, iZ);
				points[2] = getPoint(iX + 1, iY + 1, iZ);
				points[3] = getPoint(iX, iY + 1, iZ);
				points[4] = getPoint(iX, iY, iZ + 1);
				points[5] = getPoint(iX + 1, iY, iZ + 1);
				points[6] = getPoint(iX + 1, iY + 1, iZ + 1);
				points[7] = getPoint(iX, iY + 1, iZ + 1);
				edges = cubePointer->edges;
				edges[5] = &latticeEdges[edgeIndex++];
				edges[5]->axis = Y_AXIS;
				edges[6] = &latticeEdges[edgeIndex++];
				edges[6]->axis = X_AXIS;
				edges[10] = &latticeEdges[edgeIndex++];
				edges[10]->axis = Z_AXIS;
				adjacentCube = getCube(iX + 1, iY, iZ);
				if (adjacentCube)
				{
					adjacentCube->edges[11] = edges[10];
					adjacentCube->edges[7] = edges[5];
				}
				adjacentCube = getCube(iX, iY + 1, iZ);
				if (adjacentCube)
				{
					adjacentCube->edges[4] = edges[6];
					adjacentCube->edges[9] = edges[10];
				}
				adjacentCube = getCube(iX, iY + 1, iZ + 1);
				if (adjacentCube)
				{
					adjacentCube->edges[0] = edges[6];
				}
				adjacentCube = getCube(iX + 1, iY, iZ + 1);
				if (adjacentCube)
				{
					adjacentCube->edges[3] = edges[5];
				}
				adjacentCube = getCube(iX + 1, iY + 1, iZ);
				if (adjacentCube)
				{
					adjacentCube->edges[8] = edges[10];
				}
				adjacentCube = getCube(iX, iY, iZ + 1);
				if (adjacentCube)
				{
					adjacentCube->edges[1] = edges[5];
					adjacentCube->edges[2] = edges[6];
				}
				if (!edges[0]) {
					edges[0] = &latticeEdges[edgeIndex++];
					edges[0]->axis = X_AXIS;
				}
				if (!edges[1]) {
					edges[1] = &latticeEdges[edgeIndex++];
					edges[1]->axis = Y_AXIS;
				}
				if (!edges[2]) {
					edges[2] = &latticeEdges[edgeIndex++];
					edges[2]->axis = X_AXIS;
				}
				if (!edges[3]) {
					edges[3] = &latticeEdges[edgeIndex++];
					edges[3]->axis = Y_AXIS;
				}
				if (!edges[4]) {
					edges[4] = &latticeEdges[edgeIndex++];
					edges[4]->axis = X_AXIS;
				}
				if (!edges[7]) {
					edges[7] = &latticeEdges[edgeIndex++];
					edges[7]->axis = Y_AXIS;
				}
				if (!edges[8]) {
					edges[8] = &latticeEdges[edgeIndex++];
					edges[8]->axis = Z_AXIS;
				}
				if (!edges[9]) {
					edges[9] = &latticeEdges[edgeIndex++];
					edges[9]->axis = Z_AXIS;
				}
				if (!edges[11]) {
					edges[11] = &latticeEdges[edgeIndex++];
					edges[11]->axis = Z_AXIS;
				}
			}
};

void mbChargeLattice::deallocLattice() {
	if (chargeLattice)
		delete[] chargeLattice; chargeLattice = NULL;
	if (latticeCubes)
		delete[] latticeCubes; latticeCubes = NULL;
	if (latticeEdges)
		delete[] latticeEdges; latticeEdges = NULL;
	if (latticePoints)
		delete[] latticePoints; latticePoints = NULL;
		
};

mbChargeLattice::~mbChargeLattice() {
	deallocLattice();
	deallocChargeReturn();
};

unsigned int mbChargeLattice::getNodeCount() {
	return (unsigned int) chargeNodes.size();
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
