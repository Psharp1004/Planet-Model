CC=g++
CFLAGS=-c -Wall
LDFLAGS = -lGL -lGLU -lglut
EXECUTABLE=asgfinal

ifeq "$(OS)" "Windows_NT"
	LDFLAGS =-lfreeglut -lglu32 -lopengl32
	EXECUTABLE=asgfinal.exe
endif

OS := $(shell uname)
ifeq ($(OS), Darwin)
    LDFLAGS=-framework Carbon -framework OpenGL -framework GLUT
endif


SOURCES=main.cpp asgfinal.cpp vector3f.cpp point.cpp planet.cpp advcam.cpp
OBJECTS=$(SOURCES:.cpp=.o)


all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm *.o
	rm *.exe

run: $(EXECUTABLE)

	./$(EXECUTABLE)$(EXEXT)


