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

void CPUAnalyzer::extractInfo() {
	findSuffix();
	findFamily();
	findTier();
	findNumber();
	findGeneration();
	findLithogrpahy();
	findMemSupport();
	findArch();
	findSocket();
	findHasIGPU();
	findHasSMT();
	findHasPECores();
	findCores();
	findThreads();
	findSuffixProperties();
	findHasTurbo();
}

void CPUAnalyzer::printResults() {
	// Printed titles are 40 chars wide
	// FYI: The information is not in any specific order, just grouped
	// to common sections

	cout << "==========Basic CPU Information=========" << endl;
	cout << "CPU Name: " << cpu->name << endl;
	cout << "CPU Number: " << cpu->number << endl;
	cout << "CPU Family: Core " << cpu->family << endl;
	cout << "Architecture: " << cpu->arch << endl;
	cout << "Lithography: " << cpu->lithography << "nm" << endl;
	cout << "Tier: " << cpu->tier << endl;
	cout << "Generation: " << cpu->generation << writeNumericSuffix(cpu->generation) << endl << endl;

	cout << "===========CPU Specifications===========" << endl;
	cout << "Total Cores: " << (cpu->cores[0] + cpu->cores[1]) << endl;
	cout << "Has P/E Cores? " << boolToStr(cpu->hasPECores) << endl;
	cout << "Core Makeup: " << (cpu->cores[0]) << " P Cores, " << (cpu->cores[1]) << " E Cores" << endl;
	cout << "Total Threads: " << cpu->threads << endl;
	cout << "Has Hyperthreading? " << boolToStr(cpu->hasSMT) << endl;
	cout << "Has iGPU? " << boolToStr(cpu->hasIGPU) << endl << endl;

	cout << "=======Supporting CPU Information=======" << endl;
	cout << "Socket Supported: " << cpu->socket << endl;
	cout << "Memory Supported: " << cpu->memSupport << endl;
	cout << "Special Properties: " << cpu->suffixProp << endl;
}

void CPUAnalyzer::findSuffix() {
	char lastChar, secLastChar;
	bool lastCharIsDig, secLastCharIsDig;

	// Keep track of the last two chars in the CPU name
	lastChar = cpu->name.back();
	secLastChar = cpu->name[cpu->name.length() - 2];

	// See if the last chars are either numerical digits or letters
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
	// Since it is assumed the CPU name starts with "i3", etc., the CPU
	// family will just be the first couple of characters
	cpu->family = cpu->name.substr(0, 2);
	familyIsFound = true;
}

void CPUAnalyzer::findMemSupport() {
	// Check if generation is found or not, needed for analysis
	if (!genIsFound) {
		findGeneration();
	}

	int gen = cpu->generation;
	if (gen < 6) {
		// Everything pre-6th gen only supports DDR3
		cpu->memSupport = "DDR3";
	}
	else if ((gen >= 6) && (gen <= 11)) {
		// Most generations from 2015 to 2021 support DDR4 only
		cpu->memSupport = "DDR4";
	}
	else {
		// Everything else supports both DDR4 and DDR5
		cpu->memSupport = "DDR4/DDR5";
	}
}

void CPUAnalyzer::findArch() {
	// Generation is needed for analysis
	if (!genIsFound) {
		findGeneration();
	}

	// Switch cases for different generations and their respective
	// architectural names
	switch (cpu->generation) {
	case 2:
		cpu->arch = "Sandy Bridge (2011)";
		break;
	case 3:
		cpu->arch = "Ivy Bridge (2012)";
		break;
	case 4:
		// Just for simplicity's sake it counts Haswell Refresh CPUs (e.g.
		// i7-4790K) in the same architecture name
		cpu->arch = "Haswell/Devil's Canyon (2013/14)";
		break;
	case 5:
		cpu->arch = "Broadwell (2015)";
		break;
	case 6:
		cpu->arch = "Skylake (2015)";
		break;
	case 7:
		cpu->arch = "Kaby Lake (2017)";
		break;
	case 8:
		cpu->arch = "Coffee Lake (2017)";
		break;
	case 9:
		cpu->arch = "Coffee Lake Refresh (2018)";
		break;
	case 10:
		cpu->arch = "Comet Lake (2020)";
		break;
	case 11:
		cpu->arch = "Rocket Lake (2021)";
		break;
	case 12:
		cpu->arch = "Alder Lake (2021)";
		break;
	case 13:
		cpu->arch = "Raptor Lake (2022)";
		break;
	default:
		// No case should reach this, but just to be sure though
		cpu->arch = "N/A";
		break;
	}
}

void CPUAnalyzer::findSocket() {
	// Generation is needed for analysis
	if (!genIsFound) {
		findGeneration();
	}

	// If-else to narrow down the socket based on generation
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

void CPUAnalyzer::findTier() {
	// Need the CPU's family to analyze
	if (!familyIsFound) {
		findFamily();
	}

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
			// Shouldn't reach this case, but just to be safe
			cpu->tier = "N/A";
			break;
	}

}

void CPUAnalyzer::findSuffixProperties() {
	// We need the CPU's suffix after all...
	if (!suffixIsFound) {
		findSuffix();
	}
	string prop;

	// No suffix means no additional properties
	if (suffixSize == 0) {
		prop = "N/A";
	}

	// For loop will run for only however long the suffix is,
	// continually adds properties to the return string labeled prop
	for (int i = 0; i < suffixSize; i++) {

		switch (cpu->suffix.at(i)) {
		case 'C':
			prop += "LGA1150 High Performance iGPU";
			break;
		case 'E':
			prop += "Embedded Processor";
			break;
		case 'F':
			prop += "Requires Discrete Graphics";
			break;
		case 'K':
			prop += "Unlocked";
			break;
		case 'P':
			prop += "Requires Discrete Graphics";
			break;
		case 'S':
			prop += "Special Edition";
			break;
		case 'T':
			prop += "Power-Optimized Lifestyle";
			break;
		default:
			prop += "N/A";
			break;
		}

		// Add a comma after the first property for two character suffixes
		if ((suffixSize == 2) && (i == 0)) {
			prop += ", ";
		}
	}

	cpu->suffixProp = prop;
}

