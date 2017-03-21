// Sam Mustipher, Noah Sarkey
// CSE 30341 - 02
// Project 4
// output.h

////////////////////////////////// TO DO ///////////////////////////////////////


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


			outputFile.close();
		}	//end of the output function that will write the output file
};
