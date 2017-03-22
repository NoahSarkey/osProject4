site-tester: config.h fetch.h site-tester.cpp

	g++ -Wall -o site-tester site-tester.cpp -lcurl

clean:

	rm site-tester
