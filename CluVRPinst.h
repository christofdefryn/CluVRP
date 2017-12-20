#pragma once
#include <vector>

#include "Cluster.h"

class CluVRPinst
{
private:
	int nVehicles_;
	int vehicleCapacity_;

	std::vector<std::vector<double>> distNodes_;
	std::vector<std::vector<double>> distClusters_;

	std::vector<Cluster> vClusters_;
	int nClusters_;

public:
	CluVRPinst(std::vector<Cluster>, int);
	~CluVRPinst();

	//getters
	inline double getDistNodes(Node* a, Node* b) const
	{
		return this->distNodes_.at(a->id - 1).at(b->id - 1);
	}
	inline double getDistClusters(Cluster* a, Cluster* b) const
	{
		return this->distClusters_.at(a->getId()).at(b->getId());
	}
	inline int getnVehicles(void) const
	{
		return nVehicles_;
	}
	inline int getnClusters(void) const
	{
		return nClusters_;
	}
	inline int getVehicleCapacity(void) const
	{
		return vehicleCapacity_;
	}
	std::vector<Cluster*> getClientClusters(void);
	Cluster* getRandomDepot(void);
	inline Cluster* getCluster(int id)
	{
		return &this->vClusters_.at(id);
	}
	inline std::vector<std::vector<double>> getDistNodesMatrix(void) const
	{
		return distNodes_;
	}

	//setters
	inline void setDistNodes(std::vector<std::vector<double>> m)
	{
		distNodes_ = m;
	}
	inline void setDistClusters(std::vector<std::vector<double>> m)
	{
		distClusters_ = m;
	}

	inline void increasenVeh(void)
	{
		nVehicles_++;
	}
	 bool isFeasible(void);
	 //void calculateIntraClusterTSP(void);
 };
