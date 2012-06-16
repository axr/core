ARCH=64
PKG_CONFIG_PATH=/usr/lib/pkgconfig
#ARCH=32
#PKG_CONFIG_PATH=/usr/lib32/pkgconfig
CC=g++
CFLAGS=-m$(ARCH) -c -Wno-deprecated
LDFLAGS=-m$(ARCH) -lexpat -lboost_thread\
	`PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags --libs pangocairo`\
	`PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags --libs sdl`
SOURCES=main.cpp\
	../core/AXR.cpp\
	$(shell find ../core/axr/ -name \*.cpp)\
	$(shell find ../core/hss -name \*.cpp)\
	$(shell find ../core/xml -name \*.cpp)\
	$(shell find ../core/os/linux -name \*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=axr_$(ARCH)

all: $(EXECUTABLE)
clean: cleanup all

cleanup:
	find . -name \*.o -exec rm {} \;
	find ../core -name \*.o -exec rm {} \;

$(EXECUTABLE): $(OBJECTS) cairosdl/cairosdl.o
	$(CC) $(LDFLAGS) $(OBJECTS) cairosdl/cairosdl.o -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

cairosdl/cairosdl.o:
	gcc $(CFLAGS) $(LDFLAGS) -c cairosdl/cairosdl.c -o $@

