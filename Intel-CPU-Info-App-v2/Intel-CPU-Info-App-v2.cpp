#ifndef INTEL_CPU_APP_V2
#define INTEL_CPU_APP_V2
#include <iostream>
#include <string>
#include <set>
#include <fstream>
#include <sstream>
#include "CPUReader.h"

using namespace std;

set<string> CPUSet;

void loadCPUList() {
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

int main()
{
	// Read in CPU data
	cout << "Loading CPU Database..." << endl;
	loadCPUList();
	CPUReader cpu(CPUSet);

	cout << "Enter an Intel Desktop CPU Name in the following format" << endl;
	cout << "\"Intel Core i7-6700K\"" << endl;
	cpu.readCPU();
	cout << "Valid CPU: " << cpu.verifyCPU();
	

	//for (set<string>::iterator itr = CPUSet.begin(); itr != CPUSet.end(); itr++) {
	//	cout << *itr << endl;
	//}

	CPUSet.clear();
	return 0;
}


#endif //INTEL_CPU_APP_V2