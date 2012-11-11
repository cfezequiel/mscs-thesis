#! /bin/Make

# Test Makefile for compiling programs with ARIA code

CXX=g++
TARGETS=simpleConnect

CFLAGS=-fPIC -g -Wall -fno-exceptions
ARIA_INCLUDE=-I/usr/local/Aria/include -I/usr/local/Aria/ArNetworking/include
ARIA_LINK=-L/usr/local/Aria/lib -lAria -lArNetworking -lpthread -ldl -lrt

all: $(TARGETS)

%: %.cpp
	$(CXX) $(CFLAGS) $(ARIA_INCLUDE) $< -o $@ $(ARIA_LINK)


.phony: clean

clean:
	rm -f $(TARGETS)

