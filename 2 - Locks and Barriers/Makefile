CFLAGS = -Wall -g -std=c++11
CXX = g++

all: counter

counter: mysort
	$(CXX) main_counter.cpp locks.cpp -o counter -pthread $(CFLAGS)

mysort:
	$(CXX) main_sort.cpp locks.cpp -o mysort -pthread $(CFLAGS)

clean:
	rm counter
	rm mysort