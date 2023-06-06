#ifndef INTEL_CPU_APP_V2_READER_CPP
#define INTEL_CPU_APP_V2_READER_CPP

#include "CPUReader.h"
#include "CPUInfo.h"
#include <iostream>
#include <set>
#include <string>

using namespace std;

CPUReader::CPUReader(set<string> cpuSet) {
	CPUSet = cpuSet;
}

void CPUReader::readCPU() {
	cin >> CPUName;
}

bool CPUReader::verifyCPU() {
	set<string>::iterator cpuPos = CPUSet.find(CPUName);
	return cpuPos != CPUSet.end();
}


#endif //INTEL_CPU_APP_V2_READER_CPP