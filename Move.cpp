#include "Move.h"


Move::Move(){}

Move::~Move(){}

void Move::swap(ClusterSolution*& sol, Pos& a, Pos& b)
{
	std::swap(sol->vTrips_.at(a.veh_)->vClusters_.at(a.ind_), sol->vTrips_.at(b.veh_)->vClusters_.at(b.ind_));
}

void Move::relocate(ClusterSolution*& sol, Pos& a, Pos& b)		//place a right after b
{
	if (Pos::equalVehicle(a, b))
	{
		if (a.ind_ < b.ind_)
		{
			sol->vTrips_.at(b.veh_)->vClusters_.insert(sol->vTrips_.at(b.veh_)->vClusters_.begin() + b.ind_ + 1, sol->getTrip(a.veh_)->getCluster(a.ind_));
			sol->vTrips_.at(a.veh_)->vClusters_.erase(sol->vTrips_.at(a.veh_)->vClusters_.begin() + a.ind_);
		}
		else
		{
			sol->vTrips_.at(b.veh_)->vClusters_.insert(sol->vTrips_.at(b.veh_)->vClusters_.begin() + b.ind_ + 1, sol->getTrip(a.veh_)->getCluster(a.ind_));
			sol->vTrips_.at(a.veh_)->vClusters_.erase(sol->vTrips_.at(a.veh_)->vClusters_.begin() + a.ind_ + 1);
		}
	}
	else
	{
		sol->vTrips_.at(b.veh_)->vClusters_.insert(sol->vTrips_.at(b.veh_)->vClusters_.begin() + b.ind_ + 1, sol->getTrip(a.veh_)->getCluster(a.ind_));
		sol->vTrips_.at(a.veh_)->vClusters_.erase(sol->vTrips_.at(a.veh_)->vClusters_.begin() + a.ind_);

		sol->getTrip(b.veh_)->altSize(1);
		sol->getTrip(a.veh_)->altSize(-1);
	}
}

void Move::twoOpt(ClusterSolution*& sol, Pos& a, Pos& b)
{
	do
	{
		std::swap(sol->vTrips_.at(a.veh_)->vClusters_.at(a.ind_), sol->vTrips_.at(a.veh_)->vClusters_.at(b.ind_));
		a.ind_++;
		b.ind_--;
	} while (b.ind_ - a.ind_ > 0);
}

void Move::orOpt(ClusterSolution*& sol, Pos& a, Pos& b, int& N)
{
	if (Pos::equalVehicle(a, b))
	{
		if (a.ind_ < b.ind_)
		{
			for (int i = 0; i < N; i++)
			{
				sol->vTrips_.at(b.veh_)->vClusters_.insert(sol->vTrips_.at(b.veh_)->vClusters_.begin() + b.ind_ + 1 + i, sol->getTrip(a.veh_)->getCluster(a.ind_ + i));
			}
			for (int i = 0; i < N; i++)
			{
				sol->vTrips_.at(a.veh_)->vClusters_.erase(sol->vTrips_.at(a.veh_)->vClusters_.begin() + a.ind_);
			}
			return;
		}
		else
		{
			for (int i = 0; i < N; i++)
			{
				sol->vTrips_.at(a.veh_)->vClusters_.insert(sol->vTrips_.at(a.veh_)->vClusters_.begin() + b.ind_ + 1 + i, sol->getTrip(a.veh_)->getCluster(a.ind_ + 2 * i));
			}
			for (int i = 0; i < N; i++)
			{
				sol->vTrips_.at(b.veh_)->vClusters_.erase(sol->vTrips_.at(b.veh_)->vClusters_.begin() + a.ind_ + N);
			}
			return;
		}
	}
	else
	{			//different vehicles
		for (int i = 0; i < N; i++)
		{
			sol->vTrips_.at(b.veh_)->vClusters_.insert(sol->vTrips_.at(b.veh_)->vClusters_.begin() + b.ind_ + 1 + i, sol->getTrip(a.veh_)->getCluster(a.ind_ + i));
		}
		for (int i = 0; i < N; i++)
		{
			sol->vTrips_.at(a.veh_)->vClusters_.erase(sol->vTrips_.at(a.veh_)->vClusters_.begin() + a.ind_);
		}

		sol->getTrip(a.veh_)->altSize(-N);
		sol->getTrip(b.veh_)->altSize(N);
		return;
	}
}

///////////////////////////////////////////////////////////////////////

void Move::swap(NodeSolution*& sol, Pos& a, Pos& b)
{
	std::swap(sol->vTrips_.at(a.veh_)->vNodes_.at(a.ind_), sol->vTrips_.at(a.veh_)->vNodes_.at(b.ind_));
}

