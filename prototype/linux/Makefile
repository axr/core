ifndef ARCH
ARCH=64
endif

CC=g++
CFLAGS=-c -Wno-deprecated -shared
LDFLAGS=-Wl,-rpath,./ -lexpat -lboost_thread\
	`pkg-config --cflags --libs pangocairo`\
	`pkg-config --cflags --libs sdl`
SOURCES=main.cpp\
	$(shell find ../core/os/linux -name \*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)\
	libaxr/libaxr.so

ifdef DEBUG
LDFLAGS+= -g
endif

all: axr
clean: cleanup all

cleanup:
	find . -name \*.o -exec rm {} \;
	find ../core/os/linux/ -name \*.o -exec rm {} \;

axr: axr_$(ARCH)

axr_$(ARCH): $(OBJECTS) cairosdl/cairosdl.o
	$(CC) -m$(ARCH) $(LDFLAGS) $(OBJECTS) cairosdl/cairosdl.o -o $@

%.o: %.cpp
	$(CC) -m$(ARCH) $(CFLAGS) $(LDFLAGS) -c $< -o $@

cairosdl/cairosdl.o:
	gcc -m$(ARCH) $(CFLAGS) $(LDFLAGS) -c cairosdl/cairosdl.c -o $@

