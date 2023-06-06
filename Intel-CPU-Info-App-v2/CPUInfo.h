#ifndef INTEL_CPU_APP_V2_INFO_H
#define INTEL_CPU_APP_V2_INFO_H
#include <string>

using namespace std;

// Basic struct to house several CPU properties
struct CPUInfo {
	string name;
	string suffix;
	string family;
	string memSupport;
	string arch;
	string socket;
	string tier;
	int number;
	int generation;
	int lithography;
	bool hasIGPU;
	bool hasSMT;
};
#endif //INTEL_CPU_APP_V2_INFO_H