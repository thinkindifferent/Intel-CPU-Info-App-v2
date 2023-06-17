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
	analyzedCPU.findSuffix();
	analyzedCPU.findFamily();
	analyzedCPU.findTier();
	analyzedCPU.findNumber();
	analyzedCPU.findGeneration();
	analyzedCPU.findLithogrpahy();
	analyzedCPU.findMemSupport();
	analyzedCPU.findArch();
	analyzedCPU.findSocket();
	analyzedCPU.findHasIGPU();
	analyzedCPU.findHasSMT();
	analyzedCPU.findCores();
	analyzedCPU.findHasPECores();

	cout << "CPU Suffix: " << analyzedCPU.getSuffix() << endl;
	cout << "CPU Family: " << analyzedCPU.getFamily() << endl;
	cout << "CPU Tier: " << analyzedCPU.getTier() << endl;
	cout << "CPU Number: " << analyzedCPU.getNumber() << endl;
	cout << "CPU Generation: " << analyzedCPU.getGeneration() << endl;
	cout << "CPU Lithography: " << analyzedCPU.getLithography() << "nm" << endl;
	cout << "CPU Supported Memory: " << analyzedCPU.getMemSupport() << endl;
	cout << "CPU Architecture: " << analyzedCPU.getArch() << endl;
	cout << "CPU Socket: " << analyzedCPU.getSocket() << endl;
	cout << "Has iGPU? " << analyzedCPU.getHasIGPU() << endl;
	cout << "Has hyperthreading? " << analyzedCPU.getHasSMT() << endl;
	cout << "CPU P/E Cores: " << analyzedCPU.getCores()[0] << " P Cores, " << analyzedCPU.getCores()[1] << " E Cores" << endl;
	cout << "Has P/E Cores? " << analyzedCPU.getHasPECores() << endl;

	return 0;
}


#endif //INTEL_CPU_APP_V2