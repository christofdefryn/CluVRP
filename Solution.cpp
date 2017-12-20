#include "Solution.h"
#include "Heuristic.h"
#include <iostream>

using namespace std;

Solution::Solution(){}

Solution::Solution(CluVRPinst* cluVRPinst)
{
	this->cluVRPinst_ = cluVRPinst;
	this->totalDist_ = 0;
}

Solution::~Solution(){}

double Solution::getTotalDist(void) const
{
	return totalDist_;
}

void Solution::setTotalDist(double value)
{
	totalDist_ = value;
}

/********************************************************************************/
/********************************************************************************/

ClusterSolution::ClusterSolution(CluVRPinst* cluVRPinst) : Solution(cluVRPinst){}

ClusterSolution::~ClusterSolution()
{
	for (unsigned int i = 0; i < vTrips_.size(); i++) delete vTrips_.at(i);
	vTrips_.clear();
}

void ClusterSolution::addTrip(CluTrip* t)
{
	vTrips_.push_back(t);
	totalDist_ += t->getDistance();
}

int ClusterSolution::getRandomFeasibleVehicle(Cluster*& c, bool& success)
{
	std::vector<unsigned int short> feasVeh;

	for (unsigned int i = 0; i < vTrips_.size(); i++)
	{
		if (vTrips_.at(i)->getSpareCapacity() >= c->getDemand())
		{
			feasVeh.push_back(i);
		}
	}

	if (feasVeh.size() != 0)
	{	
		success = true;
		return feasVeh.at(rand() % feasVeh.size());
	}
	else
	{
		success = false;
		return -1;
	}
}

int ClusterSolution::getClosestFeasibleVehicle(Cluster*& c, bool& success)
{
	double minDist = BIG_M;
	int veh = -1;

	success = false;

	for (unsigned int i = 0; i < vTrips_.size(); i++)
	{
		if (vTrips_.at(i)->getSpareCapacity() >= c->getDemand())
		{
			double dist = cluVRPinst_->getDistClusters(vTrips_.at(i)->getLastPosition(), c);
			if (dist < minDist)
			{
				minDist = dist;
				veh = i;
				success = true;
			}
		}
	}

	return veh;
}

Pos ClusterSolution::findPosition(Cluster* c)
{
	for (int v = 0; v < cluVRPinst_->getnVehicles(); v++)
	{
		for (int i = 1; i < this->vTrips_.at(v)->getSize() - 1; i++)
		{
			if (this->vTrips_.at(v)->getCluster(i)->getId() == c->getId())
			{
				Pos p(v, i);
				return p;
			}
		}
	}

	return Pos(-1, -1);
}

double ClusterSolution::calcTotalDist(void)
{
	totalDist_ = 0;

	for (int i = 0; i < cluVRPinst_->getnVehicles(); i++)
	{
		totalDist_ += vTrips_.at(i)->calcDistance();
	}

	return totalDist_;
}

