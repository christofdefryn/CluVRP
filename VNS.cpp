#include "VNS.h"
#include <iostream>

#include "Printer.h"

VNS::VNS(CluVRPinst* cluVRPinst)
{
	cluVRPinst_ = cluVRPinst;
	currentNBH_ = 0;
}

VNS::~VNS(){}

/////////////////////////////////////////////////////////

/* INTRA VEHICLE NEIGHBOURHOODS */

bool CluVNS::intraVehicleSwap(void)
{
	bool flag = false; 

	for (int v = 0; v < cluVRPinst_->getnVehicles(); v++)		//for every vehicle
	{		
		CluTrip* cluTrip = current_->getTrip(v);

		for (int i = 1; i < cluTrip->getSize() - 2; i++)
		{
			for (int j = i + 1; j < cluTrip->getSize() - 1; j++)
			{
				double diff;

				if (j == i + 1)
				{
					diff = \
						- cluVRPinst_->getDistClusters(cluTrip->getCluster(i - 1), cluTrip->getCluster(i))\
						- cluVRPinst_->getDistClusters(cluTrip->getCluster(j), cluTrip->getCluster(j + 1))\
						+ cluVRPinst_->getDistClusters(cluTrip->getCluster(i - 1), cluTrip->getCluster(j))\
						+ cluVRPinst_->getDistClusters(cluTrip->getCluster(i), cluTrip->getCluster(j + 1));
				}
				else
				{
					diff = \
						- cluVRPinst_->getDistClusters(cluTrip->getCluster(i - 1), cluTrip->getCluster(i))\
						- cluVRPinst_->getDistClusters(cluTrip->getCluster(i), cluTrip->getCluster(i + 1))\
						- cluVRPinst_->getDistClusters(cluTrip->getCluster(j - 1), cluTrip->getCluster(j))\
						- cluVRPinst_->getDistClusters(cluTrip->getCluster(j), cluTrip->getCluster(j + 1))\
						+ cluVRPinst_->getDistClusters(cluTrip->getCluster(i - 1), cluTrip->getCluster(j))\
						+ cluVRPinst_->getDistClusters(cluTrip->getCluster(j), cluTrip->getCluster(i + 1))\
						+ cluVRPinst_->getDistClusters(cluTrip->getCluster(j - 1), cluTrip->getCluster(i))\
						+ cluVRPinst_->getDistClusters(cluTrip->getCluster(i), cluTrip->getCluster(j + 1));
				}

				if (diff < -.01)
				{
					Pos a(v, i), b(v, j);
					Move::swap(current_, a, b);
					cluTrip->altDist(diff);
					flag = true;
				}
			}
		}
	}

	return flag;
}

bool CluVNS::intraVehicleRelocate(void)		//put i right after cluster j
{
	bool flag = false;

	for (int v = 0; v < cluVRPinst_->getnVehicles(); v++)
	{
		CluTrip* cluTrip = current_->getTrip(v);

		for (int i = 1; i < cluTrip->getSize() - 1; i++)
		{
			if (i > 2)
			{
				for (int j = 0; j < i - 1; j++)			// if j = i - 1 nothing changes
				{
					double diff = \
						- cluVRPinst_->getDistClusters(cluTrip->getCluster(i - 1), cluTrip->getCluster(i))\
						- cluVRPinst_->getDistClusters(cluTrip->getCluster(i), cluTrip->getCluster(i + 1))\
						+ cluVRPinst_->getDistClusters(cluTrip->getCluster(i - 1), cluTrip->getCluster(i + 1))\
						- cluVRPinst_->getDistClusters(cluTrip->getCluster(j), cluTrip->getCluster(j + 1))\
						+ cluVRPinst_->getDistClusters(cluTrip->getCluster(j), cluTrip->getCluster(i))\
						+ cluVRPinst_->getDistClusters(cluTrip->getCluster(i), cluTrip->getCluster(j + 1));

					if (diff < -.01)
					{
						Pos a(v, i), b(v, j);
						Move::relocate(current_, a, b);
						cluTrip->altDist(diff);
						flag = true;
					}
				}
			}
			if (i < cluTrip->getSize() - 2)
			{
				for (int j = i + 1; j < cluTrip->getSize() - 1; j++)	
				{
					double diff = \
						- cluVRPinst_->getDistClusters(cluTrip->getCluster(i - 1), cluTrip->getCluster(i))\
						- cluVRPinst_->getDistClusters(cluTrip->getCluster(i), cluTrip->getCluster(i + 1))\
						+ cluVRPinst_->getDistClusters(cluTrip->getCluster(i - 1), cluTrip->getCluster(i + 1))\
						- cluVRPinst_->getDistClusters(cluTrip->getCluster(j), cluTrip->getCluster(j + 1))\
						+ cluVRPinst_->getDistClusters(cluTrip->getCluster(j), cluTrip->getCluster(i))\
						+ cluVRPinst_->getDistClusters(cluTrip->getCluster(i), cluTrip->getCluster(j + 1));

					if (diff < -.01)
					{
						Pos a(v, i), b(v, j);
						Move::relocate(current_, a, b);
						cluTrip->altDist(diff);
						flag = true;
					}
				}
			}
		}
	}
	
	return flag;
}

bool CluVNS::twoOpt(void)
{
	bool flag = false;
	
	for (int v = 0; v < cluVRPinst_->getnVehicles(); v++)		//for every vehicle
	{
		CluTrip* cluTrip = current_->getTrip(v);

		if (cluTrip->getSize() < 6) continue;

		for (int i = 1; i < cluTrip->getSize() - 4; i++)
		{
			for (int j = i + 3; j < cluTrip->getSize() - 1; j++)
			{
				double diff = \
					- cluVRPinst_->getDistClusters(cluTrip->getCluster(i - 1), cluTrip->getCluster(i))\
					- cluVRPinst_->getDistClusters(cluTrip->getCluster(j), cluTrip->getCluster(j + 1))\
					+ cluVRPinst_->getDistClusters(cluTrip->getCluster(i - 1), cluTrip->getCluster(j))\
					+ cluVRPinst_->getDistClusters(cluTrip->getCluster(i), cluTrip->getCluster(j + 1));

				if (diff < -.01)
				{
					Pos a(v, i), b(v, j);
					Move::twoOpt(current_, a, b);
					cluTrip->altDist(diff);
					flag = true;
				}
			}
		}
	}

	return flag;
}

