CC=g++
CFLAGS=-c -Wno-deprecated
LDFLAGS=-lexpat -lboost_thread\
	`pkg-config --cflags --libs pangocairo`\
	`pkg-config --cflags --libs sdl`
SOURCES=main.cpp\
	../core/AXR.cpp\
	$(shell find ../core/axr/ -name \*.cpp)\
	$(shell find ../core/hss -name \*.cpp)\
	$(shell find ../core/xml -name \*.cpp)\
	$(shell find ../core/os/linux -name \*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

ifndef ARCH
ARCH=64
endif

all: axr
clean: cleanup all

cleanup:
	find . -name \*.o -exec rm {} \;
	find ../core -name \*.o -exec rm {} \;

axr: axr_$(ARCH)

axr_$(ARCH): $(OBJECTS) cairosdl/cairosdl.o
	$(CC) -m$(ARCH) $(LDFLAGS) $(OBJECTS) cairosdl/cairosdl.o -o $@

%.o: %.cpp
	$(CC) -m$(ARCH) $(CFLAGS) $(LDFLAGS) -c $< -o $@

cairosdl/cairosdl.o:
	gcc -m$(ARCH) $(CFLAGS) $(LDFLAGS) -c cairosdl/cairosdl.c -o $@

