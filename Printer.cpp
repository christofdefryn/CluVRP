#include "Printer.h"

using namespace std;

Printer::Printer(){}

Printer::~Printer(){}

//void Printer::notFeasible(void)
//{
//	cout << endl << "The current instance (" << FileHandler::getCurrentInstanceName() << ") is not FEASIBLE";
//}

//void Printer::instanceName(void)
//{
//	cout << endl << "The current instance = " << FileHandler::getCurrentInstanceName() << endl;
//}

//void Printer::time(Timer*& time)
//{
//	cout << "Calculation Time = " << time->getElapsedTime() << endl;
//} 

void Printer::addToSolutionOverview(CluVRPsol*& best, Timer*& time, FileHandler*& fileHandler)
{
	if (Params::TUNING)
	{
		fileHandler->txtSolOverview_ << fileHandler->getCurrentInstanceName() << "	" << \
			Params::HARD_CLUSTER << "	" << Params::N_NO_IMPROVEMENT << "	" << Params::RANDOM_CONSTRUCTION << "	" << Params::PERT_RATE << "	" << \
			Params::DIVERSIFICATION_1 << "	" << best->sNode_->getTotalDist() << "	" << time->getTimeBest() << endl;
	}
	else
	{
		fileHandler->txtSolOverview_ << fileHandler->getCurrentInstanceName() << "	" << best->sNode_->getTotalDist() << "	" << time->getTimeBest() << endl;
	}
}

//void Printer::track(NodeSolution*& sol, NodeSolution*& best, Timer*& time, FileHandler*& fileHandler, bool& opt)
//{
//	if(!Params::TUNING) fileHandler->txtTrack_ << fileHandler->getCurrentInstanceName() << "	" << time->getElapsedTime() << "	" << best->getTotalDist() << "	" << sol->getTotalDist() << "	" << opt << endl;
//}

//void Printer::allCurrentParams(void)
//{
//	cout << Params::HARD_CLUSTER << "	" << Params::N_NO_IMPROVEMENT << "	" << Params::RANDOM_CONSTRUCTION << "	" << Params::PERT_RATE << "	" << Params::DIVERSIFICATION_1 << "	"  << Params::N_RUNS << endl;
//}

void Printer::cluster(CluVRPsol*& best, Timer*& time)
{
	if (Params::TUNING)
	{
		cout << FileHandler::getCurrentInstanceName() << "	" << Params::HARD_CLUSTER << "	" << Params::N_NO_IMPROVEMENT\
			<< "	" << Params::RANDOM_CONSTRUCTION << "	" << Params::RANDOM_CONVERSION << "	" << Params::PERT_RATE\
			<< "	" << Params::DIVERSIFICATION_1 << "	" << Params::N_RUNS << "	" << best->sNode_->getTotalDist()\
			<< "	" << time->getTimeBest() << endl;
	}
	else
	{
		cout << FileHandler::getCurrentInstanceName() << "	" << best->sNode_->getTotalDist() << "	" << time->getTimeBest() << endl;
	}
}

void Printer::nodeSolution(CluVRPsol*& best, FileHandler*& fileHandler)
{
	for (int i = 0; i < best->sNode_->getnTrips(); i++)
	{
		fileHandler->txtSolOverview_ << "Trip " << i << " (DEM=" << best->sNode_->getTrip(i)->getTotalDemand() <<")(DIST="<< best->sNode_->getTrip(i)->getDistance() <<") = ";

		for (int j = 0; j < best->sNode_->getTrip(i)->getSize(); j++)
		{
			fileHandler->txtSolOverview_ << " " << best->sNode_->getTrip(i)->getNode(j)->id;
		}
		
		fileHandler->txtSolOverview_ << endl;
	}
	fileHandler->txtSolOverview_ << "///////////////////////////////////////////////////////" << endl;
}

void Printer::nodeSolution(NodeSolution*& s)
{
	std::cout << endl;
	for (int i = 0; i < s->getnTrips(); i++)
	{
		std::cout << "Trip " << i << " (DEM=" << s->getTrip(i)->getTotalDemand() << ")(DIST=" << s->getTrip(i)->getDistance() << ") = ";

		for (int j = 0; j < s->getTrip(i)->getSize(); j++)
		{
			std::cout << " " << s->getTrip(i)->getNode(j)->id;
		}

		std::cout << endl;
	}
	std::cout << endl;
}