## Quick sort OpenMP

The program attached sorts a file of integers concurrently using OpenMP and stores the sorted integers into a new output file. The code uses quick sort algorithm to sort the integers. The package includes a makefile which complies code and also cleans up the binaries.

### Parallelization Strategies
- Quick sort (Open MP)

  The major steps involved in the quick sort algorithm is to partition the array recursively such that at each partition we have all elements less than pivot in one side and all greater than pivot in the right side. Multithreading can be used to parallelize quick sort by dividing the array into chunks, applying quick sort on each chunk and then merging all the chunks together parallelly.
  There are two places we apply multithreading. The first time we divide the array into chunks equal to the number of threads (retrieved by 𝑜𝑚𝑝_𝑔𝑒𝑡_𝑚𝑎𝑥_𝑡h𝑟𝑒𝑎𝑑𝑠()).  Few chunks may be uneven. An OpenMP parallel for is used to pass one chunk for each thread. Each thread performs quick sort for their chunk. All OpenMP threads wait once each chunk is quick sorter.
  The next multithreading is applied to combine these sorted array chunks. The combine is parallelized such that every 2 chunks are taken by an OpenMP thread and merged. The combined chunks are then further combined with the “combined” chunks from the other threads. Each stage of combine is parallelized using OpenMP.

### Compilation instructions

``` g++ main.cpp -o mysort -fopenmp ``` 


### Execution instructions
The code execution format as given below

``` ./mysort --name input.txt -o output.txt ```

