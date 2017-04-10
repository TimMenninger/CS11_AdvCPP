CC = g++
CPPFLAGS = -std=c++0x -g -Wall -pedantic
DEPS = vector.hh testbase.h
OBJ = test1.o test-vector.o testbase.o

all: test1 test-vector

again: clean all

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS) $(INC)

test1: test1.o
	$(CC) -o $@ $^ $(CPPFLAGS) $(INC)

test-vector: test-vector.o testbase.o
	$(CC) -o $@ $^ $(CPPFLAGS) $(INC)

clean :
	rm -rf test1 *.o *.dSYM
