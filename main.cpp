#include "FileHandler.h"
#include "CluVRPsolver.h"
#include "CluVRPinst.h"
#include "CluVRPsol.h"
#include "Reader.h"
#include "Cluster.h"
#include "Timer.h"
#include "Printer.h"

int main(void)
{
	srand((unsigned)time(NULL));

	CluVRPsolver* cluVRPsolver = new CluVRPsolver();
	CluVRPinst* cluVRPinst = nullptr;
	CluVRPsol* cluVRPsol = nullptr;
	Timer* timer = new Timer();
	Printer* print = new Printer();
	
	//select the overview file and put all necessary files in place
	FileHandler* fileHandler = new FileHandler();
	//open reader for the instance set selected
	Reader* reader = new Reader(fileHandler->getSetSelection());
	
	do {
		cluVRPinst = reader->read(fileHandler->getFilePathInstance());

		for (int i = 0; i < Params::N_RUNS; i++)
		{
			timer->startClock();
			cluVRPsol = cluVRPsolver->solve(cluVRPinst, timer);
			timer->stopClock();
			
			if (cluVRPsol == nullptr) continue;
	
			Printer::addToSolutionOverview(cluVRPsol, timer, fileHandler);
			Printer::nodeSolution(cluVRPsol, fileHandler);	
			Printer::cluster(cluVRPsol, timer);
			delete cluVRPsol;
		}

		delete cluVRPinst;

	} while (true); //for every instance

	delete fileHandler;
	delete reader;	
	delete cluVRPsolver;
	delete timer;
	delete print;	

	return 0;
}