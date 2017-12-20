#include "Hausdorff.h"

using namespace std;

Hausdorff::Hausdorff(){}

Hausdorff::~Hausdorff(){}

double Hausdorff::calcDistance(std::vector<Node>& list1, std::vector<Node>& list2, std::vector<std::vector<double>>& distNodes)
{
	unsigned int size1 = list1.size();
	unsigned int size2 = list2.size();

	vector<double> closestDistances;

	for (unsigned int i = 0; i < size1; i++)
	{
		double minDist = BIG_M;

		for (unsigned int j = 0; j < size2; j++)
		{
			double dist = distNodes.at(list1.at(i).id - 1).at(list2.at(j).id - 1);
			
			if (dist < minDist)
			{
				minDist = dist;
			}
		}

		closestDistances.push_back(minDist);
	}

	for (unsigned int i = 0; i < size2; i++)
	{
		double minDist = BIG_M;

		for (unsigned int j = 0; j < size1; j++)
		{
			double dist = distNodes.at(list1.at(j).id - 1).at(list2.at(i).id - 1);

			if (dist < minDist)
			{
				minDist = dist;
			}
		}

		closestDistances.push_back(minDist);
	}
	
	return *min_element(closestDistances.begin(), closestDistances.end());
	//return *max_element(closestDistances.begin(), closestDistances.end());
}