#pragma once

#include "Solution.h"
#include "Move.h"

class VNS
{
private:
	virtual bool findNeighbour(void) = 0;

protected:
	CluVRPinst* cluVRPinst_;

	int currentNBH_;
	int nNBHs_;

public:
	VNS(CluVRPinst*);
	~VNS();
};

class CluVNS : public VNS
{
private:

	ClusterSolution* current_;

	std::vector<int> nbhSequence_;

	/*INTRA VEHICLE NEIGHBOURHOODS*/
	bool intraVehicleSwap(void);
	bool intraVehicleRelocate(void);
	bool twoOpt(void);
	bool intraVehicleOrOpt(void);
	
	/*INTER VEHICLE NEIGHBOURHOODS*/
	bool interVehicleSwap(void);
	bool interVehicleRelocate(void);
	bool interVehicleOrOpt(void);

	bool findNeighbour(void);

public:

	CluVNS(CluVRPinst*);
	~CluVNS();

	void run(ClusterSolution*&);
};

class NodVNS : public VNS
{
private:

	NodeSolution* current_;
	bool additionalNBHs_;
	bool innerLoop_;

	std::vector<unsigned short int> nbhSequence_;

	/*INTRA VEHICLE INTRA CLUSTER NEIGHBOURHOODS*/
	bool intraVehicleSwap(void);
	bool intraVehicleRelocate(void);
	bool intraVehicleTwoOpt(void);
	bool intraVehicleOrOpt(void);

	/*INTER CLUSTER NEIGHBOURHOODS*/
	bool clusterSwap(void);
	bool clusterRelocate(void);	

	bool findNeighbour(void);

public:

	NodVNS(CluVRPinst*,bool);
	~NodVNS();
	
	void enableAdditionalNBHs(void);
	void disableAdditionalNBHs(void);
	void selectNBHs(void);

	void run(NodeSolution*&);
};