bool CluVNS::intraVehicleOrOpt(void)
{
	bool flag = false;

	for (int N = 2; N < 5; N++)	//number of clusters moved together
	{
		for (int v = 0; v < cluVRPinst_->getnVehicles(); v++)		//for every vehicle
		{
			CluTrip* cluTrip = current_->getTrip(v);

			if (cluTrip->getSize() > (N + 2))			//need enough clusters to move
			{
				for (int i = 1; i < (cluTrip->getSize() - N); i++)
				{
					if (i < (cluTrip->getSize() - N + 1))
					{
						for (int j = i + N; j < (cluTrip->getSize() - 1); j++)
						{
							double diff = \
								- cluVRPinst_->getDistClusters(cluTrip->getCluster(i - 1), cluTrip->getCluster(i))\
								- cluVRPinst_->getDistClusters(cluTrip->getCluster(i + N - 1), cluTrip->getCluster(i + N))\
								- cluVRPinst_->getDistClusters(cluTrip->getCluster(j), cluTrip->getCluster(j + 1))\
								+ cluVRPinst_->getDistClusters(cluTrip->getCluster(i - 1), cluTrip->getCluster(i + N))\
								+ cluVRPinst_->getDistClusters(cluTrip->getCluster(j), cluTrip->getCluster(i))\
								+ cluVRPinst_->getDistClusters(cluTrip->getCluster(i + N - 1), cluTrip->getCluster(j + 1));

							if (diff < -.01)
							{
								Pos a(v, i), b(v, j);
								Move::orOpt(current_, a, b, N);
								cluTrip->altDist(diff);
								flag = true;
							}
						}
					}

					if (i > 2)
					{
						for (int j = 1; j < (i - 1); j++)
						{
							double diff = \
								- cluVRPinst_->getDistClusters(cluTrip->getCluster(i - 1), cluTrip->getCluster(i))\
								- cluVRPinst_->getDistClusters(cluTrip->getCluster(i + N - 1), cluTrip->getCluster(i + N))\
								- cluVRPinst_->getDistClusters(cluTrip->getCluster(j), cluTrip->getCluster(j + 1))\
								+ cluVRPinst_->getDistClusters(cluTrip->getCluster(i - 1), cluTrip->getCluster(i + N))\
								+ cluVRPinst_->getDistClusters(cluTrip->getCluster(j), cluTrip->getCluster(i))\
								+ cluVRPinst_->getDistClusters(cluTrip->getCluster(i + N - 1), cluTrip->getCluster(j + 1));

							if (diff < -.01)
							{
								Pos a(v, i), b(v, j);
								Move::orOpt(current_, a, b, N);
								cluTrip->altDist(diff);
								flag = true;
							}
						}
					}
				}
			}
		}
	}

	return flag;
}

/* INTER VEHICLE NEIGHBOURHOODS */

bool CluVNS::interVehicleSwap(void)
{
	bool flag = false;
	
	for (int v1 = 0; v1 < cluVRPinst_->getnVehicles() - 1; v1++)		//for every vehicle
	{
		for (int v2 = v1 + 1; v2 < cluVRPinst_->getnVehicles(); v2++)
		{
			for (int i = 1; i < current_->getTrip(v1)->getSize() - 1; i++)
			{
				for (int j = 1; j < current_->getTrip(v2)->getSize() - 1; j++)
				{
					//check feasibility
					int c1 = \
						current_->getTrip(v1)->getTotalDemand() \
						- current_->getTrip(v1)->getCluster(i)->getDemand() \
						+ current_->getTrip(v2)->getCluster(j)->getDemand();
					int c2 = \
						current_->getTrip(v2)->getTotalDemand() \
						- current_->getTrip(v2)->getCluster(j)->getDemand() \
						+ current_->getTrip(v1)->getCluster(i)->getDemand();

					if (c1 <= cluVRPinst_->getVehicleCapacity() && c2 <= cluVRPinst_->getVehicleCapacity())
					{
						//check distance
						double d1 = \
							- cluVRPinst_->getDistClusters(current_->getTrip(v1)->getCluster(i - 1), current_->getTrip(v1)->getCluster(i))\
							- cluVRPinst_->getDistClusters(current_->getTrip(v1)->getCluster(i), current_->getTrip(v1)->getCluster(i + 1))\
							+ cluVRPinst_->getDistClusters(current_->getTrip(v1)->getCluster(i - 1), current_->getTrip(v2)->getCluster(j))\
							+ cluVRPinst_->getDistClusters(current_->getTrip(v2)->getCluster(j), current_->getTrip(v1)->getCluster(i + 1));
						double d2 = \
							- cluVRPinst_->getDistClusters(current_->getTrip(v2)->getCluster(j - 1), current_->getTrip(v2)->getCluster(j))\
							- cluVRPinst_->getDistClusters(current_->getTrip(v2)->getCluster(j), current_->getTrip(v2)->getCluster(j + 1))\
							+ cluVRPinst_->getDistClusters(current_->getTrip(v2)->getCluster(j - 1), current_->getTrip(v1)->getCluster(i))\
							+ cluVRPinst_->getDistClusters(current_->getTrip(v1)->getCluster(i), current_->getTrip(v2)->getCluster(j + 1));

						if (d1 + d2 < -.01)
						{
							Pos a(v1, i), b(v2, j);
							Move::swap(current_, a, b);
							current_->getTrip(v1)->altDist(d1);
							current_->getTrip(v2)->altDist(d2);
							current_->getTrip(v1)->setTotalDemand(c1);
							current_->getTrip(v2)->setTotalDemand(c2);
							flag = true;
						}
					}
				}
			}
		}
	}

	return false;
}

