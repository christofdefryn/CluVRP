#include "Heuristic.h"
#include "Move.h"

Heuristic::Heuristic(CluVRPinst* cluVRPinst)
{
	this->cluVRPinst_ = cluVRPinst;
}

Heuristic::~Heuristic(){}

/******************************** BIN PACKING ****************************************/


BinPacking::BinPacking(CluVRPinst* cluVRPinst) : Heuristic(cluVRPinst)
{
	redistributionOperator_ = new Redistribution(cluVRPinst);
	diversOperator_ = new Diversification(cluVRPinst);
}

BinPacking::~BinPacking()
{
	delete redistributionOperator_;
	delete diversOperator_;
}

bool BinPacking::run(ClusterSolution*& s)
{
	std::vector<Cluster*> vClientClusters = cluVRPinst_->getClientClusters();

	//sort clusters from by demand (large to small)
	sort(vClientClusters.begin(), vClientClusters.end(), Cluster::sortByDemandOperator);

	//create trip for every vehicle
	for (int i = 0; i < this->cluVRPinst_->getnVehicles(); i++)
	{
		CluTrip* t = new CluTrip(cluVRPinst_);
		
		//select random depot	
		t->addStop(cluVRPinst_->getRandomDepot());
		//add trip to the solution
		s->addTrip(t);
	}

	//add the clusters one by one
	int nRedistributionIterations = 0;

	while (vClientClusters.size() > 0)
	{
		//get cluster to add
		Cluster* c = vClientClusters.front();

		//get strategy
		double r = (double)rand() / RAND_MAX;
		bool success = false;
		int veh;

		//select vehicle
		if (r < Params::RANDOM_CONSTRUCTION) veh = s->getRandomFeasibleVehicle(c,success);
		else veh = s->getClosestFeasibleVehicle(c,success);

		//perform move
		if (success)
		{
			s->getTrip(veh)->addStop(c);
			vClientClusters.erase(vClientClusters.begin());
		}
		else
		{			
			redistributionOperator_->run(s);
			nRedistributionIterations++;
			
			if (nRedistributionIterations > 5)
			{
				if (nRedistributionIterations > 100) return false;
				diversOperator_->run(s);
			}
		}
	}

	//return to the initial depot
	for (int i = 0; i < this->cluVRPinst_->getnVehicles(); i++)
	{
		s->getTrip(i)->returnToDepot();
	}

	//calc objective value of this solution
	s->calcTotalDist();

	return true;
}

/******************************* DIVERSIFICATION **************************************/

void Diversification::repairCluster(ClusterSolution*& s, std::vector<Cluster*> toAdd)
{
	//add clusters in random order
	random_shuffle(toAdd.begin(), toAdd.end());
	int nTry = 0;
	
	while (toAdd.size() > 0)
	{
		std::vector<int> feasVeh;

		//look for all feasible vehicles (capacity)
		do {

			for (int v = 0; v < s->getnTrips(); v++)
			{
				if (toAdd.back()->getDemand() <= s->getTrip(v)->getSpareCapacity())
				{
					feasVeh.push_back(v);
				}
			}

			if (feasVeh.size() == 0)	//no feasible vehicles found
			{
				redistribution_->run(s);
				nTry++;
				if (nTry > 100)
				{
					//add an additional trip
					CluTrip* t = new CluTrip(cluVRPinst_);
					//add random depot
					t->addStop(cluVRPinst_->getRandomDepot());
					s->addTrip(t);
					cluVRPinst_->increasenVeh();

					continue;
				}
				else if (nTry > 5)
				{
					std::vector<Cluster*> removed;
					removed = perturbationCluster(s);
					toAdd.insert(toAdd.end(), removed.begin(), removed.end());
					continue;
				}
			}
			else break;

		} while (true);

		//choose one feasible vehicle at random
		int selection = rand() % feasVeh.size();
		s->getTrip(feasVeh.at(selection))->addStop(toAdd.back());
		toAdd.pop_back();
	}
	

	//return to the depot for every trip
	for (int v = 0; v < cluVRPinst_->getnVehicles(); v++)
	{
		s->getTrip(v)->addStop(s->getTrip(v)->getvClusters().front());
	}

	s->calcTotalDist();
}

std::vector<Cluster*> Diversification::perturbationCluster(ClusterSolution*& s)
{
	std::vector<Cluster*> removedClusters;

	for (int v = 0; v < cluVRPinst_->getnVehicles(); v++)
	{
		if (s->getTrip(v)->getvClusters().front() == s->getTrip(v)->getvClusters().back())
		{
			s->getTrip(v)->removeStop(s->getTrip(v)->getSize() - 1);		//remove depot at the end of the trip
		}

		for (int i = 1; i < s->getTrip(v)->getSize(); i++)
		{
			if (s->getTrip(v)->getSize() < 3)
			{
				break;
			}
			double r = (double)rand() / (RAND_MAX);
			if (r < Params::PERT_RATE)
			{
				removedClusters.push_back(s->getTrip(v)->getCluster(i));
				s->getTrip(v)->removeStop(i);
			}
		}
	}

	return removedClusters;
}

