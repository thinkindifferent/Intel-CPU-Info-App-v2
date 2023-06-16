#ifndef INTEL_CPU_APP_V2_ANALYZER_CPP
#define INTEL_CPU_APP_V2_ANALYZER_CPP
#include "CPUAnalyzer.h"
#include "CPUInfo.h"

//CPUAnalyzer::CPUAnalyzer() {
//	CPUInfo* cpu = new CPUInfo;
//	cpu->name = "i7-6700K";
//	cpu->suffix = "K";
//	cpu->family = "i7";
//	cpu->memSupport = "DDR4";
//	cpu->arch = "Skylake";
//	cpu->socket = "LGA1151";
//	cpu->tier = "Performance";
//	cpu->number = 6700;
//	cpu->generation = 6;
//	cpu->lithography = 14;
//	cpu->hasIGPU = true;
//	cpu->hasSMT = true;
//	currCPU = cpu;
//}

CPUAnalyzer::CPUAnalyzer(const CPUInfo& cpu) {
	CPUInfo newCPU = cpu;
	currCPU = &newCPU;
}

CPUAnalyzer::CPUAnalyzer(string name) {
	CPUInfo cpu;
	cpu.name = name;
}

void CPUAnalyzer::findSuffix() {
	char lastChar, secLastChar;
	bool lastCharIsDig, secLastCharIsDig;
	lastChar = currCPU->name.back();
	secLastChar = currCPU->name[currCPU->name.length() - 2];

	lastCharIsDig = isdigit(lastChar);
	secLastCharIsDig = isdigit(secLastChar);

	// Case where there is no suffix
	if (lastCharIsDig && secLastCharIsDig) {
		currCPU->suffix = "N/A";
	}
	// Case where there is a suffix of only 1 character
	else if (!lastCharIsDig && secLastCharIsDig) {
		currCPU->suffix = string(1, lastChar);
	}
	// Case where there is a suffix of 2 characters
	// Source: https://stackoverflow.com/questions/51017979/joining-two-characters-in-c
	else {
		string temp;
		temp += secLastChar;
		temp += lastChar;
		currCPU->suffix = temp;
	}
}
#endif //INTEL_CPU_APP_V2_ANALYZER_CPP