bool CluVNS::interVehicleRelocate(void)
{
	bool flag = false;	
	
	for (int v1 = 0; v1 < cluVRPinst_->getnVehicles(); v1++)		//for every vehicle
	{
		if (current_->getTrip(v1)->getSize() > 3)	//otherwise, cluster can not be removed
		{
			for (int v2 = 0; v2 < cluVRPinst_->getnVehicles(); v2++)
			{
				if (v1 == v2) continue;			//intra vehicle scenario

				for (int i = 1; i < current_->getTrip(v1)->getSize() - 1; i++)
				{
					for (int j = 0; j < current_->getTrip(v2)->getSize() - 1; j++)
					{
						//check feasibility
						if (current_->getTrip(v1)->getCluster(i)->getDemand() <= current_->getTrip(v2)->getSpareCapacity())
						{
							//check distance
							double d1 = \
								- cluVRPinst_->getDistClusters(current_->getTrip(v1)->getCluster(i - 1), current_->getTrip(v1)->getCluster(i))\
								- cluVRPinst_->getDistClusters(current_->getTrip(v1)->getCluster(i), current_->getTrip(v1)->getCluster(i + 1))\
								+ cluVRPinst_->getDistClusters(current_->getTrip(v1)->getCluster(i - 1), current_->getTrip(v1)->getCluster(i + 1));
							double d2 = \
								- cluVRPinst_->getDistClusters(current_->getTrip(v2)->getCluster(j), current_->getTrip(v2)->getCluster(j + 1))\
								+ cluVRPinst_->getDistClusters(current_->getTrip(v2)->getCluster(j), current_->getTrip(v1)->getCluster(i))\
								+ cluVRPinst_->getDistClusters(current_->getTrip(v1)->getCluster(i), current_->getTrip(v2)->getCluster(j + 1));

							if (d1 + d2 < -.01)
							{
								current_->getTrip(v1)->altTotalDemand(-current_->getTrip(v1)->getCluster(i)->getDemand());
								current_->getTrip(v2)->altTotalDemand(current_->getTrip(v1)->getCluster(i)->getDemand());
								Pos a(v1, i), b(v2, j);
								Move::relocate(current_, a, b);
								current_->getTrip(v1)->altDist(d1);
								current_->getTrip(v2)->altDist(d2);
								flag = true;		

								//additional checks and adjustments
								if(i != 1) i--;
								if(j != 0) j--;
								if (current_->getTrip(v1)->getSize() < 4)
								{
									return flag;
								}
							}
						}
					}
				}
			}
		}
	}

	return flag;
}

bool CluVNS::interVehicleOrOpt(void)
{
	for (int N = 2; N < 5; N++)
	{
		for (int v1 = 0; v1 < cluVRPinst_->getnVehicles(); v1++)		//for every vehicle
		{
			if (current_->getTrip(v1)->getSize() > (N + 2))			//need enough clusters to move
			{
				for (int v2 = 0; v2 < cluVRPinst_->getnVehicles(); v2++)		//for every vehicle
				{
					if (v1 == v2 || current_->getTrip(v2)->getSpareCapacity() < N) continue;				//intra vehicle scenario

					for (int i = 1; i < current_->getTrip(v1)->getSize() - N; i++)
					{
						for (int j = 0; j < current_->getTrip(v2)->getSize() - 1; j++)
						{
							//check feasibility
							int extraDemand = 0;
							for (int k = 0; k < N; k++)
							{
								extraDemand += current_->getTrip(v1)->getCluster(i + k)->getDemand();
							}

							if (current_->getTrip(v2)->getSpareCapacity() >= extraDemand)
							{
								//check distance (only total difference)
								double d = \
									- cluVRPinst_->getDistClusters(current_->getTrip(v1)->getCluster(i - 1), current_->getTrip(v1)->getCluster(i))\
									- cluVRPinst_->getDistClusters(current_->getTrip(v1)->getCluster(i + N - 1), current_->getTrip(v1)->getCluster(i + N))\
									+ cluVRPinst_->getDistClusters(current_->getTrip(v1)->getCluster(i - 1), current_->getTrip(v1)->getCluster(i + N))\
									- cluVRPinst_->getDistClusters(current_->getTrip(v2)->getCluster(j), current_->getTrip(v2)->getCluster(j + 1))\
									+ cluVRPinst_->getDistClusters(current_->getTrip(v2)->getCluster(j), current_->getTrip(v1)->getCluster(i))\
									+ cluVRPinst_->getDistClusters(current_->getTrip(v1)->getCluster(i + N - 1), current_->getTrip(v2)->getCluster(j + 1));

								if (d < -.01)	//move is feasible and reduces the total distance
								{
									//split differences over the two trips
									double d1 = \
										+ cluVRPinst_->getDistClusters(current_->getTrip(v1)->getCluster(i - 1), current_->getTrip(v1)->getCluster(i + N));
									for (int k = 0; k < N + 1; k++)
									{
										d1 -= cluVRPinst_->getDistClusters(current_->getTrip(v1)->getCluster(i + k - 1), current_->getTrip(v1)->getCluster(i + k));
									}

									double d2 = \
										- cluVRPinst_->getDistClusters(current_->getTrip(v2)->getCluster(j), current_->getTrip(v2)->getCluster(j + 1))\
										+ cluVRPinst_->getDistClusters(current_->getTrip(v2)->getCluster(j), current_->getTrip(v1)->getCluster(i))\
										+ cluVRPinst_->getDistClusters(current_->getTrip(v1)->getCluster(i + N - 1), current_->getTrip(v2)->getCluster(j + 1));
									for (int k = 0; k < N - 1; k++)
									{
										d2 += cluVRPinst_->getDistClusters(current_->getTrip(v1)->getCluster(i + k), current_->getTrip(v1)->getCluster(i + k + 1));
									}

									//perform move
									Pos a(v1, i), b(v2, j);
									Move::orOpt(current_, a, b, N);
									current_->getTrip(v1)->altDist(d1);
									current_->getTrip(v2)->altDist(d2);
									current_->getTrip(v1)->altTotalDemand(-extraDemand);
									current_->getTrip(v2)->altTotalDemand(extraDemand);
									return true;
								}
							}
						}
					}					
				}
			}
		}
	}
	
	return false;
}

/********************************/

