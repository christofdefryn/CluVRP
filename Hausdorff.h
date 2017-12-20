#pragma once
#include <vector>
#include <algorithm>		//max_element

#include "Globals.h"

/*
	Class that is in charge of calculating the Hausdorff distances between clusters

	The Hausdorff distance is "the greatest of all the distances from a point in one set to the closest point in the other set".
*/

class Hausdorff
{
private:
public:
	Hausdorff();
	~Hausdorff();

	static double calcDistance(std::vector<Node>&, std::vector<Node>&, std::vector<std::vector<double>>&);
};

