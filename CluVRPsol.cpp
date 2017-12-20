#include "CluVRPsol.h"


CluVRPsol::CluVRPsol(ClusterSolution* sCluster, NodeSolution* sNode)
{
	sCluster_ = sCluster;
	sNode_ = sNode;
}

CluVRPsol::~CluVRPsol()
{
	delete sNode_;
	delete sCluster_;
}