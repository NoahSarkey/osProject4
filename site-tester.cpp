// Sam Mustipher, Noah Sarkey
// CSE 30341 - 02
// Project 4
// site-tester.cpp

////////////////////////////////// TO DO ///////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include "config.h"
#include "search.h"
#include "fetch.h"
using namespace std;

int main (int argc, char * argv[])
{
	string configfile;
	if (argc != 2) {
		cout << "Error. Invalid number of arguments." << endl;
		exit(1);
	}
	else {
		// getting the config filefp
		configfile = argv[1];
		cout << "Config File: " << configfile << endl;
	}

	// Implementing config
	Config mainconfig((configfile));
	Search mainsearch;
	mainconfig.output();
	
	// Implementing search
	vector<string> searchTerms;
	mainsearch.create(mainconfig.SEARCH_FILE);
	searchTerms = mainsearch.phrase;
	
	// Implementing Fetch
	Fetch fetch1;	
	fetch1 = Fetch();

	// read the websites into a vector
	vector<string> websites;
	ifstream webFile;
	webFile.open(mainconfig.SITE_FILE.c_str());

	cout << mainconfig.SITE_FILE.c_str() << endl;

	if (webFile.is_open()) {
		while (!webFile.eof()) {
			string line = "";	
			getline(webFile, line);
			websites.push_back(line);		
		}	//end while loop
	}	//end if statement

	cout << websites.size() << " " << searchTerms.size() << endl;

	for (unsigned int i = 0; i < websites.size(); i ++) {
		for (unsigned int j = 0; j < searchTerms.size(); j ++) {
			int counter = 0;
			string word = "";
			fetch1.sites(websites[i]);
			
			int position = 0;
			while (fetch1.html.find(searchTerms[i], position) != string::npos) {
				position = position + searchTerms[i].size();
				counter ++;
			}	//end of while loop
			
			cout << websites[i] << " " << searchTerms[j] << " " << counter << endl;
		}	//end of for loop
	}	//end of for loop

	return 0;
}	//end of main function
