#---------------------------------------------------
# MacOSX
#---------------------------------------------------
#CC          = g++
#CCFLAGS     = -c
#LIBS        =
#INCLUDES    = -I.
#LDFLAGS     = -framework AGL -framework GLUT -framework OpenGL -framework Cocoa -lm

#---------------------------------------------------
# UNIX CIP-Pool
#---------------------------------------------------
CC          = g++
CCFLAGS     = -Wall -pedantic -c
LIBS        = -L/usr/X11/lib -L/usr/local/lib
INCLUDES    = -I/usr/local/include
LDFLAGS     = -lglut -lGL -lGLU -lm

#---------------------------------------------------
#
#---------------------------------------------------

MYOBJECTS   = photon_map_opengl.o vector.o photons.o
BIN         = photon_map_opengl

$(BIN): $(MYOBJECTS) Makefile
	$(CC) $(MYOBJECTS) $(LIBS) $(LDFLAGS)  -o $(BIN)
	./$(BIN)

.cpp.o:
	$(CC) $(INCLUDES) $(CCFLAGS) $<

clean:
	rm -f *.o *.core $(BIN)


