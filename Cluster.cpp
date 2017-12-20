#include "Cluster.h"

using namespace std;

Cluster::Cluster(int id)
{
	this->id_ = id;
	this->demand_ = 0;
	this->nNodes_ = 0;
	this->demand_ = 0;

	isDepot = false;
}

Cluster::~Cluster(){}

bool Cluster::sortByDemandOperator(Cluster*& a, Cluster*& b)
{
	return a->demand_ > b->demand_;
}

void Cluster::addNode(Node n)
{
	vNodes_.push_back(n);
	nNodes_++;
}