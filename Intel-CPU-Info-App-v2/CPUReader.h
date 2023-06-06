#ifndef INTEL_CPU_APP_V2_READER_H
#define INTEL_CPU_APP_V2_READER_H
#include <string>
#include <iostream>
#include <set>
#include "CPUInfo.h"
using namespace std;

class CPUReader {
private:
	string CPUName;
	set<string> CPUSet;

public:
	CPUReader(set<string> cpuSet);

	// Read in a CPU from the istream
	void readCPU();

	// Determines if the CPU is compatible with the app's algorithms
	bool verifyCPU();

};
#endif //INTEL_CPU_APP_V2_READER_H