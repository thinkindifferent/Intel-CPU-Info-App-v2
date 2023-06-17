#ifndef INTEL_CPU_APP_V2_ANALYZER_H
#define INTEL_CPU_APP_V2_ANALYZER_H

#include <iostream>
#include <string>


using namespace std;

class CPUAnalyzer {
private:
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

	CPUInfo* cpu = new CPUInfo;
	short suffixSize;
	bool suffixIsFound;
	bool numIsFound;
	bool genIsFound;

public:

	/*
		Copy constructor to make a new, analyzed CPU
	*/
	CPUAnalyzer(const CPUAnalyzer& oldCPU);

	/*
		Constructor to initialize analyzing a CPU with a given name
	*/
	CPUAnalyzer(string name);

	
	void extractInfo(CPUInfo cpu);

	// Finds CPU Name (e.g. "i7-6700K")
	//void findName();

	/*
		Finds any applicable suffixes (e.g. "KS" for unlocked special edition)
	*/
	void findSuffix();

	/*
		Finds the CPU family (e.g. i3, i5...)
	*/
	void findFamily();

	/*
		Finds the type of RAM supported
	*/
	void findMemSupport();

	/*
		Finds the CPU architecture codename
	*/
	void findArch();

	/*
		Finds the socket the CPU uses
	*/
	void findSocket();

	/*
		Determines the performance tier or target audience, relies on the result
		produced by findFamily()
	*/
	void findTier();

	/*
		Extracts the numeric values inside the CPU name, relies on the result
		produced by findSuffix() to get the number
	*/
	void findNumber();

	/*
		Determines generation (2nd to 13th)
	*/
	void findGeneration();

	/*
		Finds the lithography of the CPU
	*/
	void findLithogrpahy();

	/*
		Determines whether or not the CPU has an iGPU
	*/
	void findHasIGPU();

	/*
		Determines whether or not the CPU supports hyperthreading
	*/
	void findHasSMT();

	/*
		Prints the results of the analysis
	*/
	void printResults();

	// Accessor for the CPU suffix
	string getSuffix() const;

	// Accessor for the CPU family
	string getFamily() const;

	// Accessor for the CPU's supported memory
	string getMemSupport() const;

	// Accessor for the CPU architecture
	string getArch() const;

	// Accessor for the CPU socket
	string getSocket() const;

	// Accessor for the CPU's performance tier
	string getTier() const;

	// Accessor for the CPU number
	int getNumber() const;

	// Accessor for the CPU's generation
	int getGeneration() const;

	// Accessor for the CPU's lithography process
	int getLithography() const;

	// Accessor for whether the CPU has an iGPU
	bool getHasIGPU() const;

	// Accessor for whether the CPU has hyperthreading
	bool getHasSMT() const;
};
#endif //INTEL_CPU_APP_V2_ANALYZER_H