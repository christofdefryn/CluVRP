#pragma once
#include "Globals.h"
#include "Solution.h"

class Move
{
private:

public:
	Move();
	~Move();

	/* CLUSTER LEVEL */
	static void swap(ClusterSolution*&, Pos&, Pos&);
	static void relocate(ClusterSolution*&, Pos&, Pos&);
	static void twoOpt(ClusterSolution*&, Pos&, Pos&);
	static void orOpt(ClusterSolution*&, Pos&, Pos&, int&);

	/* NODE LEVEL */
	static void swap(NodeSolution*&, Pos&, Pos&);
	static void relocate(NodeSolution*&, Pos&, Pos&);
	static void twoOpt(NodeSolution*&, Pos&, Pos&);
	static void orOpt(NodeSolution*&, Pos&, Pos&, int&);

	static void swapClusterNodeLevel(NodeSolution*&, Pos&, Pos&, int&, int&);
	static void relocateClusterNodeLevel(NodeSolution*&, Pos&, Pos&, int&, int&);
};

