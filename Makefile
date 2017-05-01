.c.o:
	g++ -std=c++14 -g -c $?

# compile client and server
all: Driver

# compile server program
Driver: Driver.o DirectMappedCache.o SetAssociativeCache.o FullyAssociativeCache.o Tree.o
	g++ -std=c++14 -g Tree.o FullyAssociativeCache.o SetAssociativeCache.o DirectMappedCache.o Driver.o -o Driver

Driver.o:	Driver.cpp
	g++ -std=c++14 -g -c Driver.cpp

Cache.o: Cache.h
	g++ -std=c++14 -g -c Cache.h

DirectMappedCache.o: DirectMappedCache.cpp
	g++ -std=c++14 -g -c DirectMappedCache.cpp

SetAssociativeCache.o: SetAssociativeCache.cpp
	g++ -std=c++14 -g -c SetAssociativeCache.cpp

FullyAssociativeCache.o: FullyAssociativeCache.cpp
	g++ -std=c++14 -g -c FullyAssociativeCache.cpp

Tree.o: Tree.cpp
	g++ -std=c++14 -g -c Tree.cpp

clean:
	rm -f *.o Driver
