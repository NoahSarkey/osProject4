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
#include <ctime>
#include <sstream>
#include <iomanip>
#include "config.h"
#include "search.h"
#include "fetch.h"
using namespace std;

int TIMECOUNT = 0;
int CSVCOUNT = 1;

string getTimeDate(){
	time_t now = time(0);
	string tc = ctime (&now);
	tc.erase(tc.size()-1);
	return tc;
}

string output(string dt, string phrase, string site, int count){
	string firstline = "";
	if (TIMECOUNT == 0){
		firstline = "Date and Time, Search Phrase, Site, Count\n";
	}
	// cout << dt << ", " << phrase << ", " << site << ", " << count << endl
	
	// convert int to string
	string result;
	ostringstream convert;
	convert << count;
	result = convert.str();

	TIMECOUNT+=1;
	return firstline + dt + ", " + phrase + ", " + site + ", " + result + "\n"; 
}

int main (int argc, char * argv[]){

	string configfile;
	if (argc != 2) {
		cout << "Error. Invalid number of arguments." << endl;
		exit(1);
	}
	else {
		// getting the config filefp
		if(fopen(argv[1], "r")){
			configfile = argv[1];
			cout << "Config File: " << configfile << endl;
		}
		else{
			cout << "Given config file does not exist" << endl;
			exit(1);
		}
	}

	// Implementing config
	Config mainconfig((configfile));
	Search mainsearch;
	mainconfig.output();
	
	// Implementing search
	vector<string> searchTerms;
	if(fopen(mainconfig.SEARCH_FILE.c_str(), "r")){
		mainsearch.create(mainconfig.SEARCH_FILE);
	}
	else{
		cout << "Given search file does not exist" << endl;
		exit(1);
	}
	searchTerms = mainsearch.phrase;

	// Implementing Fetch
	Fetch fetch1;	
	fetch1 = Fetch();

	// read the websites into a vector
	vector<string> websites;
	ifstream webFile;
	if(fopen(mainconfig.SITE_FILE.c_str(), "r")){
		webFile.open(mainconfig.SITE_FILE.c_str());
	}
	else{
		cout << "Given site file does not exist" << endl;
		exit(1);
	}

	if (webFile.is_open()) {
		while (!webFile.eof()) {
			string line = "";	
			getline(webFile, line);
			websites.push_back(line);		
		}	//end while loop
	}	//end if statement

	// create file
	ofstream outputFile;
	string result;
	ostringstream convert;
	convert << CSVCOUNT;
	result = convert.str();
	string filename = result + ".csv";
	outputFile.open(filename.c_str());

	for (unsigned int i = 0; i < websites.size() - 1; i ++) {
		for (unsigned int j = 0; j < searchTerms.size() - 1; j ++) {
			int counter = 0;
			string word = "";
			fetch1.sites(websites[i]);
			
			int position = 0;

			// get time and date curr time
			string currtime = getTimeDate();
		
			while (fetch1.html.find(searchTerms[j], position) != string::npos) {
				position = fetch1.html.find(searchTerms[j], position) + searchTerms[j].size();				
				counter ++;
			}	//end of while loop
			
			outputFile << output(currtime, searchTerms[j], websites[i], counter);
		}	//end of for loop
	}	//end of for loop

	// close the file
	outputFile.close();

	return 0;
}	//end of main function
