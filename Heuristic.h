#pragma once
#include "CluVRPinst.h"
#include "Solution.h"

#include <algorithm>

class BinPacking;
class Diversification;
class Redistribution;

class Heuristic
{
protected:
	CluVRPinst* cluVRPinst_;

public:
	Heuristic(CluVRPinst*);
	~Heuristic();

	auto run(void) {};
};

class BinPacking : public Heuristic
{
private:

	Redistribution* redistributionOperator_;
	Diversification* diversOperator_;

public:
	BinPacking(CluVRPinst*);
	~BinPacking();

	bool run(ClusterSolution*&);
};

class Diversification : public Heuristic
{
private:
	Redistribution* redistribution_;

	void repairCluster(ClusterSolution*&, std::vector<Cluster*>);
	std::vector<Cluster*> perturbationCluster(ClusterSolution*&);

public:
	Diversification(CluVRPinst*);
	~Diversification();

	bool run(ClusterSolution*&);		// TRUE -> go back to conversion;  FALSE -> go back to cluVNS
};

class Redistribution : public Heuristic
{
private:
	bool swapClusters(ClusterSolution*&);

	Pos p1_;
	Pos p2_;

public:
	Redistribution(CluVRPinst*);
	~Redistribution();

	bool run(ClusterSolution*&);
};