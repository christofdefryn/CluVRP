#pragma once
#include "Solution.h"

struct CluVRPsol
{
	ClusterSolution* sCluster_;
	NodeSolution* sNode_;

	CluVRPsol(ClusterSolution*, NodeSolution*);
	~CluVRPsol();
};