bool CluVNS::findNeighbour(void)
{
	if (nbhSequence_.at(currentNBH_) == 0)
	{
		return intraVehicleSwap();
	}
	else if (nbhSequence_.at(currentNBH_) == 1)
	{
		return intraVehicleRelocate();
	}
	else if (nbhSequence_.at(currentNBH_) == 2)
	{
		return twoOpt();
	}
	else if (nbhSequence_.at(currentNBH_) == 3)
	{
		return intraVehicleOrOpt();
	}
	else if (nbhSequence_.at(currentNBH_) == 4)
	{
		return interVehicleSwap();
	}
	else if (nbhSequence_.at(currentNBH_) == 5)
	{
		return interVehicleRelocate();
	}
	else
	{
		return interVehicleOrOpt();
	}

	return false;
}

CluVNS::CluVNS(CluVRPinst* cluVRPinst) : VNS(cluVRPinst)
{
	nNBHs_ = 7;

	for (int i = 0; i < nNBHs_; i++)
	{
		nbhSequence_.push_back(i);
	}
}

CluVNS::~CluVNS(){}

void CluVNS::run(ClusterSolution*& s)
{
	//shuffle the order in which the neighbourhoods are checked
	std::random_shuffle(nbhSequence_.begin(), nbhSequence_.end());

	current_ = s;
	currentNBH_ = 0;

	while (currentNBH_ < nNBHs_)
	{
		if (findNeighbour())
		{
			currentNBH_ = 0;
			continue;
		}
		else
		{
			currentNBH_++;
		}
	}
	current_->calcTotalDist();
}

/////////////////////////////////////////////////////////

/* INTRA VEHICLE NEIGHBOURHOODS */

bool NodVNS::intraVehicleSwap(void)
{
	NodTrip* nodTrip = nullptr;
	bool flag = false;

	if (Params::HARD_CLUSTER)
	{
		for (int v = 0; v < current_->getnTrips(); v++)
		{
			nodTrip = current_->getTrip(v);

			int first = 1;	//index of the first node of the current cluster
			do {
				Cluster* currentCluster = cluVRPinst_->getCluster(nodTrip->getNode(first)->cluster);

				if (currentCluster->getnNodes() > 1)
				{
					int last = first + currentCluster->getnNodes() - 1;	//index of the last node of the current cluster

					for (int i = first; i < last; i++)
					{
						for (int j = first + 1; j <= last; j++)
						{
							double d;

							if (j == i + 1)
							{
								d = \
									- cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i))\
									- cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(j + 1))\
									+ cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(j))\
									+ cluVRPinst_->getDistNodes(nodTrip->getNode(i), nodTrip->getNode(j + 1));
							}
							else
							{
								d = \
									- cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i))\
									- cluVRPinst_->getDistNodes(nodTrip->getNode(i), nodTrip->getNode(i + 1))\
									- cluVRPinst_->getDistNodes(nodTrip->getNode(j - 1), nodTrip->getNode(j))\
									- cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(j + 1))\
									+ cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(j))\
									+ cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(i + 1))\
									+ cluVRPinst_->getDistNodes(nodTrip->getNode(j - 1), nodTrip->getNode(i))\
									+ cluVRPinst_->getDistNodes(nodTrip->getNode(i), nodTrip->getNode(j + 1));
							}
							if (d < -.01)
							{
								Pos a(v, i), b(v, j);
								Move::swap(current_, a, b);
								nodTrip->altDist(d);
								flag = true;
							}
						}
					}

				}
				first += currentCluster->getnNodes();
			} while (first < nodTrip->getSize() - 1);
		}
	}
	else
	{			//soft cluster constraints
		for (int v = 0; v < current_->getnTrips(); v++)		//for every vehicle
		{
			nodTrip = current_->getTrip(v);
			if (nodTrip->getSize() < 4) continue;
			
			for (int i = 1; i < nodTrip->getSize() - 2; i++)
			{
				for (int j = i + 1; j < nodTrip->getSize() - 1; j++)
				{
					double d;

					if (j == i + 1)
					{
						d = \
							- cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i))\
							- cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(j + 1))\
							+ cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(j))\
							+ cluVRPinst_->getDistNodes(nodTrip->getNode(i), nodTrip->getNode(j + 1));
					}
					else
					{
						d = \
							- cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i))\
							- cluVRPinst_->getDistNodes(nodTrip->getNode(i), nodTrip->getNode(i + 1))\
							- cluVRPinst_->getDistNodes(nodTrip->getNode(j - 1), nodTrip->getNode(j))\
							- cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(j + 1))\
							+ cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(j))\
							+ cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(i + 1))\
							+ cluVRPinst_->getDistNodes(nodTrip->getNode(j - 1), nodTrip->getNode(i))\
							+ cluVRPinst_->getDistNodes(nodTrip->getNode(i), nodTrip->getNode(j + 1));
					}

					if (d < -.01)
					{
						Pos a(v, i), b(v, j);
						Move::swap(current_, a, b);
						nodTrip->altDist(d);
						flag = true;
					}
				}
			}
		}
	}
	return flag;
}

