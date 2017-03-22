// fetch.h
// Authors: Sam Mustipher, Noah Sarkey

#include <stdio.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <curl/curl.h>
#include <fstream>
using namespace std;

class Fetch{
	public:
		Fetch(string sites){
			string line;
			ifstream inFile;
			inFile.open(sites.c_str());
			cout << "In fetch" << endl;
			while(getline(inFile, line)){
				// parsing the file line by line
				istringstream iss(line);
				string text;
				if(!(iss >> text)) { break; } // error
				string url;
				string delimiter = "http://";
				url = text.substr(text.find(delimiter)+delimiter.length());
				cout << url << ":" << endl;

				// start curl
				CURL *curl;
				CURLcode res;
				cout << "Outside of curl" << endl;
				curl = curl_easy_init();
				if(curl){
					curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
					curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
					curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);
					res = curl_easy_perform(curl);
					curl_easy_cleanup(curl);

					cout << html << endl;			
				}
			}
			inFile.close();
		}

		// shttp://stackoverflow.com/questions/9786150/save-curl-content-result-into-a-string-in-c
		static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
			((string *) userp)->append((char *)contents, size*nmemb);
			return size*nmemb; 
		}

		string html;
};
