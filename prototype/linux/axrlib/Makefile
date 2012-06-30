CC=g++
CFLAGS=-c -Wno-deprecated
LDFLAGS=-fPIC -lexpat -lboost_thread\
	`pkg-config --cflags --libs pangocairo`\
	`pkg-config --cflags --libs sdl`
SOURCES=../../core/AXR.cpp\
	$(shell find ../../core/axr/ -name \*.cpp)\
	$(shell find ../../core/hss -name \*.cpp)\
	$(shell find ../../core/xml -name \*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

ifndef ARCH
ARCH=64
endif

ifdef DEBUG
LDFLAGS+= -g
endif

all: axr
clean: cleanup all

cleanup:
	find . -name \*.so -exec rm {} \;
	find . -name \*.o -exec rm {} \;
	find ../../core -name \*.o -exec rm {} \;

axr: axrlib_$(ARCH).so

axrlib_$(ARCH).so: $(OBJECTS)
	$(CXX) $(LDFLAGS) -shared -Wl,-soname,$@ -o $@ $(OBJECTS)
	sudo cp $@ /usr/lib

%.o: %.cpp
	$(CC) -m$(ARCH) $(CFLAGS) $(LDFLAGS) -c $< -o $@

