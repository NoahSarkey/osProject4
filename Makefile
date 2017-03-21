site-tester: config.h site-tester.cpp

	g++ -Wall -o site-tester site-tester.cpp

clean:

	rm site-tester