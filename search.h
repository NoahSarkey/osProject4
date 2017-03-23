// Sam Mustipher, Noah Sarkey
// CSE 30341 - 02
// Project 4
// search.h

////////////////////////////////// TO DO ///////////////////////////////////////
// Take care of carraige returns
// no wildcards or regular expressions? what does this entail
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

class Search {
	public: 
		void create(string filename)
		{
			ifstream inputFile;
			inputFile.open(filename.c_str());
			//vector<string> phrase;
	
			if (inputFile.is_open()) {
				while (!inputFile.eof()) {
					int kill = 0;
					string w = "";
					// read in the line
					getline(inputFile, w);

					//do all of the work to clean the file
					for (unsigned int i = 0; i < w.size(); i ++) {
						if (w[i] == ',') {
							kill = 1;
						}
					}

					// add to the vector
					if (kill == 0) phrase.push_back(w);

				}
			}
	
			//for (unsigned int i = 0; i < phrase.size(); i ++) {
			//	cout << phrase[i] << endl;
			//}

			inputFile.close();
			//return phrase;
		}	//end of create function

		vector<string> phrase;
};	//end of class
