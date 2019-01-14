# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -Wall -g

# ****************************************************
# Targets needed to bring the executable up to date

main: main.o conv.o
	$(CXX) $(CXXFLAGS) -o main main.o conv.o

# The main.o target can be written more simply

main.o: main.cpp conv.h
	$(CXX) $(CXXFLAGS) -c main.cpp

conv.o: conv.h