bool NodVNS::intraVehicleRelocate(void)
{
	NodTrip* nodTrip = nullptr;
	bool flag = false;

	if (Params::HARD_CLUSTER)
	{
		for (int v = 0; v < current_->getnTrips(); v++)
		{
			nodTrip = current_->getTrip(v);

			int first = 1;	//index of the first node of the current cluster

			do {

				Cluster* currentCluster = cluVRPinst_->getCluster(nodTrip->getNode(first)->cluster);
				if (currentCluster->getnNodes() > 1)
				{
					int last = first + currentCluster->getnNodes() - 1;	//index of the last node of the current cluster

					for (int i = first; i <= last; i++)
					{
						for (int j = first - 1; j < last; j++)
						{
							if (i == j || i == j + 1) continue;

							double d = \
								- cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i))\
								- cluVRPinst_->getDistNodes(nodTrip->getNode(i), nodTrip->getNode(i + 1))\
								+ cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i + 1))\
								- cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(j + 1))\
								+ cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(i))\
								+ cluVRPinst_->getDistNodes(nodTrip->getNode(i), nodTrip->getNode(j + 1));

							if (d < -.01)
							{
								Pos a(v, i), b(v, j);
								Move::relocate(current_, a, b);
								nodTrip->altDist(d);
								//current_->totalDist_ += d;
								flag = true;
							}
						}
					}
				}

				first += currentCluster->getnNodes();

			} while (first < nodTrip->getSize() - 1);
		}
	}
	else
	{			//soft cluster constraints
		for (int v = 0; v < current_->getnTrips(); v++)
		{
			nodTrip = current_->getTrip(v);
			if (nodTrip->getSize() < 4) continue;

			for (int i = 1; i < nodTrip->getSize() - 1; i++)
			{
				if (i > 2)
				{
					for (int j = 0; j < i - 1; j++)			// if j = i - 1 nothing changes
					{
						double d = \
							- cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i))\
							- cluVRPinst_->getDistNodes(nodTrip->getNode(i), nodTrip->getNode(i + 1))\
							+ cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i + 1))\
							- cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(j + 1))\
							+ cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(i))\
							+ cluVRPinst_->getDistNodes(nodTrip->getNode(i), nodTrip->getNode(j + 1));

						if (d < -.01)
						{
							Pos a(v, i), b(v, j);
							Move::relocate(current_, a, b);
							nodTrip->altDist(d);
							flag = true;
						}
					}
				}
				if (i < nodTrip->getSize() - 2)
				{
					for (int j = i + 1; j < nodTrip->getSize() - 1; j++)
					{
						double d = \
							- cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i))\
							- cluVRPinst_->getDistNodes(nodTrip->getNode(i), nodTrip->getNode(i + 1))\
							+ cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i + 1))\
							- cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(j + 1))\
							+ cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(i))\
							+ cluVRPinst_->getDistNodes(nodTrip->getNode(i), nodTrip->getNode(j + 1));

						if (d < -.01)
						{
							Pos a(v, i), b(v, j);
							Move::relocate(current_, a, b);
							nodTrip->altDist(d);
							flag = true;
						}
					}
				}
			}
		}
	}

	return flag;
}

bool NodVNS::intraVehicleTwoOpt(void)
{	
	NodTrip* nodTrip = nullptr;
	bool flag = false;

	if (Params::HARD_CLUSTER)
	{
		for (int v = 0; v < current_->getnTrips(); v++)
		{
			nodTrip = current_->getTrip(v);

			int first = 1;	//index of the first node of the current cluster

			do {

				Cluster* currentCluster = cluVRPinst_->getCluster(nodTrip->getNode(first)->cluster);
				if (currentCluster->getnNodes() > 3)
				{
					int last = first + currentCluster->getnNodes() - 1;	//index of the last node of the current cluster

					for (int i = first; i <= last; i++)
					{
						for (int j = i + 3; j <= last; j++)
						{
							double d = \
								- cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i))\
								- cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(j + 1))\
								+ cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(j))\
								+ cluVRPinst_->getDistNodes(nodTrip->getNode(i), nodTrip->getNode(j + 1));

							if (d < -.01)
							{
								Pos a(v, i), b(v, j);
								Move::twoOpt(current_, a, b);
								nodTrip->altDist(d);
								flag = true;
							}
						}
					}

				}

				first += currentCluster->getnNodes();

			} while (first < nodTrip->getSize() - 1);
		}
	}
	else
	{			//soft cluster constraints
		for (int v = 0; v < current_->getnTrips(); v++)		//for every vehicle
		{
			nodTrip = current_->getTrip(v);

			if (nodTrip->getSize() > 5)	//otherwise two opt = swap move
			{
				for (int i = 1; i < nodTrip->getSize() - 4; i++)
				{
					for (int j = i + 3; j < nodTrip->getSize() - 1; j++)
					{
						double d = \
							- cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i))\
							- cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(j + 1))\
							+ cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(j))\
							+ cluVRPinst_->getDistNodes(nodTrip->getNode(i), nodTrip->getNode(j + 1));

						if (d < -.01)
						{
							Pos a(v, i), b(v, j);
							Move::twoOpt(current_, a, b);
							nodTrip->altDist(d);
							flag = true;
						}
					}
				}
			}
		}
	}
	return flag;
}

bool NodVNS::intraVehicleOrOpt(void)
{
	NodTrip* nodTrip = nullptr;
	bool flag = false;

	if (Params::HARD_CLUSTER)
	{
		for (int N = 2; N < 5; N++)	//number of clusters moved together
		{
			for (int v = 0; v < current_->getnTrips(); v++)
			{
				nodTrip = current_->getTrip(v);

				int first = 1;	//index of the first node of the current cluster

				do {

					Cluster* currentCluster = cluVRPinst_->getCluster(nodTrip->getNode(first)->cluster);
					if (currentCluster->getnNodes() > N)
					{
						int last = first + currentCluster->getnNodes() - 1;	//index of the last node of the current cluster

						for (int i = first; i <= last - N + 1; i++) 
						{
							if (i < (last - N + 1)) 
							{
								for (int j = i + N; j <= last; j++) 
								{
									double d =\
										- cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i))\
										- cluVRPinst_->getDistNodes(nodTrip->getNode(i + N - 1), nodTrip->getNode(i + N))\
										- cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(j + 1))\
										+ cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i + N))\
										+ cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(i))\
										+ cluVRPinst_->getDistNodes(nodTrip->getNode(i + N - 1), nodTrip->getNode(j + 1));

									if (d < -.01) 
									{
										Pos a(v, i), b(v, j);
										Move::orOpt(current_, a, b, N);
										nodTrip->altDist(d);
										flag = true;
									}

								}
							}
							if (i >(first + 1)) 
							{
								for (int j = first; j < (i - 1); j++) 
								{
									double d =\
										- cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i))\
										- cluVRPinst_->getDistNodes(nodTrip->getNode(i + N - 1), nodTrip->getNode(i + N))\
										- cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(j + 1))\
										+ cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i + N))\
										+ cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(i))\
										+ cluVRPinst_->getDistNodes(nodTrip->getNode(i + N - 1), nodTrip->getNode(j + 1));

									if (d < -.01) 
									{
										Pos a(v, i), b(v, j);
										Move::orOpt(current_, a, b, N);
										nodTrip->altDist(d);
										flag = true;
									}
								}
							}

						}
					}
					first += currentCluster->getnNodes();

				} while (first < nodTrip->getSize() - 1);
			}
		}
	}
	else
	{
		for (int N = 2; N < 5; N++)	//number of clusters moved together
		{
			for (int v = 0; v < current_->getnTrips(); v++)		//for every vehicle
			{
				nodTrip = current_->getTrip(v);

				if (nodTrip->getSize() >(N + 2))			//need enough clusters to move
				{
					for (int i = 1; i < (nodTrip->getSize() - N); i++)
					{
						if (i < (nodTrip->getSize() - N + 1))
						{
							for (int j = i + N; j < (nodTrip->getSize() - 1); j++)
							{
								double d = \
									- cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i))\
									- cluVRPinst_->getDistNodes(nodTrip->getNode(i + N - 1), nodTrip->getNode(i + N))\
									- cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(j + 1))\
									+ cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i + N))\
									+ cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(i))\
									+ cluVRPinst_->getDistNodes(nodTrip->getNode(i + N - 1), nodTrip->getNode(j + 1));

								if (d < -.01)
								{
									Pos a(v, i), b(v, j);
									Move::orOpt(current_, a, b, N);
									nodTrip->altDist(d);
									flag = true;
								}
							}
						}

						if (i > 2)
						{
							for (int j = 1; j < (i - 1); j++)
							{
								double d = \
									- cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i))\
									- cluVRPinst_->getDistNodes(nodTrip->getNode(i + N - 1), nodTrip->getNode(i + N))\
									- cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(j + 1))\
									+ cluVRPinst_->getDistNodes(nodTrip->getNode(i - 1), nodTrip->getNode(i + N))\
									+ cluVRPinst_->getDistNodes(nodTrip->getNode(j), nodTrip->getNode(i))\
									+ cluVRPinst_->getDistNodes(nodTrip->getNode(i + N - 1), nodTrip->getNode(j + 1));

								if (d < -.01)
								{
									Pos a(v, i), b(v, j);
									Move::orOpt(current_, a, b, N);
									nodTrip->altDist(d);
									flag = true;
								}
							}
						}
					}
				}
			}
		}
	}
	return flag;
}

