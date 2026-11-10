CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra

TP_SOURCES := $(filter-out $(wildcard pruebas*.cpp),$(wildcard *.cpp))
TEST_SOURCES := $(filter-out main.cpp,$(wildcard *.cpp))

.PHONY: all run test clean

all: tp

tp: $(TP_SOURCES)
	$(CXX) $(CXXFLAGS) -o tp $(TP_SOURCES)

pruebas: $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) -o pruebas $(TEST_SOURCES)

run: tp
	./tp

test: pruebas
	./pruebas

clean:
	rm -f tp pruebas *.o
