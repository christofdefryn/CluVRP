#include "CluVRPsolver.h"

#include <time.h>

CluVRPsolver::CluVRPsolver()
{
	params_ = new Params();

	sCluCurrent_ = nullptr;
	sCluBest_ = nullptr;
	sNodCurrent_ = nullptr;
	sNodBest_ = nullptr;
}

CluVRPsolver::~CluVRPsolver()
{
	delete params_;
}

CluVRPsol* CluVRPsolver::solve(CluVRPinst* cluVRPinst, Timer* timer)
{
	if (!cluVRPinst->isFeasible()) {
		std::cout << "instance infeasible" << std::endl; return nullptr;
	}

	CluVRPsol* cluVRPsol = nullptr;

	//create methods
	BPheuristic_ = new BinPacking(cluVRPinst);
	cluVNS_ = new CluVNS(cluVRPinst);
	nodVNS_ = new NodVNS(cluVRPinst,false);
	diversOperator_ = new Diversification(cluVRPinst);

	//keep track of the best solutions
	sCluBest_ = nullptr;
	sNodBest_ = new NodeSolution(cluVRPinst, BIG_M);
	
	sCluCurrent_ = new ClusterSolution(cluVRPinst);
	
	//try to construct feasible solution at cluster level
	while (!BPheuristic_->run(sCluCurrent_)) 
	{
		delete sCluCurrent_; sCluCurrent_ = nullptr;
		cluVRPinst->increasenVeh();
		sCluCurrent_ = new ClusterSolution(cluVRPinst);
	}

	//start local search
	bool goToNodeVNS = false;
	bool stoppingCriterion = false;
	int nIterationsWithoutImprovement = 0;

	do
	{
		cluVNS_->run(sCluCurrent_);
		sNodCurrent_ = sCluCurrent_->convert();

		do
		{			
			nodVNS_->run(sNodCurrent_);

			//evaluate node solution
			if (sNodCurrent_->evaluate(sNodBest_))
			{
				timer->setTimeBest(timer->getIntervalTime());
				//keep also the cluster variant of the best node solution
				if (sCluBest_ != nullptr) delete sCluBest_;
				sCluBest_ = sNodBest_->convert();
				//reset counter
				nIterationsWithoutImprovement = 0;

				nodVNS_->disableAdditionalNBHs();
			}
			else
			{
				delete sNodCurrent_; sNodCurrent_ = nullptr;

				nIterationsWithoutImprovement++;
				
				if (nIterationsWithoutImprovement % 100 == 0)	//every 100th iteration
				//if (nIterationsWithoutImprovement + 1 == params_->N_NO_IMPROVEMENT)	//the last iteration
				{
					//use all the power we have to improve the current best solution
					sNodCurrent_ = new NodeSolution(sNodBest_);
					nodVNS_->enableAdditionalNBHs();	//try complex neighbourhoods
					goToNodeVNS = true;
					continue;
				}				
				else if (nIterationsWithoutImprovement >= params_->N_NO_IMPROVEMENT) //check stopping criterion
				{
					stoppingCriterion = true;
					break;
				}
				else
					nodVNS_->disableAdditionalNBHs();
			}
			//diversification before new round of improvement
			goToNodeVNS = diversOperator_->run(sCluCurrent_);
			if(goToNodeVNS) sNodCurrent_ = sCluCurrent_->convert();

		} while (goToNodeVNS);

	//} while (timer->getIntervalTime() < CALC_TIME);
	} while (!stoppingCriterion);

	//free memory
	delete BPheuristic_;
	delete cluVNS_;
	delete nodVNS_;
	delete diversOperator_;
	delete sCluCurrent_;

	//create solution object
	cluVRPsol = new CluVRPsol(sCluBest_, sNodBest_);
	return cluVRPsol;	
}