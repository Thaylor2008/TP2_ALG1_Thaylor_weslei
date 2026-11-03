CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic

.PHONY: all test test-guloso test-divconq test-completo clean

all: tp02-guloso tp02-divconq

tp02-guloso: src/tp02_guloso.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

tp02-divconq: src/tp02_divconq.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

test: test-guloso test-divconq

test-guloso: tp02-guloso
	./test.sh ./tp02-guloso 20

test-divconq: tp02-divconq
	./test.sh ./tp02-divconq 31

test-completo: all
	./test.sh ./tp02-guloso 31
	./test.sh ./tp02-divconq 31

clean:
	rm -f tp02-guloso tp02-divconq
