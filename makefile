all: write

write: write.cc data.o
	c++ -o write -std=c++11 write.cc data.o

data.o: data.h data.cc particle.h
	c++ -c -std=c++11 data.cc

clean:
	rm data.o write