bool NodVNS::clusterSwap(void)
{
	NodTrip* nodTrip1 = nullptr;
	NodTrip* nodTrip2 = nullptr;

	for (int v1 = 0; v1 < current_->getnTrips(); v1++)
	{
		nodTrip1 = current_->getTrip(v1);

		int it1 = 1;

		while (it1 < nodTrip1->getSize() - 1)
		{
			int c1 = nodTrip1->getNode(it1)->cluster;
			int n1 = cluVRPinst_->getCluster(c1)->getnNodes();

			for (int v2 = 0; v2 < current_->getnTrips(); v2++)
			{
				nodTrip2 = current_->getTrip(v2);

				if (v1 == v2)
				{
					int it2 = it1 + n1;

					while (it2 < nodTrip1->getSize() - 1)
					{
						int c2 = nodTrip2->getNode(it2)->cluster;
						int n2 = cluVRPinst_->getCluster(c2)->getnNodes();

						if (it1 + n1 == it2)	//two clusters are next to each other
						{
							NodTrip* originalSequence = new NodTrip(cluVRPinst_);
							
							for (int i = -1; i < n1 + n2 + 1; i++) originalSequence->addStop(nodTrip1->getNode(it1 + i));

							NodeSolution* sSequence = new NodeSolution(cluVRPinst_);
							NodTrip* newSequence = new NodTrip(originalSequence);							
							sSequence->addTrip(newSequence);

							NodVNS* nodVNS = new NodVNS(cluVRPinst_, true);
							nodVNS->run(sSequence);

							if (sSequence->getTotalDist() < originalSequence->getDistance() - .01)
							{
								for (int i = 0; i < n1 + n2; i++) nodTrip1->vNodes_.at(it1 + i) = sSequence->getTrip(0)->getNode(i+1);
								nodTrip1->altDist(sSequence->getTotalDist() - originalSequence->getDistance());
								return true;
							}

							delete originalSequence;
							delete sSequence;
							delete nodVNS;
						}
						else
						{
							double originalDist1 = 0;
							double originalDist2 = 0;
							for (int i = -1; i < n1; i++) originalDist1 += cluVRPinst_->getDistNodes(nodTrip1->getNode(it1 + i), nodTrip1->getNode(it1 + i + 1));
							for (int i = -1; i < n2; i++) originalDist2 += cluVRPinst_->getDistNodes(nodTrip2->getNode(it2 + i), nodTrip2->getNode(it2 + i + 1));

							NodeSolution* sSequence1 = new NodeSolution(cluVRPinst_);
							NodeSolution* sSequence2 = new NodeSolution(cluVRPinst_);
							NodTrip* newSequence1 = new NodTrip(cluVRPinst_);
							NodTrip* newSequence2 = new NodTrip(cluVRPinst_);

							newSequence1->addStop(nodTrip1->getNode(it1 - 1));
							for (int i = 0; i < n2; i++) newSequence1->addStop(nodTrip2->getNode(it2 + i));
							newSequence1->addStop(nodTrip1->getNode(it1 + n1));

							newSequence2->addStop(nodTrip2->getNode(it2 - 1));
							for (int i = 0; i < n1; i++) newSequence2->addStop(nodTrip1->getNode(it1 + i));
							newSequence2->addStop(nodTrip2->getNode(it2 + n2));

							sSequence1->addTrip(newSequence1);
							sSequence2->addTrip(newSequence2);

							NodVNS* nodVNS1 = new NodVNS(cluVRPinst_, true);
							NodVNS* nodVNS2 = new NodVNS(cluVRPinst_, true);
							nodVNS1->run(sSequence1);
							nodVNS2->run(sSequence2);

							if (sSequence1->getTotalDist() + sSequence2->getTotalDist() - originalDist1 - originalDist2 < -.01)
							{
								for (int i = 0; i < n2; i++) nodTrip2->vNodes_.erase(nodTrip2->vNodes_.begin() + it2);
								for (int i = 0; i < n1; i++) nodTrip2->vNodes_.insert(nodTrip2->vNodes_.begin() + it2 + i, newSequence2->vNodes_.at(i + 1));
								
								for (int i = 0; i < n1; i++) nodTrip1->vNodes_.erase(nodTrip1->vNodes_.begin() + it1);
								for (int i = 0; i < n2; i++) nodTrip1->vNodes_.insert(nodTrip1->vNodes_.begin() + it1 + i, newSequence1->vNodes_.at(i + 1));

								nodTrip1->altDist(sSequence1->getTotalDist() + sSequence2->getTotalDist() - originalDist1 - originalDist2);
								return true;
							}

							delete sSequence1;
							delete sSequence2;
							delete nodVNS1;
							delete nodVNS2;
						}
						it2 += n2;
					}
				}
				else
				{	
					//v1 != v2
					int it2 = 1;

					while (it2 < nodTrip2->getSize() - 1)
					{
						int c2 = nodTrip2->getNode(it2)->cluster;
						int n2 = cluVRPinst_->getCluster(c2)->getnNodes();

						if (nodTrip1->getSpareCapacity() + cluVRPinst_->getCluster(c1)->getDemand() >= cluVRPinst_->getCluster(c2)->getDemand()\
							&& nodTrip2->getSpareCapacity() + cluVRPinst_->getCluster(c2)->getDemand() >= cluVRPinst_->getCluster(c1)->getDemand())
						{
							double originalDist1 = 0;
							double originalDist2 = 0;
							for (int i = -1; i < n1; i++) originalDist1 += cluVRPinst_->getDistNodes(nodTrip1->getNode(it1 + i), nodTrip1->getNode(it1 + i + 1));
							for (int i = -1; i < n2; i++) originalDist2 += cluVRPinst_->getDistNodes(nodTrip2->getNode(it2 + i), nodTrip2->getNode(it2 + i + 1));

							NodeSolution* sSequence1 = new NodeSolution(cluVRPinst_);
							NodeSolution* sSequence2 = new NodeSolution(cluVRPinst_);
							NodTrip* newSequence1 = new NodTrip(cluVRPinst_);
							NodTrip* newSequence2 = new NodTrip(cluVRPinst_);

							newSequence1->addStop(nodTrip1->getNode(it1 - 1));
							for (int i = 0; i < n2; i++) newSequence1->addStop(nodTrip2->getNode(it2 + i));
							newSequence1->addStop(nodTrip1->getNode(it1 + n1));

							newSequence2->addStop(nodTrip2->getNode(it2 - 1));
							for (int i = 0; i < n1; i++) newSequence2->addStop(nodTrip1->getNode(it1 + i));
							newSequence2->addStop(nodTrip2->getNode(it2 + n2));

							sSequence1->addTrip(newSequence1);
							sSequence2->addTrip(newSequence2);

							NodVNS* nodVNS1 = new NodVNS(cluVRPinst_, true);
							NodVNS* nodVNS2 = new NodVNS(cluVRPinst_, true);
							nodVNS1->run(sSequence1);
							nodVNS2->run(sSequence2);

							if ((sSequence1->getTotalDist() + sSequence2->getTotalDist()) - (originalDist1 + originalDist2) < -.01)
							{
								for (int i = 0; i < n2; i++) nodTrip2->vNodes_.erase(nodTrip2->vNodes_.begin() + it2);
								for (int i = 0; i < n1; i++) nodTrip2->vNodes_.insert(nodTrip2->vNodes_.begin() + it2 + i, newSequence2->vNodes_.at(i + 1));

								for (int i = 0; i < n1; i++) nodTrip1->vNodes_.erase(nodTrip1->vNodes_.begin() + it1);
								for (int i = 0; i < n2; i++) nodTrip1->vNodes_.insert(nodTrip1->vNodes_.begin() + it1 + i, newSequence1->vNodes_.at(i + 1));

								nodTrip1->size_ = nodTrip1->vNodes_.size();
								nodTrip2->size_ = nodTrip2->vNodes_.size();
								nodTrip1->altDist(sSequence1->getTotalDist() - originalDist1);
								nodTrip2->altDist(sSequence2->getTotalDist() - originalDist2);
								nodTrip1->altTotalDemand(cluVRPinst_->getCluster(c2)->getDemand() - cluVRPinst_->getCluster(c1)->getDemand());
								nodTrip2->altTotalDemand(cluVRPinst_->getCluster(c1)->getDemand() - cluVRPinst_->getCluster(c2)->getDemand());
								current_->calcTotalDist();
								return true;
							}

							delete sSequence1;
							delete sSequence2;
							delete nodVNS1;
							delete nodVNS2;
						}

						it2 += n2;
					}
				}
			}

			it1 += n1;
		}
	}
	return false;
}

