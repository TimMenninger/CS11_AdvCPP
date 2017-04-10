CC = g++
CPPFLAGS = -std=c++0x -g -Wall -pedantic
DEPS = vector.hh testbase.h

all: test1 test2 test3 test-vector

again: clean all

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS) $(INC)

test1: test1.o
	$(CC) -o $@ $^ $(CPPFLAGS) $(INC)

test2: test2.o
	$(CC) -o $@ $^ $(CPPFLAGS) $(INC)

test3: test3.o
	$(CC) -o $@ $^ $(CPPFLAGS) $(INC)

test-vector: test-vector.o testbase.o
	$(CC) -o $@ $^ $(CPPFLAGS) $(INC)

clean :
	rm -rf test1 test2 test3 test-vector *.o *.dSYM
