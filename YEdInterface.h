#pragma once
#include "CluVRPinst.h"
#include "CluVRPsol.h"
#include "Trip.h"

#include <fstream>
#include <string>

class YEdInterface
{
private:

	std::ofstream YEdFile_;

	void preamble(void);
	void addCluster(Cluster* c);
	void drawNodes(CluVRPinst*&);
	void drawSolution(NodeSolution*&, int&);
	void closeFile(void);

public:
	YEdInterface();
	~YEdInterface();

	void generateYEdInstance(CluVRPinst*&, CluVRPsol*&, std::string&);
};

