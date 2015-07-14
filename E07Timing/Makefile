#---------------------------------------------------
#
# Makefile
#
# for simple openGl programs
# with GLUT-library for X-window and event handling
#
#---------------------------------------------------
CC          = g++
#CFLAGS     = -c -Wall -g
CFLAGS      = -c

# for Linux use the standard pathes to the libs
#LDFLAGS     = -lglut -lGL -lGLU -lm
# for MacOSX use the framework
LDFLAGS    = -lGL -lGLU -lglut


MYEXECUTABLE = E01
MYOBJECTS   = main.o my_time.o

$(MYEXECUTABLE): $(MYOBJECTS) Makefile
	$(CC) $(MYOBJECTS) $(LDFLAGS)  -o $(MYEXECUTABLE)
	strip $(MYEXECUTABLE)
	./$(MYEXECUTABLE)

run: $(MYEXECUTABLE)
	./$(MYEXECUTABLE)

.c.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f $(MYOBJECTS) $(MYEXECUTABLE) *~


.PHONY: run clean
