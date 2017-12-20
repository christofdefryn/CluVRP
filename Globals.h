#pragma once
#include <vector>
#include <iostream>

////* DEFAULT PARAMETER SETTINGS *////
#define HARD_CLUSTER_DEFAULT		true
#define N_RUNS_DEFAULT				20
#define N_NO_IMPROVEMENT_DEFAULT	2000
#define RANDOM_CONSTRUCTION_DEFAULT	0
#define RANDOM_CONVERSION_DEFAULT	.4
#define PERT_RATE_DEFAULT			.1
#define DIVERSIFICATION_1_DEFAULT	1
#define CALC_TIME					60
//////////////////////////////////////

#define BIG_M						30000

struct Params
{
	static bool TUNING;
	static bool HARD_CLUSTER;				//if FALSE => soft cluster constraint
	static int N_RUNS;
	static int N_NO_IMPROVEMENT;
	static double RANDOM_CONSTRUCTION;
	static double RANDOM_CONVERSION;
	static double PERT_RATE;
	static double DIVERSIFICATION_1;

	inline Params()
	{
		if (__argc < 3)
		{
			HARD_CLUSTER = HARD_CLUSTER_DEFAULT;
			N_RUNS = N_RUNS_DEFAULT;
			N_NO_IMPROVEMENT = N_NO_IMPROVEMENT_DEFAULT;
			RANDOM_CONSTRUCTION = RANDOM_CONSTRUCTION_DEFAULT;
			RANDOM_CONVERSION = RANDOM_CONVERSION_DEFAULT;
			PERT_RATE = PERT_RATE_DEFAULT;
			DIVERSIFICATION_1 = DIVERSIFICATION_1_DEFAULT;
			TUNING = false;
		}
		else
		{
			TUNING = false;

			int temp;
			if (sscanf_s(__argv[2], "%d", &temp) != 1) HARD_CLUSTER = HARD_CLUSTER_DEFAULT;
			if (temp == 0) HARD_CLUSTER = false;
			else HARD_CLUSTER = true;

			if (sscanf_s(__argv[3], "%d", &N_NO_IMPROVEMENT) != 1) N_NO_IMPROVEMENT = N_NO_IMPROVEMENT_DEFAULT;

			RANDOM_CONSTRUCTION = (double)atof(__argv[4]) / 100;
			RANDOM_CONVERSION = (double)atof(__argv[5]) / 100;
			PERT_RATE = (double) atof(__argv[6]) / 100;
			DIVERSIFICATION_1 = (double) atof(__argv[7]) / 100;

			if (sscanf_s(__argv[8], "%d", &N_RUNS) != 1) N_RUNS = N_RUNS_DEFAULT;
		}
	}
};

struct Globals
{
	static int vehicleCapacity;
};

struct Node
{
	int id;
	int cluster;
	double x;
	double y;

	inline Node(int& id, double& x, double& y)
	{
		this->id = id;
		this->x = x;
		this->y = y;
		this->cluster = -1;
	}
};

struct Pos				//position -> given to MOVE class
{
	int veh_;
	int ind_;

	inline Pos(){}
	inline Pos(int veh, int ind)
	{
		this->veh_ = veh;
		this->ind_ = ind;
	}
	inline bool operator ==(Pos& a)
	{
		return (a.veh_ == this->veh_) && (a.ind_ == this->ind_);
	}
	inline static bool equalVehicle(Pos& a, Pos& b)
	{
		return a.veh_ == b.veh_;
	}
	inline static bool consecutive(Pos& a, Pos& b)
	{
		if (equalVehicle(a, b)) return false;
		if (a.ind_ - b.ind_ == 1 || b.ind_ - a.ind_ == 1) return true;
		else return false;
	}
};

struct Cost
{
	int route;
	int toPay;
	int toReceive;

	inline Cost(int a, int b, int c)
	{
		route = a;
		toPay = b;
		toReceive = c;
	}
	inline int getAllocatedCost(void) const
	{
		return route + toPay - toReceive;
	}
};