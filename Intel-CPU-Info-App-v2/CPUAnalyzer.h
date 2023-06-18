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
		string suffixProp;
		int number;
		int generation;
		int lithography;
		int cores[2];
		int threads;
		bool hasIGPU;
		bool hasSMT;
		bool hasPECores;
		bool hasTurbo;
	};

	CPUInfo* cpu = new CPUInfo;
	short suffixSize;
	bool suffixIsFound;
	bool numIsFound;
	bool genIsFound;
	bool familyIsFound;
	bool smtIsFound;

	// Helper to write suffixes for numbers, e.g. 3rd, 5th, etc.
	string writeNumericSuffix(int num);

	// Helper to make a "Yes" or "No" from a bool
	string boolToStr(bool in);
public:

	/*
		Copy constructor to make a new, analyzed CPU
	*/
	CPUAnalyzer(const CPUAnalyzer& oldCPU);

	/*
		Constructor to initialize analyzing a CPU with a given name
	*/
	CPUAnalyzer(string name);

	/*
		Combines all find() methods to extract information in one call
	*/
	void extractInfo();

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
		Determines the special properties that a CPU's suffix would have
	*/
	void findSuffixProperties();

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
		Finds the number of cores the CPU would have
	*/
	void findCores();

	/*
		Finds the number of threads the CPU would have
	*/
	void findThreads();

	/*
		Determines whether or not the CPU has an iGPU
	*/
	void findHasIGPU();

	/*
		Determines whether or not the CPU supports hyperthreading
	*/
	void findHasSMT();

	/*
		Determines whether or not the CPU has P or E cores
	*/
	void findHasPECores();

	/*
		Determines whether or not the CPU is capable of turbo boost
	*/
	void findHasTurbo();

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

	// Accessor for CPU suffix properties
	string getSuffixProperties() const;

	// Accessor for the CPU number
	int getNumber() const;

	// Accessor for the CPU's generation
	int getGeneration() const;

	// Accessor for the CPU's lithography process
	int getLithography() const;

	// Accessor for the core count
	int* getCores() const;

	// Accessor for the thread count
	int getThreads() const;

	// Accessor for whether the CPU has an iGPU
	bool getHasIGPU() const;

	// Accessor for whether the CPU has hyperthreading
	bool getHasSMT() const;

	// Accessor for whether there are P and E cores
	bool getHasPECores() const;

	// Accessor for whether the CPU has turbo boost
	bool getHasTurbo() const;
};
#endif //INTEL_CPU_APP_V2_ANALYZER_H