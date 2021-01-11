## Quick sort and Bucket sort - Fork Join

The program attached sorts a file of integers concurrently and stores the sorted integers into a new output file. The code uses quick sort and bucket sort algorithms based on the input provided. The package includes a makefile which complies code and also cleans up the binaries.

### Parallelization Strategies
- Quick sort (fork-join)

  The major steps involved in the quick sort algorithm is to partition the array recursively such that at each partition we have all elements less than pivot in one side and all greater than pivot in the right side. Multithreading can be used to parallelize quick sort by dividing the array into chunks, applying quick sort on each chunk and then merging all the chunks together parallelly.
  There are two places we apply multithreading. The first time we divide the array into chunks equal to the number of threads. Few chunks may be uneven. We fork and pass one chunk for each thread. Each thread performs quick sort for their chunk. Main thread handles one chunk. We join and wait for all the threads to finish their sorting.
  The next multithreading is applied to combine these sorted array chunks. The combine is parallelized such that every 2 chunks are taken by a thread and merged. The combined chunks are then further combined with the “combined” chunks from the other threads.

- Bucket sort (fork-join with lock)

  The steps involved in the bucket sort algorithm is to add elements in the array to different buckets such that each bucked is sorted. The sorted individual buckets are then combined to obtain the fully sorted array. Multithreading can be used to parallelize bucket sort by dividing the array into chunks, and parallelly adding the elements into their individual buckets and then finally merging all the buckets.
  The algorithm provided in the question is used to implement bucket sort. Each bucket is a priority queue so that the elements added to them are sorted. An array of priority queues is used to handle all the buckets. The sort is parallelized such that we divide the array into chunks equal to the number of threads. Few chunks may be uneven. We fork and pass one chunk for each thread. Each thread adds the element in it to the respective bucket.
  Concurrency while adding is handled using an array of locks. The number of locks is equal to the number of buckets. Any thread adding to a particular bucket acquires the lock adds the element and then releases the lock. Main thread also handles one chunk of the array. Finally, the buckets are combined to obtain the fully sorted array.

### Compilation instructions
``` g++ main.cpp bucket_sort.cpp quick_sort.cpp -o mysort ```

### Execution instructions
The code execution format as given below
``` ./mysort [--name] <sourcefile.txt> -o <outfile.txt> -t num_threads --alg=<fjmerge,fjquick,lkbucket> ```

Only using the name option will print my name. Running it without the name option but rest of them will sort and output the file. Using all the options will print my name as well as ouput a sorted file.
Examples:

``` ./mysort [--name] ```

``` ./mysort [--name] numbers.txt -o ouput.txt -t 1 --alg=fjquick ``` 

``` ./mysort numbers.txt -o ouput.txt -t 3 --alg=lkbucket ```
