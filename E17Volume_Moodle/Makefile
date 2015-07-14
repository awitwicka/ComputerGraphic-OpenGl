CC          = g++
CCFLAGS     = -Wno-deprecated
#Ubuntu 64bit
INCLUDES    = -I/usr/include/vtk-5.8
LIBPATH     = -L/usr/lib/vtk-5.8
#Fedora 64bit
#INCLUDES    = -I/usr/include/vtk
#LIBPATH     = -L/usr/lib64/vtk
LIBS        = -lvtkCommon -lvtkRendering -lvtkIO -lvtkGraphics -lvtkFiltering -lvtkVolumeRendering -lvtkImaging

BIN       = E17Volume

$(BIN): E17Volume.cpp Makefile
	$(CC) E17Volume.cpp $(INCLUDES) $(LIBPATH) $(LIBS) $(CCFLAGS) -o $(BIN)

clean:
	rm -f *.o $(BIN)

