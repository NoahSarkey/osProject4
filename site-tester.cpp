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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <queue>
#include <vector>
#include <fstream>
#include <ctime>
#include <utility>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <sstream>
#include <iomanip>
#include <curl/curl.h>
#include "config.h"
#include "search.h"
//#include "fetch.h"
#include "threading.h"
using namespace std;

int CREATE_OUTPUT = 0;
condition_variable conditionalVar1, conditionalVar2;
int TIMECOUNT = 0;
int CSVCOUNT = 1;
int timer = 1;
vector<string> WEBSITES;
vector<string> SEARCH_TERMS;
QueueClass<pair<string,string>> bothData;
QueueClass<string> websitesData;

//struct parseItm {
//	pthread_t id;
//	string site; // Website
//	string data; // Content 
//};

////////////////////////////// FETCH CLASS /////////////////////////////////////

class Fetch {
	public:
		Fetch() {}
	 
		struct MemoryStruct {
		  char *memory;
		  size_t size;
		};
		 
		static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
		{
		  	size_t realsize = size * nmemb;
		  	struct MemoryStruct *mem = (struct MemoryStruct *)userp;
		 
		  	mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
		  	if(mem->memory == NULL) {
		    		/* out of memory! */ 
		    		printf("not enough memory (realloc returned NULL)\n");
		    		return 0;
		  	}
		 
		  	memcpy(&(mem->memory[mem->size]), contents, realsize);
		  	mem->size += realsize;
		  	mem->memory[mem->size] = 0;
		 
		  	return realsize;
		}	//end of writeMemoryCallback function
		 
		void sites(QueueClass<pair<string,string>> &bothData, QueueClass<string> &websitesData, mutex &mut)
		{
			unique_lock<mutex> locker2(mut);
			conditionalVar1.wait(locker2);
		  	CURL *curl_handle;
		  	CURLcode res;
			string searchSite = websitesData.queue_pop();
		  	struct MemoryStruct chunk;
		 
		  	chunk.memory = (char *)malloc(1);  /* will be grown as needed by the realloc above */ 
		  	chunk.size = 0;    /* no data at this point */ 
		 
		  	curl_global_init(CURL_GLOBAL_ALL);
		 
		  	/* init the curl session */ 
		  	curl_handle = curl_easy_init();
		 
		  	/* specify URL to get */ 
		  	curl_easy_setopt(curl_handle, CURLOPT_URL, searchSite.c_str());
		 
		  	/* send all data to this function  */ 
		  	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		 
		  	/* we pass our 'chunk' struct to the callback function */ 
		  	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
		 
		  	/* some servers don't like requests that are made without a user-agent field, so we provide one */ 
		  	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
		 
		  	/* get it! */ 
		  	res = curl_easy_perform(curl_handle);
		 
		  	/* check for errors */ 
		  	if(res != CURLE_OK) {
		    		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		  	
			}

		 	else {
		 		html = string(chunk.memory);
				bothData.queue_push(make_pair(html, searchSite));
		  	}
		 
		  	/* cleanup curl stuff */ 
		  	curl_easy_cleanup(curl_handle);
		 
		  	free(chunk.memory);
		 
		  	/* we're done with libcurl, so clean it up */ 
		  	curl_global_cleanup();
			conditionalVar2.notify_one();
		}
		
		string html;
		mutex mu;
};

////////////////////////////////////////////////////////////////////////////////

string getTimeDate(){
	time_t now = time(0);
	string tc = ctime (&now);
	tc.erase(tc.size()-1);
	return tc;
}

////////////////////////////////////////////////////////////////////////////////
/*
void alarmFunc() {
	for (unsigned int i = 0; i < WEBSITES.size()-1; i ++) {
		fetchItm temp;
		temp.site = WEBSITES[i];
		fetchQueue.Enqueue(temp);
	}
	alarm(timer);
}
*/
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

void output(string dt, string phrase, string site, int count) {
	//if (CREATE_OUTPUT == 0) {
		ofstream outputFile;
		string result;
		ostringstream convert;
		convert << CSVCOUNT;
		result = convert.str();
		string filename = result + ".csv";
		outputFile.open(filename.c_str());
	//}
	CREATE_OUTPUT ++;

	string firstline = "";
	if (TIMECOUNT == 0){
		firstline = "Time,Phrase,Site,Count\n";
	}
	
	// convert int to string
	//string result;
	//ostringstream convert;
	//convert << count;
	//result = convert.str();

	TIMECOUNT+=1;
	string s = firstline + dt + "," + phrase + "," + site + "," + result + "\n"; 
	outputFile << s;
	unsigned int v2 = SEARCH_TERMS.size() * WEBSITES.size();
	unsigned int v1 = CREATE_OUTPUT;
	if (v1 == v2) outputFile.close();
}

////////////////////////////////////////////////////////////////////////////////

void signalHandler(int value) {
	curl_global_cleanup();
	exit(0);
}

void setFlag(int value) {
	conditionalVar1.notify_one();
	alarm(30);
}

////////////////////////////////////////////////////////////////////////////////

void countApp(QueueClass<pair<string,string>> &bothData, QueueClass<string> &websitesData, mutex &mut) {
	unique_lock<mutex> locker(mut);
	conditionalVar2.wait(locker);
	string currtime = getTimeDate();
	pair<string,string> p = bothData.queue_pop();
	//int inc = 0;
	
	for (unsigned int i = 0; i < SEARCH_TERMS.size(); i ++)
	{
		int position = 0;
		int inc = 0;
		while (p.first.find(SEARCH_TERMS[i], position) != string::npos) {
			position = p.first.find(SEARCH_TERMS[i], position) + SEARCH_TERMS[i].size();
			inc ++;
		}	//end of while loop
		//outfile << output(currtime, SEARCH_TERMS[i], p.second, inc);
		output(currtime, SEARCH_TERMS[i], p.second, inc);
	}
}

////////////////////////////////////////////////////////////////////////////////

int main (int argc, char * argv[]){

	// declare variables
	string configfile;
	//QueueClass<pair<string.string>> bothData;
	//QueueClass<string> websitesData;
	vector<thread> fetchThread;
	vector<thread> parseThread;

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
	SEARCH_TERMS = mainsearch.phrase;

	// Implementing Fetch
	Fetch fetch1;	
	
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

//////////////////////// begin forming the threads /////////////////////////////
	signal(SIGALRM, setFlag);
	signal(SIGINT, signalHandler);
	signal(SIGHUP, signalHandler);
	alarm(30);
	mutex mut;

	for (int i = 0; i < mainconfig.NUM_FETCH; i ++) {
		fetchThread.emplace_back(fetch1.sites, ref(bothData), ref(websitesData), ref(mut));
	}

	for (int j = 0; j < mainconfig.NUM_PARSE; j ++) {
		parseThread.emplace_back(countApp, ref(bothData), ref(websitesData), ref(mut));
	}

	for (int k = 0; k < mainconfig.NUM_FETCH; k ++) {
		fetchThread[k].join();
	}	

	for (int l = 0; l < mainconfig.NUM_PARSE; l ++) {
		parseThread[l].join();
	}

	// close the file
	//outputFile.close();
	curl_global_cleanup();
	return 0;
}	//end of main function