NodeSolution* ClusterSolution::convert(void)
{
	CluTrip* cluT = nullptr;
	NodTrip* nodT = nullptr;
	NodeSolution* nodSol = new NodeSolution(cluVRPinst_);

	for (int v = 0; v < cluVRPinst_->getnVehicles(); v++)
	{
		//copy current Clustered Trip
		cluT = vTrips_.at(v);

		//Create new Node Trip
		nodT = new NodTrip(cluVRPinst_);
		nodT->setTotalDemand(vTrips_.at(v)->getTotalDemand());

		//for every cluster in the Clustered Trip, take all nodes and push them in the Node Trip
		for (int i = 0; i < cluT->getSize(); i++)
		{
			Cluster* c = cluT->getCluster(i);

			//there is only one node in the current cluster -> just add the node
			if (c->getnNodes() == 1)
			{
				nodT->addStop(c->getvNodesPtr().front());
				continue;
			}
			else
			{
				std::vector<Node*> vNodes = c->getvNodesPtr();

				if ((double)rand() / RAND_MAX < Params::RANDOM_CONVERSION)
				{
					/* STRATEGY 1 => add nodes in random order for this cluster */
					random_shuffle(vNodes.begin(), vNodes.end());

					for (int j = 0; j < c->getnNodes(); j++)
					{
						nodT->addStop(vNodes.at(j));
					}
				}
				else
				{
					/* STRATEGY 2 => add nodes according to nearest neighbour approach */

					//define and add first node in the cluster (closest to current position)
					double minDist = BIG_M;
					int first;
					for (unsigned int j = 0; j < vNodes.size(); j++)
					{
						if (cluVRPinst_->getDistNodes(nodT->getLastPosition(), vNodes.at(j)) < minDist)
						{
							first = j;
							minDist = cluVRPinst_->getDistNodes(nodT->getLastPosition(), vNodes.at(j));
						}
					}

					nodT->addStop(vNodes.at(first));
					vNodes.erase(vNodes.begin() + first);

					//get next cluster to define the appropriate last node in the current cluster (closest to a node in the next cluster)
					std::vector<Node*> vNodesNext = cluT->getCluster(i + 1)->getvNodesPtr();
					minDist = BIG_M;
					int last;
					for (unsigned int j = 0; j < vNodes.size(); j++)
					{
						for (unsigned int k = 0; k < vNodesNext.size(); k++)
						{
							if (cluVRPinst_->getDistNodes(vNodes.at(j), vNodesNext.at(k)) < minDist)
							{
								last = j;
								minDist = cluVRPinst_->getDistNodes(vNodes.at(j), vNodesNext.at(k));
							}
						}
					}

					Node* lastNode = vNodes.at(last);
					vNodes.erase(vNodes.begin() + last);

					//add optimal sequence to the trip
					while (vNodes.size() > 0)
					{
						minDist = BIG_M;
						int next;
						for (unsigned int j = 0; j < vNodes.size(); j++)
						{
							if (cluVRPinst_->getDistNodes(nodT->getLastPosition(), vNodes.at(j)) < minDist)
							{
								next = j;
								minDist = cluVRPinst_->getDistNodes(nodT->getLastPosition(), vNodes.at(j));
							}
						}

						nodT->addStop(vNodes.at(next));
						vNodes.erase(vNodes.begin() + next);
					}

					nodT->addStop(lastNode);
				}
			}
		}

		nodT->calcDistance();
		nodSol->addTrip(nodT);
	}

	return nodSol;
}

/********************************************************************************/
/********************************************************************************/

NodeSolution::NodeSolution(){}

NodeSolution::NodeSolution(CluVRPinst* cluVRPinst):Solution(cluVRPinst)
{
	this->totalDist_ = 0;
}

NodeSolution::NodeSolution(CluVRPinst* cluVRPinst, int d) : Solution(cluVRPinst)
{
	this->totalDist_ = d;
}

NodeSolution::NodeSolution(NodeSolution* copy):Solution(copy->cluVRPinst_)
{
	for (int i = 0; i < copy->getnTrips(); i++)
	{
		NodTrip* t = new NodTrip(copy->getTrip(i));
		this->addTrip(t);
	}

	this->totalDist_ = copy->totalDist_;
}

NodeSolution::~NodeSolution()
{
	for (unsigned int i = 0; i < vTrips_.size(); i++) delete vTrips_.at(i);
	vTrips_.clear();
}

void NodeSolution::addTrip(NodTrip* t)
{	
	this->vTrips_.push_back(t);
	this->totalDist_ += t->getDistance();
}

double NodeSolution::calcTotalDist(void)
{
	totalDist_ = 0;

	for (int i = 0; i < this->getnTrips(); i++)
	{
		totalDist_ += vTrips_.at(i)->getDistance();
	}

	return totalDist_;
}

bool NodeSolution::evaluate(NodeSolution*& best)
{
	if (this->totalDist_ < best->totalDist_)
	{
		delete best;		
		best = this;
		return true;
	}
	else
	{
		return false;
	}
}

ClusterSolution* NodeSolution::convert(void)
{
	ClusterSolution* s = new ClusterSolution(cluVRPinst_);

	for (unsigned int i = 0; i < vTrips_.size(); i++)
	{
		NodTrip* nT = vTrips_.at(i);
		CluTrip* cT = new CluTrip(cluVRPinst_);
		
		int c = nT->getvNodes().front()->cluster;		//c is first cluster
		cT->addStop(cluVRPinst_->getCluster(c));
		
		//loop through all nodes, if cluster changes -> add new cluster
		for (int j = 1; j < nT->getSize(); j++)
		{
			if (nT->getNode(j)->cluster == c) continue;

			c = nT->getNode(j)->cluster;
			cT->addStop(cluVRPinst_->getCluster(c));
		}

		s->addTrip(cT);
	}

	return s;
}