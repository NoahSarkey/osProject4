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
	searchTerms = mainsearch.create(mainconfig.SEARCH_FILE);
	
	cout << "Before fetch" << endl;	
	// Implementing Fetch
	Fetch fetch1(mainconfig.SITE_FILE);	
	cout << "After fetch" << endl;	

	return 0;
}	//end of main function

