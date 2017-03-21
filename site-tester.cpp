// Sam Mustipher, Noah Sarkey
// Project 04
// CSE 30341 - 02

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include "config.h"
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
	Config mainconfig((configfile));
	mainconfig.output();
	
	return 0;
}	//end of main function

