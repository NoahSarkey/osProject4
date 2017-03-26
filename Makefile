site-tester: site-tester.o
	g++ -std=c++0x -g -Wall -lcurl site-tester.o -o site-tester
site-tester.o: site-tester.cpp config.h
	g++ -std=c++0x -g -Wall -c site-tester.cpp -o site-tester.o
clean:
	rm -f *.o site-tester
