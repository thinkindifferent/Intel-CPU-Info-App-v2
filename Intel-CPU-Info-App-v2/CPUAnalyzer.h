#ifndef INTEL_CPU_APP_V2_ANALYZER_H
#define INTEL_CPU_APP_V2_ANALYZER_H

#include <iostream>
#include <string>
#include "CPUInfo.h"

using namespace std;

class CPUAnalyzer {
private:
	CPUInfo currCPU;
public:
	CPUAnalyzer(CPUInfo cpu);

	// Utilizes the below functions to extract CPU info
	void extractInfo(CPUInfo cpu);

	// Finds CPU Name (e.g. "i7-6700K")
	void findName();

	// Finds any applicable suffixes (e.g. "KS" for unlocked special edition)
	void findSuffix();

	// Finds the CPU family (e.g. i3, i5...)
	void findFamily();

	// Finds the type of RAM supported
	void findMemSupport();

	// Finds the CPU architecture codename
	void findArch();

	// Finds the socket the CPU uses
	void findSocket();

	// Determines the performance tier or target audience
	void findTier();

	// Extracts the numeric values inside the CPU name
	void findNumber();

	// Determines generation (2nd to 13th)
	void findGeneration();

	// Finds the lithography of the CPU
	void findLithogrpahy();

	// Determines whether or not the CPU has an iGPU
	void findHasIGPU();

	// Determines whether or not the CPU supports hyperthreading
	void findHasSMT();
};
#endif //INTEL_CPU_APP_V2_ANALYZER_H