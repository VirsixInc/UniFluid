#pragma once
#include <vector>

struct mbChargeNode {
	float x, y, z, charge;
};

struct mbLatticePoint {
	int latticeLocation[3];
	unsigned int lastFrameVisited;
	float charge;
	float position[3];
};

struct mbLatticeEdge {
	int vertexIndex;
	float position[3];
	unsigned int lastFrameVisited;
	int axis;
};

struct mbLatticeCube {
	mbLatticePoint* points[8];
	mbLatticeEdge* edges[12];
	int latticePosition[3];
	unsigned int lastFrameVisited;
};


class mbChargeLattice {
public:
	unsigned int xDim, yDim, zDim;
	bool chargeAllocated = false;
	float xScale, yScale, zScale;
	float latticeMag;
	unsigned int currentFrame;
	float *chargeLattice = NULL;
	mbLatticeEdge *latticeEdges = NULL;
	mbLatticeCube *latticeCubes = NULL;
	mbLatticePoint *latticePoints = NULL;
	float* meshVertices = NULL;
	float* meshNormals = NULL;
	int* meshTriangles = NULL;
	int pointCount, edgeCount, cubeCount, vertexCount;
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
	mbLatticePoint* mbChargeLattice::getPoint(int x, int y, int z);
	mbLatticeCube* mbChargeLattice::getCube(int x, int y, int z);
	void mbChargeLattice::connectLatticeObjects();
	void mbChargeLattice::initializePoints();
	void mbChargeLattice::initalizeEdges();
	void mbChargeLattice::initializeLattice();
	void mbChargeLattice::initializeCubes();

};