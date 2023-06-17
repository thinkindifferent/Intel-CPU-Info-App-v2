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
	familyIsFound = true;
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
	genIsFound = true;
}

void CPUAnalyzer::findLithogrpahy() {
	if (!genIsFound) {
		findGeneration();
	}
	
	int gen = cpu->generation;
	if (gen == 2) {
		cpu->lithography = 32;
	}
	else if ((gen == 3) || (gen == 4)) {
		cpu->lithography = 22;
	}
	else if ((gen > 4) && (gen < 12)) {
		cpu->lithography = 14;
	}
	else {
		cpu->lithography = 10;
	}
}

void CPUAnalyzer::findCores() {
	if (!genIsFound || !familyIsFound) {
		findGeneration();
		findFamily();
	}

	// The cpu->cores member would be an array of 2, P cores in index 0
	// and E cores in index 1
	if (cpu->generation < 12) {
		cpu->cores[1] = 0;
	}

	if (cpu->generation < 8) {
		if ((cpu->family == "i7") || (cpu->family == "i5")) {
			cpu->cores[0] = 4;
		}
		else if (cpu->family == "i3") {
			cpu->cores[0] = 2;
		}
		return;
	}
	
	switch (cpu->generation) {
		case 8:
			if ((cpu->family == "i7") || (cpu->family == "i5")) {
				cpu->cores[0] = 6;
			}
			else if (cpu->family == "i3") {
				cpu->cores[0] = 4;
			}
			break;
		case 9:
			if ((cpu->family == "i9") || (cpu->family == "i7")) {
				cpu->cores[0] = 8;
			}
			else if (cpu->family == "i5") {
				cpu->cores[0] = 6;
			}
			else if (cpu->family == "i3") {
				cpu->cores[0] = 4;
			}
			break;
		case 10:
			if (cpu->family == "i9") {
				cpu->cores[0] = 10;
			}
			else if (cpu->family == "i7") {
				cpu->cores[0] = 8;
			}
			else if (cpu->family == "i5") {
				cpu->cores[0] = 6;
			}
			else if (cpu->family == "i3") {
				cpu->cores[0] = 4;
			}
			break;
		case 11:
			if ((cpu->family == "i9") || (cpu->family == "i7")) {
				cpu->cores[0] = 8;
			}
			else if (cpu->family == "i5") {
				cpu->cores[0] = 6;
			}
			break;
		case 12:
			if (cpu->family == "i9") {
				cpu->cores[0] = 8;
				cpu->cores[1] = 8;
			}
			else if (cpu->family == "i7") {
				cpu->cores[0] = 8;
				cpu->cores[1] = 4;
			}
			else if (cpu->family == "i5") {
				cpu->cores[0] = 6;
				if (cpu->number == 12600) {
					cpu->cores[1] = 4;
				}
				else {
					cpu->cores[1] = 0;
				}
			}
			else if (cpu->family == "i3") {
				cpu->cores[0] = 4;
				cpu->cores[1] = 0;
			}
			break;
		case 13:
			if (cpu->family == "i9") {
				cpu->cores[0] = 8;
				cpu->cores[1] = 16;
			}
			else if (cpu->family == "i7") {
				cpu->cores[0] = 8;
				cpu->cores[1] = 8;
			}
			else if (cpu->family == "i5") {
				cpu->cores[0] = 6;
				if (cpu->number >= 13500) {
					cpu->cores[1] = 8;
				}
				else {
					cpu->cores[1] = 4;
				}
			}
			else if (cpu->family == "i3") {
				cpu->cores[0] = 4;
				cpu->cores[1] = 0;
			}
			break;
	}
	
}

void CPUAnalyzer::findMemSupport() {
	if (!genIsFound) {
		findGeneration();
	}

	int gen = cpu->generation;
	if (gen < 6) {
		cpu->memSupport = "DDR3";
	}
	else if ((gen >= 6) && (gen <= 11)) {
		cpu->memSupport = "DDR4";
	}
	else {
		cpu->memSupport = "DDR4/DDR5";
	}
}

