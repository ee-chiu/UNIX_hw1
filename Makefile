.PHONY: all, clean
CXX = g++
CFLAGS = -Wall -g

PROGS = hw1

all: ${PROGS}

hw1: hw1.cpp with_perror.h without_perror.h
	$(CXX) -o $@ $(CFLAGS) $<

clean:
	rm -f *~ $(PROGS)