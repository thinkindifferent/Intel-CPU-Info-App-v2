#ifndef INTEL_CPU_APP_V2
#define INTEL_CPU_APP_V2
#include <iostream>
#include <string>
#include <set>
#include <fstream>
#include <sstream>
#include "CPUReader.h"
#include "CPUAnalyzer.h"

using namespace std;

int main()
{
	// Read in CPU data
	cout << "Loading CPU Database..." << endl;
	CPUReader nonAnalyzedCPU(true);

	// Prompt user to enter a CPU name
	cout << "Enter an Intel Desktop CPU Name in the following format" << endl;
	cout << "\"i7-6700K\"" << endl;

	// TODO: Invalid CPUs will get through to the CPUAnalyzer object, fix it
	//
	// Read and verify the CPU
	nonAnalyzedCPU.readCPU();
	nonAnalyzedCPU.verifyCPU();
	cout << "Valid CPU: " << nonAnalyzedCPU.isGoodCPU() << endl;

	/*
		TESTING AREA! ONLY FOR DIAGNOSING INDIVIDUAL FUNCTIONS
	*/
	CPUAnalyzer analyzedCPU(nonAnalyzedCPU.getCPUName());

	analyzedCPU.extractInfo();
	analyzedCPU.printResults();

	return 0;
}


#endif //INTEL_CPU_APP_V2