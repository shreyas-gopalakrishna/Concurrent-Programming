CFLAGS = -Wall -g -std=c++11
CXX = g++

all: sort

sort:
	$(CXX) main.cpp quick_sort.cpp bucket_sort.cpp -o mysort -pthread $(CFLAGS)

clean:
	rm mysort