bool NodVNS::clusterRelocate(void)
{
	NodTrip* nodTrip1 = nullptr;
	NodTrip* nodTrip2 = nullptr;

	for (int v1 = 0; v1 < current_->getnTrips(); v1++)
	{
		nodTrip1 = current_->getTrip(v1);

		int it1 = 1;
		int size1 = current_->getTrip(v1)->getSize();

		while (it1 < size1 - 1)
		{
			int c1 = nodTrip1->getNode(it1)->cluster;
			int n1 = cluVRPinst_->getCluster(c1)->getnNodes();

			for (int v2 = 0; v2 < current_->getnTrips(); v2++)
			{
				nodTrip2 = current_->getTrip(v2);

				if (v1 == v2)
				{
					int it2 = 0;

					while (it2 < size1 - 1)
					{
						int c2 = nodTrip2->getNode(it2)->cluster;
						int n2 = cluVRPinst_->getCluster(c2)->getnNodes();

						if (it1 != it2 && it2 + n2 != it1) 
						{
							//relocate sequence 1 into position 2
							double originalDist1 = 0;
							for(int i = -1; i < n1; i++) originalDist1 += cluVRPinst_->getDistNodes(nodTrip1->getNode(it1 + i), nodTrip1->getNode(it1 + i + 1));
							double originalDist2 = cluVRPinst_->getDistNodes(nodTrip2->getNode(it2 + n2 - 1), nodTrip2->getNode(it2 + n2));
							double newDist1 = cluVRPinst_->getDistNodes(nodTrip1->getNode(it1 - 1), nodTrip1->getNode(it1 + n1));

							NodeSolution* sSequence = new NodeSolution(cluVRPinst_);
							NodTrip* newSequence = new NodTrip(cluVRPinst_);

							newSequence->addStop(nodTrip2->getNode(it2 + n2 - 1));
							for (int i = 0; i < n1; i++) newSequence->addStop(nodTrip1->getNode(it1 + i));
							newSequence->addStop(nodTrip2->getNode(it2 + n2));

							sSequence->addTrip(newSequence);

							NodVNS* nodVNS = new NodVNS(cluVRPinst_, true);
							nodVNS->run(sSequence);

							if (sSequence->getTotalDist() + newDist1 - originalDist1 - originalDist2 < -.01)
							{
								if (it1 > it2)
								{
									for (int i = 0; i < n1; i++) nodTrip1->vNodes_.erase(nodTrip1->vNodes_.begin() + it1);
									for (int i = 0; i < n1; i++) nodTrip2->vNodes_.insert(nodTrip2->vNodes_.begin() + it2 + n2 + i, newSequence->getNode(i + 1));
								}
								else
								{
									for (int i = 0; i < n1; i++) nodTrip2->vNodes_.insert(nodTrip2->vNodes_.begin() + it2 + n2 + i, newSequence->getNode(i + 1));
									for (int i = 0; i < n1; i++) nodTrip1->vNodes_.erase(nodTrip1->vNodes_.begin() + it1);
								}

								nodTrip1->altDist(sSequence->getTotalDist() + newDist1 - originalDist1 - originalDist2);
								current_->calcTotalDist();
								return true;
							}

							delete sSequence;
							delete nodVNS;

						}
						it2 += n2;
					}
				}
				else
				{		//v1 != v2
					int it2 = 0;
					int size2 = current_->getTrip(v2)->getSize();

					while (it2 < size2 - 1)
					{
						int c2 = current_->getTrip(v2)->getNode(it2)->cluster;
						int n2 = cluVRPinst_->getCluster(c2)->getnNodes();

						if (nodTrip2->getSpareCapacity() - cluVRPinst_->getCluster(c1)->getDemand() >= 0)
						{
							//relocate sequence 1 into position 2
							double originalDist1 = 0;
							for (int i = -1; i < n1; i++) originalDist1 += cluVRPinst_->getDistNodes(nodTrip1->getNode(it1 + i), nodTrip1->getNode(it1 + i + 1));
							double originalDist2 = cluVRPinst_->getDistNodes(nodTrip2->getNode(it2 + n2 - 1), nodTrip2->getNode(it2 + n2));
							double newDist1 = cluVRPinst_->getDistNodes(nodTrip1->getNode(it1 - 1), nodTrip1->getNode(it1 + n1));

							NodeSolution* sSequence = new NodeSolution(cluVRPinst_);
							NodTrip* newSequence = new NodTrip(cluVRPinst_);

							newSequence->addStop(nodTrip2->getNode(it2 + n2 - 1));
							for (int i = 0; i < n1; i++) newSequence->addStop(nodTrip1->getNode(it1 + i));
							newSequence->addStop(nodTrip2->getNode(it2 + n2));

							sSequence->addTrip(newSequence);

							NodVNS* nodVNS = new NodVNS(cluVRPinst_, true);
							nodVNS->run(sSequence);

							if (sSequence->getTotalDist() + newDist1 - originalDist1 - originalDist2 < -.01)
							{

								for (int i = 0; i < n1; i++) nodTrip1->vNodes_.erase(nodTrip1->vNodes_.begin() + it1);
								for (int i = 0; i < n1; i++) nodTrip2->vNodes_.insert(nodTrip2->vNodes_.begin() + it2 + n2 + i, newSequence->getNode(i + 1));
							
								nodTrip1->size_ = nodTrip1->vNodes_.size();
								nodTrip2->size_ = nodTrip2->vNodes_.size();
								nodTrip1->altDist(newDist1 - originalDist1);
								nodTrip2->altDist(sSequence->getTotalDist() - originalDist2);
								nodTrip1->altTotalDemand(-cluVRPinst_->getCluster(c1)->getDemand());
								nodTrip2->altTotalDemand(cluVRPinst_->getCluster(c1)->getDemand());
								current_->calcTotalDist();
								return true;
							}

							delete sSequence;
							delete nodVNS;

						}
						it2 += n2;
					}
				}
			}
			it1 += n1;
		}
	}
	return false;
}

