# Makefile
OPENBLAS = -lopenblas
BOOST = -lboost_program_options -lboost_system
MKL = -fopenmp -m64 -I${MKLROOT}/include -Wl,--no-as-needed -L${MKLROOT}/lib/intel64 -lmkl_intel_lp64 -lmkl_core -lmkl_gnu_thread -lpthread -lm -ldl
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

all: $(EDIR)/main $(EDIR)/evaluator $(EDIR)/matgen $(EDIR)/mean $(EDIR)/lenet

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(OPENBLAS) $(BOOST) $(MKL)

$(EDIR)/main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(OPENBLAS) $(BOOST) $(MKL)

# Evaulator
$(ODIR)/evaluator.o: $(SDIR)/evaluator.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(OPENBLAS) $(BOOST) $(MKL)

$(EDIR)/evaluator: $(ODIR)/evaluator.o
	$(CC) -o $@ $^ $(CFLAGS) $(OPENBLAS) $(BOOST) $(MKL)

# Matrix Generator
$(ODIR)/matgen.o: $(SDIR)/matgen.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(OPENBLAS) $(BOOST) $(MKL)

$(EDIR)/matgen: $(ODIR)/matgen.o
	$(CC) -o $@ $^ $(CFLAGS) $(OPENBLAS) $(BOOST) $(MKL)

# Matrix Generator
$(ODIR)/mean.o: $(SDIR)/mean.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(OPENBLAS) $(BOOST) $(MKL)

$(EDIR)/mean: $(ODIR)/mean.o
	$(CC) -o $@ $^ $(CFLAGS) $(OPENBLAS) $(BOOST) $(MKL)

# lenet
$(ODIR)/lenet.o: $(SDIR)/lenet.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(OPENBLAS) $(BOOST) $(MKL)

$(EDIR)/lenet: $(ODIR)/lenet.o
	$(CC) -o $@ $^ $(CFLAGS) $(OPENBLAS) $(BOOST) $(MKL)


.PHONY: clean


clean:
	rm -f $(ODIR)/*.o $(EDIR)/*