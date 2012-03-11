CC=g++
CFLAGS=-c -Wno-deprecated
LDFLAGS=-lexpat\
	`pkg-config --cflags --libs pangocairo`\
	`pkg-config --cflags --libs sdl`
SOURCES=main.cpp\
	cairosdl/cairosdl.c\
	../core/AXR.cpp\
	$(shell find ../core/axr/ -name \*.cpp)\
	$(shell find ../core/hss -name \*.cpp)\
	$(shell find ../core/xml -name \*.cpp)\
	$(shell find ../core/os/linux -name \*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=axr

all: $(EXECUTABLE)
clean: cleanup all

cleanup:
	find . -name \*.o -exec rm {} \;
	find ../core -name \*.o -exec rm {} \;

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

