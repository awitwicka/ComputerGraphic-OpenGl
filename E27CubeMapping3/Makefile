# Makefile for simple Cg-Shaders
# by Daniel Jungblut, IWR Heidelberg, February 2008


CPP  = g++
CC   = gcc

RES  = 
OBJ  = main.o png.o $(RES)
LINKOBJ  = $(OBJ)

LDFLAGS     = -lglut -lGL -lm -lGLU -lCg -lCgGL -lpthread -lpng

BIN  = E24
CXXFLAGS = $(CXXINCS)  
CFLAGS = $(INCS)  
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) $(LDFLAGS) -o $(BIN) $(LIBS)
	./$(BIN)

main.o: main.cpp 
	$(CPP) -c main.cpp -o main.o $(CXXFLAGS)

png.o: png.cpp
	$(CPP) -c png.cpp -o png.o $(CXXFLAGS)












