#include "Reader.h"

using namespace std;

std::vector<std::vector<double>> Reader::calcNodeDistances(std::vector<Node>& vNodes)
{
	std::vector<std::vector<double>> m;
	unsigned int nNodes = vNodes.size();

	//resize the vector
	m.resize(nNodes);
	for (unsigned int i = 0; i < nNodes; i++) m.at(i).resize(nNodes);

	//calcalute for all pairs
	for (unsigned int i = 0; i < nNodes - 1; i++)
	{
		for (unsigned int j = i + 1; j < nNodes; j++)
		{
			if (setSelection_ == 0 || setSelection_ == 1 || setSelection_ == 2)
			{
				m.at(i).at(j) = m.at(j).at(i) = \
					(int) round(sqrt((double)(vNodes.at(i).x - vNodes.at(j).x)*(vNodes.at(i).x - vNodes.at(j).x) \
						+ (vNodes.at(i).y - vNodes.at(j).y)*(vNodes.at(i).y - vNodes.at(j).y)));
			}
			else
			{
				m.at(i).at(j) = m.at(j).at(i) = \
					sqrt((double)(vNodes.at(i).x - vNodes.at(j).x)*(vNodes.at(i).x - vNodes.at(j).x) \
						+ (vNodes.at(i).y - vNodes.at(j).y)*(vNodes.at(i).y - vNodes.at(j).y));
			}
		}
		m.at(i).at(i) = BIG_M;
	}

	return m;
}

std::vector<std::vector<double>> Reader::calcClusterDistances(std::vector<Cluster>& vClusters, std::vector<std::vector<double>>& distNodes)
{
	std::vector<std::vector<double>> m;
	unsigned int nClusters = vClusters.size();

	//resize the vector
	m.resize(nClusters);
	for (unsigned int i = 0; i < nClusters; i++) m.at(i).resize(nClusters);

	//calculate for all pairs
	for (unsigned int i = 0; i < nClusters; i++)
	{
		for (unsigned int j = i; j < nClusters; j++)
		{
			if (i == j) m.at(i).at(j) = BIG_M;
			else
			{
				m.at(i).at(j) = m.at(j).at(i) = Hausdorff::calcDistance(vClusters.at(i).getvNodes(), vClusters.at(j).getvNodes(), distNodes);
			}
		}
	}

	return m;
}

Reader::Reader(int setSelection)
{
	this->setSelection_ = setSelection;
}

Reader::~Reader(){}

CluVRPinst* Reader::read(std::string filePathInstance)
{
	CluVRPinst* cluVRPinst = nullptr;
	FILE* f_ptr;

	fopen_s(&f_ptr, filePathInstance.c_str(), "r");

	//read header
	int dimension;
	int nVehicles;
	int nClusters;

	fscanf_s(f_ptr, "%*s %*s %*s %*s %*s %*s %*s");
	if (setSelection_ == 0 || setSelection_ == 3 || setSelection_ == 4 || setSelection_ == 5 || setSelection_ == 6 || setSelection_ == 7) fscanf_s(f_ptr, "%*s");			//only for the CluVRP2 set

	if (setSelection_ == 3 || setSelection_ == 4 || setSelection_ == 5 || setSelection_ == 6 || setSelection_ == 7) fscanf_s(f_ptr, "%d %*s %*s %d %*s %*s %d %*s", &dimension, &nClusters, &Globals::vehicleCapacity);
	else fscanf_s(f_ptr, "%d %*s %*s %d %*s %*s %d %*s %*s %d %*s", &dimension, &nVehicles, &nClusters, &Globals::vehicleCapacity);

	if (setSelection_ == 0 || setSelection_ == 3 || setSelection_ == 4 || setSelection_ == 5 || setSelection_ == 6 || setSelection_ == 7) fscanf_s(f_ptr, "%*s %*s %*s");	//not for the largegolden set
	if (setSelection_ == 3 || setSelection_ == 4 || setSelection_ == 5 || setSelection_ == 6 || setSelection_ == 7) fscanf_s(f_ptr, "%*s %*s");

	//read individual nodes
	int id;
	double x, y;
	vector<Node> vNodes;

	for (int i = 0; i < dimension; i++)
	{
		fscanf_s(f_ptr, "%d %lf %lf", &id, &x, &y);
		vNodes.push_back(Node(id, x, y));
	}

	fscanf_s(f_ptr, "%*s");

	//read cluster composition
	vector<Cluster> vClusters;

	if (setSelection_ == 3 || setSelection_ == 4 || setSelection_ == 5 || setSelection_ == 6 || setSelection_ == 7)
	{
		//read demand for the individual customers
		std::vector<int> vDemands;

		int demand;
		int sumDemand = 0;

		for (int i = 0; i < dimension; i++)
		{
			fscanf_s(f_ptr, "%*d %d", &demand);
			vDemands.push_back(demand);
			sumDemand += demand;
		}

		fscanf_s(f_ptr, "%*s"); //skip first cluster number

		//create all customer clusters
		for (int i = 0; i < nClusters; i++)
		{
			Cluster c(i);
			vClusters.push_back(c);
		}

		int clusterid;
		for (int i = 0; i < dimension; i++)
		{
			fscanf_s(f_ptr, "%d %d", &id, &clusterid);

			vNodes.at(id - 1).cluster = clusterid - 1;
			vClusters.at(clusterid - 1).addNode(vNodes.at(id - 1));
			vClusters.at(clusterid - 1).addDemand(vDemands.at(id - 1));
		}

		//set depot node / cluster
		fscanf_s(f_ptr, "%*s");

		fscanf_s(f_ptr, "%d", &id);
		vClusters.at(vNodes.at(id - 1).cluster).isDepot = true;

		nVehicles = (int)ceil((double)sumDemand / (double)Globals::vehicleCapacity);
	}
	else //(setSelection_ = 0,1,2)
	{
		//add dummy depot cluster
		Cluster c(0);
		vNodes.front().cluster = 0;
		c.addNode(vNodes.front());
		c.isDepot = true;
		vClusters.push_back(c);

		//add client clusters
		for (int i = 0; i < nClusters; i++)
		{
			fscanf_s(f_ptr, "%d", &id);
			Cluster c(id);

			fscanf_s(f_ptr, "%d", &id);	//add all client from that belong to this cluster

			while (id != -1)
			{
				vNodes.at(id - 1).cluster = c.getId();		//put cluster id in this node
				c.addNode(vNodes.at(id - 1));
				fscanf_s(f_ptr, "%d", &id);
			}

			vClusters.push_back(c);
		}

		fscanf_s(f_ptr, "%*s");

		//read cluster demands

		int demand;

		for (int i = 0; i < nClusters; i++)
		{
			fscanf_s(f_ptr, "%d %d", &id, &demand);
			vClusters.at(id).setDemand(demand);
		}
	}

	fclose(f_ptr);

	cluVRPinst = new CluVRPinst(vClusters, nVehicles);
	cluVRPinst->setDistNodes(calcNodeDistances(vNodes));
	cluVRPinst->setDistClusters(calcClusterDistances(vClusters, cluVRPinst->getDistNodesMatrix()));
	//cluVRPinst->setSpan(span_);
	//cluVRPinst->calculateIntraClusterTSP();

	return cluVRPinst;
}