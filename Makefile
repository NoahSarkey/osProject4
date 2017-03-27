CPP=/afs/nd.edu/user14/csesoft/new/bin/g++
CPPFLAGS= -Wall -std=c++11
LD=/afs/nd.edu/user14/csesoft/new/bin/g++
LDFLAGS= -lcurl -static-libstdc++

site-tester: site-tester.o
	$(LD) $(LDFLAGS) site-tester.o -o site-tester

%.o: %.cpp
	$(CPP) $(CPPFLAGS) $^ -c -o $@

clean:
	rm site-tester *.o
