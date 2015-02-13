all: write read propagate.o

read: read.cc data.o
	c++ -o read -std=c++11 read.cc data.o

write: write.cc data.o
	c++ -o write -std=c++11 write.cc data.o

data.o: data.h data.cc particle.h
	c++ -c -std=c++11 data.cc

propagate.o: propagate.cc propagate.h data.h particle.h
	c++ -c -std=c++11 data.cc

clean:
	rm data.o propagate.o write read
