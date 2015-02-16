all: write read propagate.o

read: read.cc snapshot.o
	c++ -o read -std=c++11 read.cc snapshot.o

write: write.cc snapshot.o
	c++ -o write -std=c++11 write.cc snapshot.o

snapshot.o: snapshot.h snapshot.cc particle.h
	c++ -c -std=c++11 snapshot.cc

propagate.o: propagate.cc propagate.h snapshot.h particle.h
	c++ -c -std=c++11 propagate.cc

clean:
	rm snapshot.o propagate.o write read
