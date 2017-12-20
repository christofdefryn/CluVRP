#pragma once
#include <vector>
#include "Trip.h"
#include "CluVRPinst.h"

class Solution;
class ClusterSolution;
class NodeSolution;

class Solution
{
protected:

	CluVRPinst* cluVRPinst_;
	double totalDist_;

public:
	Solution();
	Solution(CluVRPinst*);
	~Solution();

	virtual double calcTotalDist(void) = 0;
	double getTotalDist(void) const;

	void setTotalDist(double);
};

class ClusterSolution : public Solution
{
	friend class Move;

private:
	std::vector<CluTrip*> vTrips_;

public:
	ClusterSolution(CluVRPinst*);
	~ClusterSolution();

	//getters
	inline CluTrip* getTrip(int i) const
	{
		return vTrips_.at(i);
	}
	inline int getnTrips(void) const
	{
		return (int)vTrips_.size();
	}

	void addTrip(CluTrip*);
	
	int getRandomFeasibleVehicle(Cluster*&, bool&);
	int getClosestFeasibleVehicle(Cluster*&, bool&);
	//bool redistribution(void);
	
	Pos findPosition(Cluster*);						//find position of the cluster with given ID
	double calcTotalDist(void);

	NodeSolution* convert(void);
};

class NodeSolution : public Solution
{
	friend class Move;

private:

	std::vector<NodTrip*> vTrips_;

public:
	NodeSolution();
	NodeSolution(CluVRPinst*);
	NodeSolution(CluVRPinst*, int);
	NodeSolution(NodeSolution*);
	~NodeSolution();

	//getters
	inline NodTrip* getTrip(int i) const
	{
		return vTrips_.at(i);
	}
	inline int getnTrips(void) const
	{
		return (int)vTrips_.size();
	}

	void addTrip(NodTrip*);
	double calcTotalDist(void);
	bool evaluate(NodeSolution*&);		//returns true if there was an improvement
	ClusterSolution* convert(void);
};