bool NodVNS::findNeighbour(void)
{
	if (nbhSequence_.at(currentNBH_) == 0)
	{
		return intraVehicleSwap();
	}
	else if (nbhSequence_.at(currentNBH_) == 1)
	{
		return intraVehicleRelocate();
	}
	else if (nbhSequence_.at(currentNBH_) == 2)
	{
		return intraVehicleTwoOpt();
	}
	else if (nbhSequence_.at(currentNBH_) == 3)
	{
		return intraVehicleOrOpt();
	}
	else if (nbhSequence_.at(currentNBH_) == 4)
	{
		return clusterSwap();
	}
	else
	{
		return clusterRelocate();
	}
	
	return false;
}

NodVNS::NodVNS(CluVRPinst* cluVRPInst, bool innerLoop) :VNS(cluVRPInst)
{
	additionalNBHs_ = false;
	innerLoop_ = innerLoop;
}

NodVNS::~NodVNS(){}

void NodVNS::enableAdditionalNBHs(void)
{
	additionalNBHs_ = true;
	//selectNBHs();
}

void NodVNS::disableAdditionalNBHs(void)
{
	additionalNBHs_ = false;
	//selectNBHs();
}

void NodVNS::selectNBHs(void)
{
	nbhSequence_.clear();

	if (innerLoop_)	nNBHs_ = 3;
	else if (Params::HARD_CLUSTER)
	{
		if (additionalNBHs_)
		{
			nNBHs_ = 2;
			nbhSequence_.push_back(4);
			nbhSequence_.push_back(5);
			return;
		}
		else nNBHs_ = 4;
	}
	else nNBHs_ = 4;

	for (int i = 0; i < nNBHs_; i++) nbhSequence_.push_back(i);
	return;
}

void NodVNS::run(NodeSolution*& s)
{
	//shuffle the order in which the neighbourhoods are checked
	selectNBHs();
	std::random_shuffle(nbhSequence_.begin(), nbhSequence_.end());

	current_ = s;
	currentNBH_ = 0;

	while (currentNBH_ < nNBHs_)
	{
		if (findNeighbour())
		{
			currentNBH_ = 0;
			continue;
		}
		else
		{
			currentNBH_++;
		}
	}
	current_->calcTotalDist();
}