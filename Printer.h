#pragma once
#include <iostream>
#include "FileHandler.h"
#include "Timer.h"
#include "CluVRPsol.h"

class Printer
{
public:
	Printer();
	~Printer();

	//static void notFeasible(void);
	//static void instanceName(void); 
	//static void time(Timer*&);
	static void addToSolutionOverview(CluVRPsol*&, Timer*&, FileHandler*&);
	//static void track(NodeSolution*&, NodeSolution*&, Timer*&, FileHandler*&, bool&);
	//static void allCurrentParams(void);
	static void cluster(CluVRPsol*&, Timer*&);
	static void nodeSolution(CluVRPsol*&, FileHandler*&);
	static void nodeSolution(NodeSolution*&);
};

