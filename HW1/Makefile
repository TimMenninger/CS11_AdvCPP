CC = g++
CPPFLAGS = -std=c++0x -g -Wall -pedantic
DEPS = rational.hh testbase.hh common.hh
OBJ = rational.o testbase.o test-rational.o

all: test-rational

again: clean all

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS) $(INC)

test-rational: $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS) $(INC)

clean :
	rm -rf test-rational *.o *.dSYM
