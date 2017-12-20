#pragma once
#include <string>
#include <fstream>		//ofstream
#include <iostream>		//cout
#include <sstream>		//ostringstream
#include <direct.h>		//cwd

#include "Globals.h"

class FileHandler
{
private:

	int setSelection_;	//the instance set, selected by the user

	static std::string currentInstanceName_;

	std::string filePathOverview_;
	std::string filePathInstance_;
	FILE* fpo_;	//overview file

	void createFilePathNextInstance(void);

	void createFilePathSolutionOverview(void);
	void createTrack(void);

	void initOverview(void);
	void initOutputFiles(void);
public:

	//SOLUTION OVERVIEW FILE
	std::string filePathSolutionOverview_;
	std::ofstream txtSolOverview_;

	//TRACKING FILE
	std::string filePathTracker_;
	std::ofstream txtTrack_;

	FileHandler();
	~FileHandler();

	std::string getFilePathInstance(void);
	static std::string getCurrentInstanceName(void);
	int getSetSelection(void) const;
};

