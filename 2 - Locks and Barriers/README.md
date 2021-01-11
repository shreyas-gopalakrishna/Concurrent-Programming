## Locks and Barriers

The two programs attached verifies the implementation of locks and barriers. The counter program increments a common counter by certain number of iterations in every thread provided as input. The sort program sorts a file of integers concurrently using bucket sort and stores the sorted integers into a new output file. In both the programs, the concurrently primitives – lock or barrier are chosen based on the user provided input. The package includes a makefile which complies code and also cleans up the binaries.

### Parallelization Strategies
- Locks

  The locks are implemented such that, an abstract class lock is created with two major methods, lock and unlock. Every sub class of the parent abstract class implements the lock and unlock method based on their respective algorithms. A total of 4 locks – test and set, test and test and set, ticket and mcs locks are implemented.
  Counter – for the counter, each thread chooses the appropriate lock and locks before incrementing the counter and then unlocks after the increment.
  Bucket sort – for the array is divided into chunks equal to the number of threads. And each thread adds its elements into the bucket. An array of locks are chosen equal to the number of buckets for threads to access each bucket based on their lock.

- Barriers

  The barrier is implemented using a class sense is defined which handles the wait function of the barrier.
  Counter – for counter, every thread runs 𝑛𝑢𝑚𝑏𝑒𝑟 𝑡h𝑟𝑒𝑎𝑑𝑠 ∗ 𝑖𝑡𝑒𝑟𝑎𝑡𝑖𝑜𝑛𝑠 times and chooses to increment the counter based on the thread id. Once incremented, it waits for the other threads to also finish their first increment. Then all threads take turn for next round of increments.
  Bucket sort – for the bucket sort implementation, the input is formatted such that every thread receives equal number of elements to add to bucket. Since the number of elements to add to bucket is the same, each thread take turn just like counter to add to the bucket one at a time. After adding, it waits for other threads to add its first element. Once every thread adds its first element, the next element is added and waits. This continues until all elements are added to buckets. Then the buckets are combined to obtain the sorted elements.

### Compilation instructions

``` g++ main_counter.cpp locks.cpp -o counter -pthread ``` 

``` g++ main_sort.cpp locks.cpp -o mysort -pthread ```

### Execution instructions
The code execution format as given below

``` ./counter --name -t NUM_THREADS -i=NUM_ITERATIONS --bar=<sense,pthread> -- lock=<tas,ttas,ticket,mcs,pthread> -o output.txt ```

``` ./mysort --name input.txt -o output.txt -t NUM_THREADS --alg=bucket --bar=<sense,pthread> -- lock=<tas,ttas,ticket,mcs,pthread> ```
