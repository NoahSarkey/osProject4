// Sam Mustipher, Noah Sarkey
// CSE 30341 - 02
// Project 4
// output.h

////////////////////////////////// TO DO ///////////////////////////////////////
// get libcurl functioning so that we can run all of the checking
// connect to site-tester.cpp
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

class Output {
	public:
		Output() {
			ofstream outputFile;
			outputFile.open("output.txt");

			// write to the file the headers
			outputFile << "Time,Phrase,Site,Count" << endl;

			// actually write everything to the file (need libcurl functioning)

			outputFile.close();
		}	//end of the output function that will write the output file
};
