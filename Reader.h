#pragma once
#include <string>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <direct.h>

#include "Globals.h"
#include "CluVRPInst.h"
#include "Hausdorff.h"


class Reader
{
private:
	int setSelection_;

	std::vector<std::vector<double>> calcNodeDistances(std::vector<Node>&);
	std::vector<std::vector<double>> calcClusterDistances(std::vector<Cluster>&, std::vector<std::vector<double>>&);

public:
	Reader(int);
	~Reader();

	CluVRPinst* read(std::string);
};
