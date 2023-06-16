#ifndef INTEL_CPU_APP_V2_READER_CPP
#define INTEL_CPU_APP_V2_READER_CPP

#include "CPUReader.h"
#include "CPUInfo.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <string>

using namespace std;

CPUReader::CPUReader() {
	CPUName = "";
	goodCPU = false;
}

CPUReader::CPUReader(bool createList) {
	if (createList) {
		loadCPUList();
	}
	CPUName = "";
	goodCPU = false;
}

CPUReader::~CPUReader() {
	CPUSet.clear();
}

void CPUReader::loadCPUList() {
	// Source: https://java2blog.com/read-csv-file-in-cpp/

	string temp, line, currName;
	// Open the CPU list CSV
	fstream cpuList;
	cpuList.open("C:/Users/willi/OneDrive - UW/Third Year/Intel CPU Info App v2/CompatibleCPUs.csv", ios::in);

	if (cpuList.is_open()) {
		while (getline(cpuList, line)) {

			stringstream s(line);

			while (getline(s, currName, ',')) {
				CPUSet.insert(currName);
			}
		}
	}
	else {
		cout << "\"CompatibleCPUs.csv\" not found" << endl;
	}
}

void CPUReader::readCPU() {
	cin >> CPUName;
}

// Verifies if a CPU is valid (i.e. exists within CompatibleCPUs.csv)
void CPUReader::verifyCPU() {
	set<string>::iterator cpuPos = CPUSet.find(CPUName);
	goodCPU = (cpuPos != CPUSet.end());
}

bool CPUReader::isGoodCPU() {
	return goodCPU;
}

string CPUReader::getCPUName() {
	return CPUName;
}

#endif //INTEL_CPU_APP_V2_READER_CPP