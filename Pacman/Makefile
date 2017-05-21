CXX = g++
CXXFLAGS = -std=c++14 -g -Wall -pedantic

INCLUDE = -I/usr/include -Ilib
LIBDIR = -L/usr/local/lib -L/usr/lib/x86_64-linux-gnu
LIBS = -lm -lncurses

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
DATA_DIR = data

# There should be an object file for each cpp file you create.
RUN_GAME_DEP =	$(OBJ_DIR)/game.o
RUN_GAME_DEP +=	$(OBJ_DIR)/display.o
RUN_GAME_DEP +=	$(OBJ_DIR)/map.o
RUN_GAME_DEP +=	$(OBJ_DIR)/main.o

RUN_GAME_EXE =	$(BIN_DIR)/pacman


TARGETS = $(RUN_GAME_EXE)

all: $(TARGETS)

$(RUN_GAME_EXE): $(RUN_GAME_DEP)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@ $(LIBDIR) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@ $(LIBS)

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGETS)

again: clean all

.PHONY: all clean
