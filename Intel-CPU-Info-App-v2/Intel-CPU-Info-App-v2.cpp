#ifndef INTEL_CPU_APP_V2
#define INTEL_CPU_APP_V2
#include <iostream>
#include <string>
#include <set>
#include <fstream>
#include <sstream>
#include "CPUReader.h"
#include "CPUAnalyzer.h"

using namespace std;

fstream fout;
bool saveToFile;
bool validYorN;
bool testAnotherCPU;
char saveForUser;
char anotherCPUInput;
string savePath;

int main()
{
	// Read in CPU data
	cout << "Loading CPU Database..." << endl;
	CPUReader nonAnalyzedCPU(true);
	do {
		// Prompt user to enter a CPU name
		cout << "Enter an Intel Desktop CPU Name in the following format without quotes" << endl;
		cout << "e.g. \"i7-6700K\"" << endl;

		// TODO: Invalid CPUs will get through to the CPUAnalyzer object, fix it
		// **Update: Fixed


		// Read and verify the CPU
		do {
			nonAnalyzedCPU.readCPU();
			nonAnalyzedCPU.verifyCPU();

			// If the user enters an invalid CPU
			if (!nonAnalyzedCPU.isGoodCPU()) {
				cout << "Invalid CPU. Please enter a new CPU name." << endl;
			}
			// Loop back for bad CPUs
		} while (!nonAnalyzedCPU.isGoodCPU());

		cout << "Printing results..." << endl << endl;

		/*
			TESTING AREA! ONLY FOR DIAGNOSING INDIVIDUAL FUNCTIONS
		*/
		CPUAnalyzer analyzedCPU(nonAnalyzedCPU.getCPUName());
		analyzedCPU.extractInfo();
		analyzedCPU.printResults();

		// Section for whether the user would like to save to a file
		cout << endl;
		cout << "Would you like to save these results to a file? (Y/N)" << endl;
		cin >> saveForUser;
		do {
			// If the user wants to save to a file, assumes a valid file path
			if ((saveForUser == 'Y') || (saveForUser == 'y')) {
				cout << "Please enter a file path with file extension." << endl;
				cout << "If the file exists, this CPU's information will be" << endl;
				cout << "appended to the end of the file." << endl;
				cin >> savePath;

				// Source: https://www.geeksforgeeks.org/csv-file-management-using-c/
				// and https://stackoverflow.com/questions/23603735/attempting-to-reference-a-deleted-function
				// Additionally, there is a warning to the user that the
				// program will append to the file if it already exists
				fout.open(savePath, ios::out | ios::app);

				// Check that the file can be opened, if not, loop back around
				if (!fout.is_open()) {
					cout << "File could not be opened. Please retype your path." << endl;
					validYorN = false;
					continue;
				}
				else {
					analyzedCPU.outputResults(fout);
					cout << "CPU information saved." << endl;
				}

				validYorN = true;
			}
			// If the user doesn't want to save to file
			else if ((saveForUser == 'N') || (saveForUser == 'n')) {
				validYorN = true;
			}
			// If an invalid char was entered
			else {
				cout << "Please enter a Y or N." << endl;
				validYorN = false;
			}
		} while (!validYorN);

		cout << "Would you like to enter a new CPU? (Y/N)" << endl;
		cin >> anotherCPUInput;

		// Section for whether the user would like to enter a new CPU
		do {
			// If the uesr wants to enter another CPU
			if ((anotherCPUInput == 'Y') || (anotherCPUInput == 'y')) {
				testAnotherCPU = true;
				break;
			}
			// If the user doesn't want to enter another CPU
			else if ((anotherCPUInput == 'N') || (anotherCPUInput == 'n')) {
				testAnotherCPU = false;
				break;
			}
			// If the user enters an invalid char
			else {
				cout << "Please enter a Y or N." << endl;
			}
		} while (!validYorN);
	} while (testAnotherCPU);

	return 0;
}


#endif //INTEL_CPU_APP_V2