#pragma once
#include <time.h>

class Timer
{
private:

	clock_t startTime_;
	double elapsedTime_;
	double timeBest_;

public:
	Timer();
	~Timer();

	double getElapsedTime(void) const;
	double getIntervalTime(void) const;
	double getTimeBest(void) const;

	void startClock();
	void stopClock();
	void setTimeBest(double);
};

