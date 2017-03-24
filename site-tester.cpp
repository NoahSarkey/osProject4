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
#include "threading.h"
using namespace std;

int TIMECOUNT = 0;
int CSVCOUNT = 1;
int timer = 1;
vector<string> WEBSITES;

struct parseItm {
	pthread_t id;
	string site; // Website
	string data; // Content 
};

QueueClass<parseItm> parseQueue;
QueueClass<string> fetchQueue; //
QueueClass<string> resultsQueue;

////////////////////////////////////////////////////////////////////////////////

string getTimeDate(){
	time_t now = time(0);
	string tc = ctime (&now);
	tc.erase(tc.size()-1);
	return tc;
}

////////////////////////////////////////////////////////////////////////////////

void alarmFunc() {
	for (unsigned int i = 0; i < WEBSITES.size()-1; i ++) {
		fetchItm temp;
		temp.site = WEBSITES[i];
		fetchQueue.Enqueue(temp);
	}
	alarm(timer);
}

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

string output(string dt, string phrase, string site, int count) {
	string firstline = "";
	if (TIMECOUNT == 0){
		firstline = "Time,Phrase,Site,Count\n";
	}
	// cout << dt << ", " << phrase << ", " << site << ", " << count << endl
	
	// convert int to string
	string result;
	ostringstream convert;
	convert << count;
	result = convert.str();

	TIMECOUNT+=1;
	return firstline + dt + "," + phrase + "," + site + "," + result + "\n"; 
}

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

int main (int argc, char * argv[]){

	// declare variables
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
			WEBSITES.push_back(line);		
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

//////////////////////// begin forming the threads /////////////////////////////
	pthread_t threads[NUM_THREADS];
	struct fetchItm td[NUM_THREADS];
	int rc;
	
	for (int i = 0;i < NUM_FETCH; i ++) {
		cout << "main() : creating thread, " << i << endl;
		td[i].id = i;
		td[i].site = WEBSITES[j];
		rc = pthread_create(&threads[i], NULL, func ,NULL); // make function that calls sites on fetch1 and places on to the queue 
	}

	// make this into a curl function
	for (unsigned int i = 0; i < WEBSITES.size() - 1; i ++) {
		for (unsigned int j = 0; j < searchTerms.size() - 1; j ++) {
			int counter = 0;
			string word = "";
			fetch1.sites(WEBSITES[i]);
			
			int position = 0;

			// get time and date curr time
			string currtime = getTimeDate();
		
			// count make into function
			while (fetch1.html.find(searchTerms[j], position) != string::npos) {
				position = fetch1.html.find(searchTerms[j], position) + searchTerms[j].size();				
				counter ++;
			}	//end of while loop
			
			outputFile << output(currtime, searchTerms[j], WEBSITES[i], counter);
		}	//end of for loop
	}	//end of for loop

	// close the file
	outputFile.close();

	return 0;
}	//end of main function
