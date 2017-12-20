#include "Trip.h"

Trip::Trip(CluVRPinst* cluVRPinst)
{
	this->cluVRPinst_ = cluVRPinst;
	this->dist_ = 0;
	this->totalDemand_ = 0;
	this->size_ = 0;
}

Trip::~Trip(){}

CluTrip::CluTrip(CluVRPinst* cluVRPinst) 
	: Trip(cluVRPinst){}

CluTrip::~CluTrip(){}

double CluTrip::calcDistance(void)
{
	dist_ = 0;
	
	for (int i = 0; i < size_ - 1; i++)
	{
		dist_ += cluVRPinst_->getDistClusters(vClusters_.at(i), vClusters_.at(i + 1));
	}

	return dist_;
}

void CluTrip::addStop(Cluster* c)
{
	this->vClusters_.push_back(c);
	this->size_++;
	this->totalDemand_ += c->getDemand();
	
	if (size_ > 1)
	{
		this->dist_ += cluVRPinst_->getDistClusters(vClusters_.at(size_ - 2), vClusters_.at(size_ - 1));
	}
}

void CluTrip::removeStop(int it)
{
	this->totalDemand_ -= vClusters_.at(it)->getDemand();
	this->size_--;
	this->vClusters_.erase(vClusters_.begin() + it);

	//no distance update!
}

void CluTrip::returnToDepot(void)
{
	vClusters_.push_back(vClusters_.front());
	this->size_++;
	this->dist_ += cluVRPinst_->getDistClusters(vClusters_.at(size_ - 2), vClusters_.at(size_ - 1));
}

//////////////////////////////////////////////////////////////////////

NodTrip::NodTrip(NodTrip* nodTrip) : Trip(nodTrip->cluVRPinst_)
{
	this->dist_ = nodTrip->dist_;
	this->totalDemand_ = nodTrip->totalDemand_;
	this->size_ = nodTrip->size_;
	this->vNodes_ = nodTrip->vNodes_;
}

NodTrip::NodTrip(CluVRPinst* cluVRPinst) 
	: Trip(cluVRPinst){}

NodTrip::~NodTrip(){}

double NodTrip::calcDistance(void)
{
	dist_ = 0;

	for (int i = 0; i < size_ - 1; i++)
	{
		dist_ += cluVRPinst_->getDistNodes(vNodes_.at(i), vNodes_.at(i + 1));
	}

	return dist_;
}

void NodTrip::addStop(Node* n)
{
	this->vNodes_.push_back(n);
	this->size_++;
	if (size_ > 1)
	{
		this->dist_ += cluVRPinst_->getDistNodes(vNodes_.at(size_ - 2), vNodes_.at(size_ - 1));
	}
}
