// Sam Mustipher, Noah Sarkey
// CSE 30341 - 02
// Project 4

#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

void create()
{
	ifstream inputFile;
	inputFile.open("Search.txt");
	vector<string> phrase;
	
	if (inputFile.is_open()) {
		while (!inputFile.eof()) {
			int kill = 0;
			string w = "";
			// read in the line
			getline(inputFile, w);

			//do all of the work to clean the file
			for (int i = 0; i < w.size(); i ++) {
				if (w[i] == ',') {
					kill = 1;
				}
			}

			// add to the vector
			if (kill == 0) phrase.push_back(w);

		}
	}
	
	for (int i = 0; i < phrase.size(); i ++) {
		cout << phrase[i] << endl;
	}

	inputFile.close();
}	//end of create function
