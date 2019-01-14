# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -Wall -g

# ****************************************************
# Targets needed to bring the executable up to date

main: main.o imgOp.o
	$(CXX) $(CXXFLAGS) -o main main.o imgOp.o

# The main.o target can be written more simply

main.o: main.cpp imgOp.h
	$(CXX) $(CXXFLAGS) -c main.cpp

imgOp.o: imgOp.h