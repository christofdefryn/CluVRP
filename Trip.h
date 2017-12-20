#pragma once
#include <vector>
#include "CluVRPinst.h"

class Trip
{
protected:

	CluVRPinst* cluVRPinst_;

	double dist_;
	int totalDemand_;

	int size_;
	
public:
	Trip(CluVRPinst*);
	~Trip();

	//getters
	inline double getDistance(void) const
	{
		return this->dist_;
	}
	inline int getSpareCapacity(void) const
	{
		return cluVRPinst_->getVehicleCapacity() - this->totalDemand_;
	}
	inline int getSize(void) const
	{
		return size_;
	}
	inline int getTotalDemand(void) const
	{
		return totalDemand_;
	}

	//setters
	inline void setTotalDemand(int demand)
	{
		this->totalDemand_ = demand;
	}
	inline void altTotalDemand(int diff)
	{
		totalDemand_ += diff;
	}
	inline void altDist(double diff)
	{
		dist_ += diff;
	}
	inline void altSize(int diff)
	{
		size_ += diff;
	}

	virtual double calcDistance(void) = 0;
};

class CluTrip : public Trip
{
	friend class Move;

private:

	std::vector<Cluster*> vClusters_;

public:
	CluTrip(CluVRPinst*);
	~CluTrip();

	inline Cluster* getLastPosition(void) const
	{
		return this->vClusters_.back();
	}
	inline Cluster* getCluster(int it) const
	{
		return vClusters_.at(it);
	}
	inline std::vector<Cluster*> getvClusters(void) const
	{
		return vClusters_;
	}

	inline void setvClusters(std::vector<Cluster*> v)
	{
		vClusters_ = v;
	}

	double calcDistance(void);
	void addStop(Cluster*);
	void removeStop(int);
	void returnToDepot(void);
};

class NodTrip : public Trip
{
	friend class Move;
	friend class NodVNS;

private:

	std::vector<Node*> vNodes_;

public:
	NodTrip(CluVRPinst*);
	NodTrip(NodTrip*);
	~NodTrip();

	//getters
	inline Node* getLastPosition(void) const
	{
		return this->vNodes_.back();
	}
	inline Node* getNode(int it) const
	{
		return vNodes_.at(it);
	}
	inline std::vector<Node*> getvNodes(void) const
	{
		return vNodes_;
	}

	double calcDistance(void);
	void addStop(Node*);
};