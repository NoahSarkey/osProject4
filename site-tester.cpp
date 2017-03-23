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
	searchTerms = mainsearch.phrase;
	
	cout << "Before fetch" << endl;	
	// Implementing Fetch
	Fetch fetch1;	
	cout << "After fetch" << endl;	
	
	fetch1 = Fetch();

	// this all has to be linked from the config file
	//for (int i = 0; i < mainconfig.SITE_FILE.size(); i ++) {
	//	getline(mainconfig.SITE_FILE, line);
	//}


	//for (int i = 0; i < websites.size(); i ++) {
	//	for (int j = 0; j < searchTerms.size(); i ++) {
	//		fetch1.sites(websites[i]);
	//		//for word in fetch1.html
	//		// if word matches the search term then increment counter
	//		// now print this shit out to the output file
	//	}
	//}

	string website = "http://www.google.com";
	fetch1.sites(website);
	cout << fetch1.html << endl;

	// actually perform the search

	return 0;
}	//end of main function
