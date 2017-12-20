#pragma once
#include <vector>
#include <algorithm>

#include "Globals.h"

class Cluster
{
private:

	int id_;
	int demand_;

	int nNodes_;
	std::vector<Node> vNodes_;

public:

	bool isDepot;

	Cluster(int);
	~Cluster();

	//getters
	inline int getDemand(void) const
	{
		return demand_;
	}
	inline int getId(void) const
	{
		return id_;
	}
	inline int getnNodes(void) const
	{
		return nNodes_;
	}
	inline Node* getNode(int& i)
	{
		return &vNodes_.at(i);
	}
	inline std::vector<Node*> getvNodesPtr(void)
	{
		std::vector<Node*> v;
		for (int i = 0; i < nNodes_; i++) v.push_back(&vNodes_.at(i));
		return v;
	}
	inline std::vector<Node> getvNodes(void)
	{
		return vNodes_;
	}

	//setters
	inline void setDemand(int& d)
	{
		this->demand_ = d;
	}
	inline void addDemand(int& d)
	{
		this->demand_ += d;
	}
	inline void setvNodes(std::vector<Node>& vNodes)
	{
		vNodes_ = vNodes;
	}

	//operator
	static bool sortByDemandOperator(Cluster*&, Cluster*&); //large to small

	void addNode(Node); //add individual client to this cluster
};

