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
#include <chrono>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <sstream>
#include <iomanip>
#include <curl/curl.h>
#include "config.h"
#include "search.h"
#include "threading.h"
using namespace std;

int CREATE_OUTPUT = 0;
condition_variable conditionalVar1, conditionalVar2;
int CSVCOUNT = 1;
int timer = 1;
vector<string> WEBSITES;
vector<string> SEARCH_TERMS;
QueueClass<pair<string,string>> bothData;
QueueClass<string> websitesData;

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
		 
		static void sites(QueueClass<pair<string,string>> &bothData, QueueClass<string> &websitesData, mutex &mut)
		{
			while (1) {
				unique_lock<mutex> locker2(mut);
				conditionalVar1.wait(locker2);
			  	CURL *curl_handle;
			  	CURLcode res;
				while (!websitesData.empty()) {
					string searchSite = websitesData.queue_pop();
					cout << "INSIDE OF CURL THE SITE IS " << searchSite << endl;
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
						string h = string(chunk.memory);
				 		//html = string(chunk.memory);
						bothData.queue_push(make_pair(h, searchSite));
						//bothData.queue_push(make_pair(html, searchSite));
				  	}
				 
				  	/* cleanup curl stuff */ 
				  	curl_easy_cleanup(curl_handle);
				 
				  	free(chunk.memory);
				 
				  	/* we're done with libcurl, so clean it up */ 
				  	curl_global_cleanup();
					conditionalVar2.notify_one();
				}	//end while
			}	//end while
		}
		
		string html;
		//mutex mu;
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

void output(vector<string> outstrings) {

	cout << "IN HERE" << endl;

	if(!(outstrings.empty())){
		ofstream outputFile;
		string result;
		ostringstream convert;
		convert << CSVCOUNT;
		result = convert.str();
		string filename = result + ".csv";
		outputFile.open(filename.c_str());

		/*string firstline = "";
		if (TIMECOUNT == 0){
			firstline = "Time,Phrase,Site,Count\n";
		}*/
		string firstline = "Time,Phrase,Site,Count\n";
		// string cnt = to_string(count);

		// string s = firstline + dt + "," + phrase + "," + site + "," + cnt + "\n"; 
		// cout << s << endl;
		outputFile << firstline;
		for(vector<string>::const_iterator i = outstrings.begin(); i != outstrings.end(); i++){
			outputFile << *i << endl;
		}

		unsigned int v2 = SEARCH_TERMS.size() * WEBSITES.size();
		unsigned int v1 = CREATE_OUTPUT;
		if (v1 == v2) outputFile.close();
	}
}

////////////////////////////////////////////////////////////////////////////////

void signalHandler(int value) {
	curl_global_cleanup();
	exit(0);
}

void setFlag(int value) {
	conditionalVar1.notify_all();
	alarm(30);
}

////////////////////////////////////////////////////////////////////////////////

void countApp(QueueClass<pair<string,string>> &bothData, QueueClass<string> &websitesData, mutex &mut) {
	vector<string> mystrings;
	while (1) {	
		cout << "counting" << endl;
		unique_lock<mutex> locker(mut);
		cout << "c" << endl;
		conditionalVar2.wait(locker);

	cout << "b" << endl;

		string currtime = getTimeDate();

		cout << "a" << endl;
		while (!bothData.empty()) {
			pair<string,string> p = bothData.queue_pop();
			//cout << p.first << p.second << endl;
			cout << "counting for " << p.second << endl;
			cout << "enter for" << endl;

			for (unsigned int i = 0; i < SEARCH_TERMS.size() - 1; i ++)
			{
				int position = 0;
				int inc = 0;
				unsigned int end = 0;
				position = p.first.find("<body", 0);
				end = p.first.find("</body>");
				while (p.first.find(SEARCH_TERMS[i], position) < end) {
					position = p.first.find(SEARCH_TERMS[i], position) + SEARCH_TERMS[i].size();
					inc ++;
					//cout << inc << "count" << endl;
				}	//end of while loop
				cout << "calling output" << endl;

				cout << SEARCH_TERMS[i] << " " << inc << endl;

				// output(currtime, SEARCH_TERMS[i], p.second, inc); // p.second is website, inc is count, search terms is word
				string count = to_string(inc); 
				string s = currtime + ", " + SEARCH_TERMS[i] + ", " + p.second + ", " + count;
				mystrings.push_back(s);
				cout << SEARCH_TERMS.size() << " " << i << "value when outputting" << endl;
			}
		}
		output(mystrings);
		CSVCOUNT+=1;
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
	Fetch fetchInstant;	
	
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
			if (!line.empty()) {
				WEBSITES.push_back(line);
				cout << "num sites " << WEBSITES.size() << line << endl;
				websitesData.queue_push(line);	
			}
		}	//end while loop
	}	//end if statement

	///cout << websitesData << endl;

	cout << "1" << endl;

//////////////////////// begin forming the threads /////////////////////////////
		
		signal(SIGINT, signalHandler);
		signal(SIGHUP, signalHandler);
	//	alarm(30);
		mutex mut;

		cout << "2" << endl;

		for (int i = 0; i < mainconfig.NUM_FETCH; i ++) {
			fetchThread.emplace_back(Fetch::sites, ref(bothData), ref(websitesData), ref(mut));
			cout << "3" << endl;	
		}

		cout << "4" << endl;

		for (int j = 0; j < mainconfig.NUM_PARSE; j ++) {
			parseThread.emplace_back(countApp, ref(bothData), ref(websitesData), ref(mut));
			cout << "5" << endl;	
		}

		cout << "6" << endl;
	/*
		for (int k = 0; k < mainconfig.NUM_FETCH; k ++) {
			cout << "6.5" << endl;
			fetchThread[k].join();
			cout << "7" << endl;	
		}	

		cout << "8" << endl;

		for (int l = 0; l < mainconfig.NUM_PARSE; l ++) {
			parseThread[l].join();
			cout << "9" << endl;
		}
	*/
		cout << "10" << endl;
	while (true) {
		//signal(SIGALRM, setFlag);
		conditionalVar1.notify_all();

		cout << "final while" << endl;		

		this_thread::sleep_for (std::chrono::seconds(5));
	}
	// close the file
	//outputFile.close();
	curl_global_cleanup();
	return 0;
}	//end of main function
