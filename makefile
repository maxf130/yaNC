all: write read propagate InitCond

InitCond: InitCond.cc snapshot.o snapshot.h particle.h
	c++ -o InitCond -std=c++11 InitCond.cc snapshot.o

read: read.cc snapshot.o
	c++ -o read -std=c++11 read.cc snapshot.o

write: write.cc snapshot.o
	c++ -o write -std=c++11 write.cc snapshot.o

snapshot.o: snapshot.h snapshot.cc particle.h
	c++ -c -std=c++11 snapshot.cc

integrator.o: snapshot.h integrator.h integrator.cc
	c++ -c -std=c++11 integrator.cc

propagate: propagate.cc propagate.h snapshot.o snapshot.h particle.h integrator.o
	c++ -o propagate -std=c++11 propagate.cc snapshot.o integrator.o

clean:
	rm snapshot.o integrator.o propagate write read InitCond