Diversification::Diversification(CluVRPinst* cluVRPinst) : Heuristic (cluVRPinst)
{
	redistribution_ = new Redistribution(cluVRPinst_);
}

Diversification::~Diversification() 
{
	delete redistribution_;
}

bool Diversification::run(ClusterSolution*& s)
{
	std::vector<Cluster*> toAdd;

	toAdd = perturbationCluster(s);
	repairCluster(s, toAdd);

	double r = (double)rand() / RAND_MAX;
	if (r < Params::DIVERSIFICATION_1)
		return false;		//go to cluVNS
	else
		return true;		//go to conversion
}

/******************************* REDISTRIBUTION **************************************/

bool Redistribution::swapClusters(ClusterSolution*& cluSol)
{
	CluTrip* cluT1 = nullptr;
	CluTrip* cluT2 = nullptr;

	int minDif = BIG_M;
	bool success = false;
	bool flag = false;

	for (int v1 = 0; v1 < cluVRPinst_->getnVehicles(); v1++)
	{
		cluT1 = cluSol->getTrip(v1);

		int spareCapacity = cluT1->getSpareCapacity();
		if (spareCapacity == 0) continue;

		for (int v2 = 0; v2 < cluVRPinst_->getnVehicles(); v2++)
		{
			if (v1 == v2) continue;

			cluT2 = cluSol->getTrip(v2);

			for (int i = 1; i < cluT1->getSize() - 1; i++)
			{
				for (int j = 1; j < cluT2->getSize() - 1; j++)
				{
					if (cluT1->getCluster(i)->getDemand() == cluT2->getCluster(j)->getDemand()) continue;

					int diff = \
						+ spareCapacity\
						+ cluT1->getCluster(i)->getDemand()\
						- cluT2->getCluster(j)->getDemand();

					if (diff >= 0 && diff < minDif)
					{
						Pos p1(v1, i);
						Pos p2(v2, j);
						if ((p1 == p1_ && p2 == p2_) || (p1 == p2_ && p2 == p1_)) continue;
						
						success = true;
						minDif = diff;
						p1_ = p1;
						p2_ = p2;
						if (minDif == 0)
						{
							flag = true;
							break;
						}
					}
				}
				if (flag) break;
			}
			if (flag) break;
		}
		if (flag) break;
	}
	if (success)
	{
		int c = \
			+ cluSol->getTrip(p1_.veh_)->getCluster(p1_.ind_)->getDemand()\
			- cluSol->getTrip(p2_.veh_)->getCluster(p2_.ind_)->getDemand();

		cluSol->getTrip(p1_.veh_)->altTotalDemand(-c);
		cluSol->getTrip(p2_.veh_)->altTotalDemand(c);

		cluSol->getTrip(p1_.veh_)->altDist(\
			- cluVRPinst_->getDistClusters(cluSol->getTrip(p1_.veh_)->getCluster(p1_.ind_ - 1), cluSol->getTrip(p1_.veh_)->getCluster(p1_.ind_))\
			- cluVRPinst_->getDistClusters(cluSol->getTrip(p1_.veh_)->getCluster(p1_.ind_), cluSol->getTrip(p1_.veh_)->getCluster(p1_.ind_ + 1))\
			+ cluVRPinst_->getDistClusters(cluSol->getTrip(p1_.veh_)->getCluster(p1_.ind_ - 1), cluSol->getTrip(p2_.veh_)->getCluster(p2_.ind_))\
			+ cluVRPinst_->getDistClusters(cluSol->getTrip(p2_.veh_)->getCluster(p2_.ind_), cluSol->getTrip(p1_.veh_)->getCluster(p1_.ind_ + 1)));

		cluSol->getTrip(p2_.veh_)->altDist(\
			- cluVRPinst_->getDistClusters(cluSol->getTrip(p2_.veh_)->getCluster(p2_.ind_ - 1), cluSol->getTrip(p2_.veh_)->getCluster(p2_.ind_))\
			- cluVRPinst_->getDistClusters(cluSol->getTrip(p2_.veh_)->getCluster(p2_.ind_), cluSol->getTrip(p2_.veh_)->getCluster(p2_.ind_ + 1))\
			+ cluVRPinst_->getDistClusters(cluSol->getTrip(p2_.veh_)->getCluster(p2_.ind_ - 1), cluSol->getTrip(p1_.veh_)->getCluster(p1_.ind_))\
			+ cluVRPinst_->getDistClusters(cluSol->getTrip(p1_.veh_)->getCluster(p1_.ind_), cluSol->getTrip(p2_.veh_)->getCluster(p2_.ind_ + 1)));

		Move::swap(cluSol, p1_, p2_);
		return true;
	}
	else
	{
		return false;
	}
}

Redistribution::Redistribution(CluVRPinst* cluVRPinst) : Heuristic(cluVRPinst)
{
	p1_ = Pos(-1, -1);
	p2_ = Pos(-1, -1);
}

Redistribution::~Redistribution() {}

bool Redistribution::run(ClusterSolution*& cluSol)
{
	return swapClusters(cluSol);
}