void Move::relocate(NodeSolution*& sol, Pos& a, Pos& b)		//place a right after b
{
	if (Pos::equalVehicle(a, b))
	{
		if (a.ind_ < b.ind_)
		{
			
			sol->vTrips_.at(b.veh_)->vNodes_.insert(sol->vTrips_.at(b.veh_)->vNodes_.begin() + b.ind_ + 1, sol->getTrip(a.veh_)->getNode(a.ind_));
			sol->vTrips_.at(a.veh_)->vNodes_.erase(sol->vTrips_.at(a.veh_)->vNodes_.begin() + a.ind_);
		}
		else
		{
			sol->vTrips_.at(b.veh_)->vNodes_.insert(sol->vTrips_.at(b.veh_)->vNodes_.begin() + b.ind_ + 1, sol->getTrip(a.veh_)->getNode(a.ind_));
			sol->vTrips_.at(a.veh_)->vNodes_.erase(sol->vTrips_.at(a.veh_)->vNodes_.begin() + a.ind_ + 1);
		}
	}
	else
	{
		sol->vTrips_.at(b.veh_)->vNodes_.insert(sol->vTrips_.at(b.veh_)->vNodes_.begin() + b.ind_ + 1, sol->getTrip(a.veh_)->getNode(a.ind_));
		sol->vTrips_.at(a.veh_)->vNodes_.erase(sol->vTrips_.at(a.veh_)->vNodes_.begin() + a.ind_);

		sol->getTrip(b.veh_)->altSize(1);
		sol->getTrip(a.veh_)->altSize(-1);
	}
}

void Move::twoOpt(NodeSolution*& sol, Pos& a, Pos& b)
{
	do
	{
		std::swap(sol->vTrips_.at(a.veh_)->vNodes_.at(a.ind_), sol->vTrips_.at(b.veh_)->vNodes_.at(b.ind_));
		a.ind_++;
		b.ind_--;
	} while (b.ind_ - a.ind_ > 0);
}

void Move::orOpt(NodeSolution*& sol, Pos& a, Pos& b, int& N)
{
	if (Pos::equalVehicle(a, b))
	{
		if (a.ind_ < b.ind_)
		{
			for (int i = 0; i < N; i++)
			{
				sol->vTrips_.at(b.veh_)->vNodes_.insert(sol->vTrips_.at(b.veh_)->vNodes_.begin() + b.ind_ + 1 + i, sol->getTrip(a.veh_)->getNode(a.ind_ + i));
			}
			for (int i = 0; i < N; i++)
			{
				sol->vTrips_.at(a.veh_)->vNodes_.erase(sol->vTrips_.at(a.veh_)->vNodes_.begin() + a.ind_);
			}
			return;
		}
		else
		{
			for (int i = 0; i < N; i++)
			{
				sol->vTrips_.at(a.veh_)->vNodes_.insert(sol->vTrips_.at(a.veh_)->vNodes_.begin() + b.ind_ + 1 + i, sol->getTrip(a.veh_)->getNode(a.ind_ + 2 * i));
			}
			for (int i = 0; i < N; i++)
			{
				sol->vTrips_.at(b.veh_)->vNodes_.erase(sol->vTrips_.at(b.veh_)->vNodes_.begin() + a.ind_ + N);
			}
			return;
		}
	}
	else
	{			//different vehicles
		for (int i = 0; i < N; i++)
		{
			sol->vTrips_.at(b.veh_)->vNodes_.insert(sol->vTrips_.at(b.veh_)->vNodes_.begin() + b.ind_ + 1 + i, sol->getTrip(a.veh_)->getNode(a.ind_ + i));
		}
		for (int i = 0; i < N; i++)
		{
			sol->vTrips_.at(a.veh_)->vNodes_.erase(sol->vTrips_.at(a.veh_)->vNodes_.begin() + a.ind_);
		}

		sol->getTrip(a.veh_)->altSize(-N);
		sol->getTrip(b.veh_)->altSize(N);
		return;
	}
}

