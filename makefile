all: write read propagate InitCond test

InitCond: InitCond.cc snapshot.o snapshot.h particle.h
	c++ -o InitCond -std=c++11 InitCond.cc snapshot.o -Wall -pedantic

read: read.cc snapshot.o
	c++ -o read -std=c++11 read.cc snapshot.o -Wall -pedantic

write: write.cc snapshot.o
	c++ -o write -std=c++11 write.cc snapshot.o -Wall -pedantic

snapshot.o: snapshot.h snapshot.cc particle.h
	c++ -c -std=c++11 snapshot.cc -Wall -pedantic

integrator.o: snapshot.h integrator.h integrator.cc
	c++ -c -std=c++11 integrator.cc -Wall -pedantic

propagate: propagate.cc propagate.h snapshot.o snapshot.h particle.h integrator.o
	c++ -o propagate -std=c++11 propagate.cc snapshot.o integrator.o -Wall -pedantic

test: test.h test.cc snapshot.o particle.h integrator.o
	c++ -o test -std=c++11 test.cc snapshot.o integrator.o -Wall -pedantic

clean:
	rm snapshot.o integrator.o propagate write read InitCond test
