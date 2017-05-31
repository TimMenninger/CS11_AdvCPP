CC = g++
CPPFLAGS = -std=c++0x -g -Wall -pedantic
DEPS = vector.hh testbase.hh

all: test-vector

again: clean all

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS) $(INC)

test-vector: test-vector.o testbase.o
	$(CC) -o $@ $^ $(CPPFLAGS) $(INC)

clean :
	rm -rf test test-vector *.o *.dSYM