void Move::swapClusterNodeLevel(NodeSolution*& sol, Pos& a, Pos& b, int& n1, int& n2)
{
	std::vector<Node*> temp;

	if (a.veh_ == b.veh_ && a.ind_ < b.ind_)
	{
		//back up cluster 2
		for (int i = 0; i < n2; i++)
		{
			temp.push_back(sol->getTrip(b.veh_)->getNode(b.ind_));
			sol->vTrips_.at(b.veh_)->vNodes_.erase(sol->vTrips_.at(b.veh_)->vNodes_.begin() + b.ind_);
		}

		//insert cluster 1 at the position of cluster 2
		for (int i = 0; i < n1; i++)
		{
			sol->vTrips_.at(b.veh_)->vNodes_.insert(sol->vTrips_.at(b.veh_)->vNodes_.begin() + b.ind_, sol->getTrip(a.veh_)->getNode(a.ind_));
			sol->vTrips_.at(a.veh_)->vNodes_.erase(sol->vTrips_.at(a.veh_)->vNodes_.begin() + a.ind_);
		}

		//insert back up cluster 2 at the position of cluster 1
		for (int i = 0; i < n2; i++)
		{
			sol->vTrips_.at(a.veh_)->vNodes_.insert(sol->vTrips_.at(a.veh_)->vNodes_.begin() + a.ind_ + i, temp.at(i));
		}
	}
	else 
	{
		//back up cluster 1
		for (int i = 0; i < n1; i++)
		{
			temp.push_back(sol->getTrip(a.veh_)->getNode(a.ind_));
			sol->vTrips_.at(a.veh_)->vNodes_.erase(sol->vTrips_.at(a.veh_)->vNodes_.begin() + a.ind_);
		}

		//insert cluster 2 at the position of cluster 1
		if (a.veh_ == b.veh_)
		{
			for (int i = 0; i < n2; i++)
			{
				sol->vTrips_.at(a.veh_)->vNodes_.insert(sol->vTrips_.at(a.veh_)->vNodes_.begin() + a.ind_, sol->getTrip(b.veh_)->getNode(b.ind_));
				sol->vTrips_.at(b.veh_)->vNodes_.erase(sol->vTrips_.at(b.veh_)->vNodes_.begin() + b.ind_);
			}
		}
		else
		{
			for (int i = 0; i < n2; i++)
			{
				sol->vTrips_.at(a.veh_)->vNodes_.insert(sol->vTrips_.at(a.veh_)->vNodes_.begin() + a.ind_ + i, sol->getTrip(b.veh_)->getNode(b.ind_));
				sol->vTrips_.at(b.veh_)->vNodes_.erase(sol->vTrips_.at(b.veh_)->vNodes_.begin() + b.ind_);
			}
		}

		//insert back up cluster 1 at the position of cluster 2
		for (int i = 0; i < n1; i++)
		{
			sol->vTrips_.at(b.veh_)->vNodes_.insert(sol->vTrips_.at(b.veh_)->vNodes_.begin() + b.ind_ + i, temp.at(i));
		}
	}

	if (a.veh_ != b.veh_)
	{
		sol->getTrip(a.veh_)->altSize(n2 - n1);
		sol->getTrip(b.veh_)->altSize(n1 - n2);
	}
}

void Move::relocateClusterNodeLevel(NodeSolution*& sol, Pos& a, Pos& b, int& n1, int& n2)
{
	std::vector<Node*> temp;

	if (a.veh_ == b.veh_ && a.ind_ < b.ind_)
	{
		//insert cluster 1 after cluster 2 and erase cluster 1
		for (int i = 0; i < n1; i++)
		{
			sol->vTrips_.at(b.veh_)->vNodes_.insert(sol->vTrips_.at(b.veh_)->vNodes_.begin() + b.ind_ + n2, sol->getTrip(a.veh_)->getNode(a.ind_));
			sol->vTrips_.at(a.veh_)->vNodes_.erase(sol->vTrips_.at(a.veh_)->vNodes_.begin() + a.ind_);
		}
	}
	else if (a.veh_ == b.veh_)
	{
		//insert cluster 1 after cluster 2 and erase cluster 1
		for (int i = 0; i < n1; i++)
		{
			sol->vTrips_.at(b.veh_)->vNodes_.insert(sol->vTrips_.at(b.veh_)->vNodes_.begin() + b.ind_ + n2 + i, sol->getTrip(a.veh_)->getNode(a.ind_ + i));
			sol->vTrips_.at(a.veh_)->vNodes_.erase(sol->vTrips_.at(a.veh_)->vNodes_.begin() + a.ind_ + i + 1);
		}
	}
	else
	{
		//insert cluster 1 after cluster 2 and erase cluster 1
		for (int i = 0; i < n1; i++)
		{
			sol->vTrips_.at(b.veh_)->vNodes_.insert(sol->vTrips_.at(b.veh_)->vNodes_.begin() + b.ind_ + n2 + i, sol->getTrip(a.veh_)->getNode(a.ind_));
			sol->vTrips_.at(a.veh_)->vNodes_.erase(sol->vTrips_.at(a.veh_)->vNodes_.begin() + a.ind_);
		}
	}

	if (a.veh_ != b.veh_)
	{
		sol->getTrip(a.veh_)->altSize(-n1);
		sol->getTrip(b.veh_)->altSize(n1);
	}
}