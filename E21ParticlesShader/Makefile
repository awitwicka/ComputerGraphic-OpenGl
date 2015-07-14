#---------------------------------------------------
# UNIX CIP-Pool
#---------------------------------------------------
CC          = g++
CCFLAGS     = -c
LIBS        = -L /usr/X11/lib
INCLUDES    = -I .
LDFLAGS     = -lglut -lGL -lGLU -lpng -lm

#---------------------------------------------------
# MacOSX
#---------------------------------------------------
#CC          = g++
#CCFLAGS     = -c
#LIBS        =
#INCLUDES    = -I .
#LDFLAGS     = -framework AGL -framework GLUT -framework OpenGL -framework Cocoa


#---------------------------------------------------
#
#---------------------------------------------------
OBJECTS     = mytime.o png.o E16.o 
BIN         = E16

all: $(BIN)
	./$(BIN)
	
$(BIN): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) $(LDFLAGS) -o $(BIN)


.c.o:
	$(CC) $(INCLUDES) $(CCFLAGS) $<

clean:
	rm -f $(OBJECTS) $(BIN)
