CXX = g++
CXXFLAGS = -std=c++11 -I/usr/lib/x86_64-linux-gnu/ -I/home/ubuntu/workspace/DateTime/Code/
INCLUDES = 
LD = icc
LDFLAGS := -L/usr/lib/x86_64-linux-gnu/ -lboost_filesystem -lboost_program_options -lboost_system -L/home/ubuntu/workspace/DateTime/Code/

SOURCES = perfmetrics.cpp
HEADERS = perfmetrics.h 
OBJ     = main.o perfmetrics.o

.SUFFIXES: .cpp

all: perf

perf: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o perf $(LDFLAGS)
	
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp
	
perfmetrics.o: perfmetrics.cpp perfmetrics.h
	$(CXX) $(CXXFLAGS) -c perfmetrics.cpp

clean:
	rm -f main
	rm -f *.o
	rm -f *.i
	rm -f *.lst
	rm -f output.*