void CPUAnalyzer::findNumber() {
	// Makes sure there's a suffix length to use for isolating the CPU's numbers
	if (!suffixIsFound) {
		findSuffix();
	}
	// Length of the CPU number, based on suffix size (0, 1, or 2), minus 3
	// to deal with "i3", "i5", etc.
	int numLength = cpu->name.length() - suffixSize - 3;

	// Substring of only the numerical chars inside the name
	string numStr = cpu->name.substr(3, numLength);
	cpu->number = stoi(numStr);
	numIsFound = true;
}

void CPUAnalyzer::findGeneration() {
	// We need the CPU number to get its generation
	if (!numIsFound) {
		findNumber();
	}

	// Divide the CPU number by 1000 to isolate the first two digits
	cpu->generation = cpu->number / 1000;
	genIsFound = true;
}

void CPUAnalyzer::findLithogrpahy() {
	// We need generation to analyze for the CPU's lithography process
	if (!genIsFound) {
		findGeneration();
	}
	// Temp var
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
	// All CPU's pre-12th gen have zero E cores
	if (cpu->generation < 12) {
		cpu->cores[1] = 0;
	}

	// i5's and i7's before 8th gen all had 4 cores
	if (cpu->generation < 8) {
		if ((cpu->family == "i7") || (cpu->family == "i5")) {
			cpu->cores[0] = 4;
		}
		// All i3's had 2 cores
		else if (cpu->family == "i3") {
			cpu->cores[0] = 2;
		}
		return;
	}
	
	// Every next generation had different core counts, the switch statement
	// would account for each of those core count changes
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
	coresAreFound = true;
}

void CPUAnalyzer::findThreads() {
	// Need to know if SMT and core count are both found
	if (!smtIsFound && !coresAreFound) {
		findHasSMT();
		findCores();
	}
	
	// Hyperthreading means 2 times the P core count
	if (cpu->hasSMT) {
		cpu->threads = (2 * cpu->cores[0]) + cpu->cores[1];
	}
	else {
		// If no hyperthreading, just add the core counts together
		cpu->threads = cpu->cores[0] + cpu->cores[1];
	}
}

void CPUAnalyzer::findHasIGPU() {
	// Need to know if the suffix contains an F or P (i.e. no iGPU)
	if (!suffixIsFound) {
		findSuffix();
	}
	int suffixLength = cpu->suffix.length();

	// For the 2 char long suffix case
	if (suffixLength == 2) {
		cpu->hasIGPU = !((cpu->suffix.at(1) == 'F') || (cpu->suffix.at(1) == 'P'));
	}
	// For the 1 char long suffix case
	else if (suffixLength == 1) {
		cpu->hasIGPU = !((cpu->suffix.at(0) == 'F') || (cpu->suffix.at(0) == 'P'));
	}
	else {
		// Everything else has an iGPU
		cpu->hasIGPU = true;
	}
}

void CPUAnalyzer::findHasSMT() {
	// Need both generation and family to analyze
	if (!genIsFound || !familyIsFound) {
		findGeneration();
		findFamily();
	}

	// Case by case basis for finding hyperthreading, since it
	// changes a lot for several generations or families
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
	
	smtIsFound = true;
}

void CPUAnalyzer::findHasPECores() {
	// Only 12th gen and newer have both P and E cores
	cpu->hasPECores = (cpu->generation >= 12);
}

void CPUAnalyzer::findHasTurbo() {
	// Need generation and family for analysis
	if (!genIsFound || !familyIsFound) {
		findGeneration();
		findFamily();
	}

	// If the CPU is not a pre-9th gen i3, it has turbo boost
	cpu->hasTurbo = !((cpu->family == "i3") && (cpu->generation < 9));
}

string CPUAnalyzer::getSuffix() const {
	return cpu->suffix;
}

string CPUAnalyzer::getFamily() const {
	return cpu->family;
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

string CPUAnalyzer::getTier() const {
	return cpu->tier;
}

string CPUAnalyzer::getSuffixProperties() const {
	return cpu->suffixProp;
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

int CPUAnalyzer::getThreads() const {
	return cpu->threads;
}

bool CPUAnalyzer::getHasIGPU() const {
	return cpu->hasIGPU;
}

bool CPUAnalyzer::getHasSMT() const {
	return cpu->hasSMT;
}

bool CPUAnalyzer::getHasPECores() const {
	return cpu->hasPECores;
}

bool CPUAnalyzer::getHasTurbo() const {
	return cpu->hasTurbo;
}

string CPUAnalyzer::writeNumericSuffix(int num) {
	// Since all generations are numerically under 20, numbers' suffixes would
	// mostly be "th", only 1st, 2nd, and 3rd will have different suffixes
	string ret;
	switch (num) {
		case 1:
			// None should reach this case, but good to be safe
			ret = "st";
			break;
		case 2:
			ret = "nd";
			break;
		case 3:
			ret = "rd";
			break;
		default:
			ret = "th";
			break;
	}
	return ret;
}

string CPUAnalyzer::boolToStr(bool in) {
	// Could use a ternary operator instead, but who uses those anymore
	if (in) {
		return "Yes";
	}
	else {
		return "No";
	}
}

#endif //INTEL_CPU_APP_V2_ANALYZER_CPP