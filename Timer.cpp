#include "Timer.h"


Timer::Timer()
{
	this->elapsedTime_ = 0;
	this->timeBest_ = 0;
}

Timer::~Timer(){}

double Timer::getElapsedTime(void) const
{
	return this->elapsedTime_;
}

double Timer::getIntervalTime(void) const
{
	return (float)(clock() - startTime_) / CLK_TCK;
}

double Timer::getTimeBest(void) const
{
	return timeBest_;
}

void Timer::startClock(void)
{
	startTime_ = clock();
}

void Timer::stopClock(void)
{
	elapsedTime_ = (float)(clock() - startTime_) / CLK_TCK;
}

void Timer::setTimeBest(double timeBest)
{
	timeBest_ = timeBest;
}