# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation

# CXX = g++
# CXXFLAGS = -Wall -g

# ****************************************************
# Targets needed to bring the executable up to date

# main: main.o imgOp.o
	# $(CXX) $(CXXFLAGS) -o main main.o imgOp.o

# The main.o target can be written more simply

# main.o: main.cpp imgOp.h
# 	$(CXX) $(CXXFLAGS) -c main.cpp

# imgOp.o: imgOp.h

IDIR = ./include
CC = g++
CFLAGS = -I$(IDIR) -std=c++11

ODIR = ./build
EDIR = ./bin
SDIR = ./src

_DEPS = imgOp.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o imgOp.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EDIR)/main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(EDIR)/*