void CPUAnalyzer::findArch() {
	if (!genIsFound) {
		findGeneration();
	}

	switch (cpu->generation) {
		case 2:
			cpu->arch = "Sandy Bridge";
			break;
		case 3:
			cpu->arch = "Ivy Bridge";
			break;
		case 4:
			cpu->arch = "Haswell/Devil's Canyon";
			break;
		case 5:
			cpu->arch = "Broadwell";
			break;
		case 6:
			cpu->arch = "Skylake";
			break;
		case 7:
			cpu->arch = "Kaby Lake";
			break;
		case 8:
			cpu->arch = "Coffee Lake";
			break;
		case 9:
			cpu->arch = "Coffee Lake Refresh";
			break;
		case 10:
			cpu->arch = "Comet Lake";
			break;
		case 11:
			cpu->arch = "Rocket Lake";
			break;
		case 12:
			cpu->arch = "Alder Lake";
			break;
		case 13:
			cpu->arch = "Raptor Lake";
			break;
		default:
			cpu->arch = "N/A";
			break;
	}
}

void CPUAnalyzer::findSocket() {
	if (!genIsFound) {
		findGeneration();
	}

	if (cpu->generation < 4) {
		cpu->socket = "LGA1155";
	}
	else if ((cpu->generation == 4) || (cpu->generation == 5)) {
		cpu->socket = "LGA1150";
	}
	else if ((cpu->generation > 5) && (cpu->generation < 10)) {
		cpu->socket = "LGA1151";
	}
	else if ((cpu->generation == 10) || (cpu->generation == 11)) {
		cpu->socket = "LGA1200";
	}
	else {
		cpu->socket = "LGA1700";
	}
}

void CPUAnalyzer::findHasIGPU() {
	if (!suffixIsFound) {
		findSuffix();
	}

	int suffixLength = cpu->suffix.length();
	if (suffixLength == 2) {
		if ((cpu->suffix.at(1) == 'F') || (cpu->suffix.at(1) == 'P')) {
			cpu->hasIGPU = false;
		}
		else {
			cpu->hasIGPU = true;
		}
	}
	else if (suffixLength == 1) {
		if ((cpu->suffix.at(0) == 'F') || (cpu->suffix.at(0) == 'P')) {
			cpu->hasIGPU = false;
		}
		else {
			cpu->hasIGPU = true;
		}
	}

	// TODO: K CPU's don't seem to be caught in this else case
	// Fixed above***
	else {
		cpu->hasIGPU = true;
	}
}

void CPUAnalyzer::findHasSMT() {
	if (!genIsFound || !familyIsFound) {
		findGeneration();
		findFamily();
	}

	if ((cpu->family == "i9") || (cpu->family == "i3")) {
		cpu->hasSMT = true;
	}
	else if ((cpu->family == "i7") && (cpu->generation != 9)) {
		cpu->hasSMT = true;
	}
	else if ((cpu->family == "i5") && (cpu->generation > 9)) {
		cpu->hasSMT = true;
	}
	else {
		cpu->hasSMT = false;
	}
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

int CPUAnalyzer::getLithography() const {
	return cpu->lithography;
}

int* CPUAnalyzer::getCores() const {
	return cpu->cores;
}

string CPUAnalyzer::getMemSupport() const {
	return cpu->memSupport;
}

string CPUAnalyzer::getArch() const {
	return cpu->arch;
}

string CPUAnalyzer::getSocket() const {
	return cpu->socket;
}

bool CPUAnalyzer::getHasIGPU() const {
	return cpu->hasIGPU;
}

bool CPUAnalyzer::getHasSMT() const {
	return cpu->hasSMT;
}
#endif //INTEL_CPU_APP_V2_ANALYZER_CPP