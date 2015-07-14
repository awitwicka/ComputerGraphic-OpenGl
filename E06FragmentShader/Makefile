# Makefile for simple Cg-Shaders
# by Daniel Jungblut, IWR Heidelberg, February 2008
# revised by Susanne Kroemker, IWR Heidelberg, April 2014


CPP  = g++
CC   = gcc

RES  = 
OBJ  = main.o $(RES)
LINKOBJ  = $(OBJ)

LDFLAGS     = -lglut -lGL -lm -lGLU -lCg -lCgGL -lpthread
#LDFLAGS     = -framework AGL -framework GLUT -framework OpenGL -framework Cocoa -lm -framework Cg -lpthread

BIN  = E06
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











