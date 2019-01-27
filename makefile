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
OPENBLAS = -lopenblas
BOOST = -lboost_program_options -lboost_system
MKL = -fopenmp -m64 -I${MKLROOT}/include -Wl,--no-as-needed -L${MKLROOT}/lib/intel64 -lmkl_intel_lp64 -lmkl_core -lmkl_gnu_thread -lpthread -lm -ldl
IDIR = ./include
CC = g++
CFLAGS = -I$(IDIR) -std=c++11 $(OPENBLAS) $(BOOST) $(MKL)

ODIR = ./build
EDIR = ./bin
SDIR = ./src

_DEPS = imgOp.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o imgOp.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: $(EDIR)/main $(EDIR)/evaluator $(EDIR)/matgen $(EDIR)/mean

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EDIR)/main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# Evaulator
$(ODIR)/evaluator.o: $(SDIR)/evaluator.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(EDIR)/evaluator: $(ODIR)/evaluator.o
	$(CC) -o $@ $^ $(CFLAGS)

# Matrix Generator
$(ODIR)/matgen.o: $(SDIR)/matgen.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(EDIR)/matgen: $(ODIR)/matgen.o
	$(CC) -o $@ $^ $(CFLAGS)

# Matrix Generator
$(ODIR)/mean.o: $(SDIR)/mean.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(EDIR)/mean: $(ODIR)/mean.o
	$(CC) -o $@ $^ $(CFLAGS)
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(EDIR)/*