Sam Mustipher, Noah Sarkey
CSE 30341-02 Project4

**How the System Works:**
When you run make the program will compile all of the files so that all you have
to do is run ./site-tester.cpp with the given configuration file. The configuration
file will be parsed in config.h to be sent to all of the other files for use. The 
output from site-tester users a vector of strings for each of the different threads 
in order to print out the given output file. 

If we encounter an invalid item within the Config.txt file, we will throw an error
and not run the program. The error must be corrected before the program runs.

Errors will be output to the screen if we run into issues with certain links, such
as taking too long to load or an invalid link altogether.


**Parameters:**
Config.h:
	NUM_FETCH: number of fetch threads
	NUM_PARSE: number of parsing threads
	SEARCH_FILE: file containing the search things
	SITE_FILE: file containing the sites to query

site-tester.cpp:
	CSV_COUNT = running count of the csv file number


**For the grader:**
We both completed the 1pt in-class extra credit. In addition, we implemented the
5pt extra credit within the project to make our code HTML aware (must see EC2 for
details, does not automatically run as HTML aware). Please see EC2.txt for a 
further explanation.
