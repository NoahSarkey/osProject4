// config.h
// Authors: Sam Mustipher, Noah Sarkey
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
using namespace std;

class Config{
	public:
		Config(string configtext){
			// initialize default values
			PERIOD_FETCH = 180;
			pnd["PERIOD_FETCH"] = PERIOD_FETCH;
			NUM_FETCH = 1;
			pnd["NUM_FETCH"] = NUM_FETCH;
			NUM_PARSE = 1;
			pnd["NUM_PARSE"] = NUM_PARSE;
			SEARCH_FILE = "Search.txt";
			ss["SEARCH_FILE"] = SEARCH_FILE;
			SITE_FILE = "Sites.txt";
			ss["SITE_FILE"] = SITE_FILE;

			string line;

			ifstream inFile;
			inFile.open(configtext);

			while(getline(inFile, line)){
				istringstream iss(line);
				string text;
				if(!(iss >> text)) { break; } // error
				string param;
				string data;
				string delimiter = "=";
				param = text.substr(0,text.find(delimiter));
				data = text.substr(text.find(delimiter)+delimiter.length());
				// checking the parameters, could make switch
				if(param == "PERIOD_FETCH"){
					pnd["PERIOD_FETCH"] = atoi(data.c_str());
				}
				else if(param == "NUM_FETCH"){
					pnd["NUM_FETCH"] = atoi(data.c_str());
				}
				else if(param == "NUM_PARSE"){
					pnd["NUM_PARSE"] = atoi(data.c_str());
				}
				else if(param == "SEARCH_FILE"){
					ss["SEARCH_FILE"] = data;
				}
				else if(param == "SITE_FILE"){
					ss["SITE_FILE"] = data;
				}
				else{ cout << "What the fuck is that?" << endl; }
			}
		}

		void output(){
			// printing out elements of a map
			map<string,int>::iterator it;
			for(it = pnd.begin(); it != pnd.end(); it++){
				cout << it->first << " = " << it->second << endl;
			}

			// printing out search and site file
			map<string,string>::iterator st;
			for(st = ss.begin(); st != ss.end(); st++){
				cout << st->first << " = " << st->second << endl;
			}
		}

		// member variables
		int PERIOD_FETCH;
		int NUM_FETCH;
		int NUM_PARSE;
		string SEARCH_FILE;
		string SITE_FILE;
		map<string,int> pnd; // parameters and data
		map<string, string> ss; // search and sitefile

};