#ifndef INTEL_CPU_APP_V2_ANALYZER_CPP
#define INTEL_CPU_APP_V2_ANALYZER_CPP
#include "CPUAnalyzer.h"
#include "CPUInfo.h"

CPUAnalyzer::CPUAnalyzer(const CPUAnalyzer& oldCPU) {
	this->cpu = oldCPU.cpu;
}

CPUAnalyzer::CPUAnalyzer(string name) {
	cpu->name = name;
}

void CPUAnalyzer::findSuffix() {
	char lastChar, secLastChar;
	bool lastCharIsDig, secLastCharIsDig;
	lastChar = cpu->name.back();
	secLastChar = cpu->name[cpu->name.length() - 2];

	lastCharIsDig = isdigit(lastChar);
	secLastCharIsDig = isdigit(secLastChar);

	// Case where there is no suffix
	if (lastCharIsDig && secLastCharIsDig) {
		cpu->suffix = "N/A";
		suffixSize = 0;
	}
	// Case where there is a suffix of only 1 character
	else if (!lastCharIsDig && secLastCharIsDig) {
		cpu->suffix = string(1, lastChar);
		suffixSize = 1;
	}
	// Case where there is a suffix of 2 characters
	// Source: https://stackoverflow.com/questions/51017979/joining-two-characters-in-c
	else {
		string temp;
		temp += secLastChar;
		temp += lastChar;
		cpu->suffix = temp;
		suffixSize = 2;
	}
	suffixIsFound = true;
}

void CPUAnalyzer::findFamily() {
	cpu->family = cpu->name.substr(0, 2);
}

void CPUAnalyzer::findTier() {
	findFamily(); // To ensure that the family field is filled in
	int perfLevel = int(cpu->family.at(1)) - 48; // Correct the conversion
	switch (perfLevel) {
		case 3:
			cpu->tier = "Entry Level";
			break;
		case 5:
			cpu->tier = "Mainstream";
			break;
		case 7:
			cpu->tier = "Performance";
			break;
		case 9:
			cpu->tier = "Enthusiast";
			break;
		default:
			cpu->tier = "N/A";
			break;
	}

}

void CPUAnalyzer::findNumber() {
	// Makes sure there's a suffix length to use for isolating the CPU's numbers
	if (!suffixIsFound) {
		findSuffix();
	}
	// Length of the CPU number, based on suffix size (0, 1, or 2), minus 3
	// to deal with "i3", "i5", etc.
	int numLength = cpu->name.length() - suffixSize - 3;
	string numStr = cpu->name.substr(3, numLength);
	cpu->number = stoi(numStr);
	numIsFound = true;
}

void CPUAnalyzer::findGeneration() {
	if (!numIsFound) {
		findNumber();
	}

	// Divide the CPU number by 1000 to isolate the first two digits
	cpu->generation = cpu->number / 1000;
}

string CPUAnalyzer::getSuffix() const {
	return cpu->suffix;
}

string CPUAnalyzer::getFamily() const {
	return cpu->family;
}

string CPUAnalyzer::getTier() const {
	return cpu->tier;
}

int CPUAnalyzer::getNumber() const {
	return cpu->number;
}

int CPUAnalyzer::getGeneration() const {
	return cpu->generation;
}
#endif //INTEL_CPU_APP_V2_ANALYZER_CPP