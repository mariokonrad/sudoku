# Makefile

#CXXFLAGS+=-Wall -ansi -pedantic -ggdb -O2 -fomit-frame-pointer -funroll-loops
CXXFLAGS+=-Wall -ansi -pedantic -ggdb

.PHONY: all
all: sudokusolv solver

sudokusolv : sudokusolv.o
	$(CXX) -o $@ $^

solver : solver.o
	$(CXX) -o $@ $^ `wx-config --libs`

solver.o : solver.cpp
	$(CXX) -Wall -ggdb -O2 -fomit-frame-pointer -funroll-loops -o $@ -c $< `wx-config --cxxflags`

.PHONY: clean
clean :
	rm -f *.exe *.stackdump *.o *.back *~ solver sudokusolv

%.o%.cpp :
	$(CXX) $(CXXFLAGS) -o $@ -c $^

# EOF
