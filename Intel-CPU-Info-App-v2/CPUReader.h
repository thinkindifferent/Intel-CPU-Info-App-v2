#ifndef INTEL_CPU_APP_V2_READER_H
#define INTEL_CPU_APP_V2_READER_H
#include <string>
#include <set>


using namespace std;

class CPUReader {
private:
	string CPUName;
	set<string> CPUSet;
	bool goodCPU;

public:
	// No-arg constructor, should just initialize everything to zero
	CPUReader();

	// Constructor, if createSet is true, will create a set that would store valid CPU names
	CPUReader(bool createSet);

	// Destructor, clears out the CPUSet
	~CPUReader();

	void loadCPUList();

	// Read in a CPU from the istream
	void readCPU();

	// Determines if the CPU is compatible with the app's algorithms
	void verifyCPU();

	// Accessor for goodCPU flag
	bool isGoodCPU();

	// Accessor for CPU name
	string getCPUName();

};
#endif //INTEL_CPU_APP_V2_READER_H