#ifndef INTEL_CPU_APP_V2_ANALYZER_CPP
#define INTEL_CPU_APP_V2_ANALYZER_CPP
#include "CPUAnalyzer.h"
#include "CPUInfo.h"

CPUAnalyzer::CPUAnalyzer(CPUInfo cpu) {
	currCPU = cpu;
}

CPUAnalyzer::CPUAnalyzer() {
	CPUInfo cpu;
	cpu.name = "i7-6700K";
	cpu.arch = "Skylake";

}
#endif //INTEL_CPU_APP_V2_ANALYZER_CPP