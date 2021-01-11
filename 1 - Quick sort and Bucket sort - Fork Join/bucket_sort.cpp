using namespace std;

#include<algorithm>
#include "bucket_sort.h"
#include "common.h"

pthread_barrier_t bar1;

int num_of_buckets;
int maximum;
vector<mutex> mtx;
vector<priority_queue<int, vector<int>, greater<int>>> all_buckets;

/**
    Sorts a vector of integers using the bucket sort algorithm
    @param struct indices_data which gives the start, end indices
*/
void *bucket_sort(void *input){
    // local init - start
	struct indices_data *data = (struct indices_data*)input;
    int start = data->start;
    int end = data->end;
    int tid = data->thread_id;
    // local init - end

    // algo - start
    pthread_barrier_wait(&bar1);
    if(tid==0){
		clock_gettime(CLOCK_MONOTONIC,&start_time);
	}
    for(int i = start; i <= end; i++ ){
        int bucket_number = floor(num_of_buckets * numbers[i] / maximum);
        if(bucket_number >= num_of_buckets){
            bucket_number = num_of_buckets - 1;
        }
        mtx[bucket_number].lock();
        all_buckets[bucket_number].push(numbers[i]);
        mtx[bucket_number].unlock();
    }

    pthread_barrier_wait(&bar1);
	if(tid==0){
		clock_gettime(CLOCK_MONOTONIC,&end_time);
	}
    // algo - end
    return NULL;
}

/**
    Perform bucket sort using multi threading
    @param int num_threads max number of threads to use
    @param int chunk_size max no. of elements in each thread
*/
void multithreaded_bucket_sort(int num_threads, int chunk_size){

     // global init - start
	pthread_t* threads = (pthread_t*)malloc((num_threads)*sizeof(pthread_t));
    pthread_barrier_init(&bar1, NULL, num_threads);
	size_t current_chunk = chunk_size;

    num_of_buckets = ceil(float(log(numbers.size())));
    maximum = *std::max_element(numbers.begin(), numbers.end());
    std::vector<std::mutex> list(num_of_buckets);
    mtx.swap(list);
    all_buckets.resize(num_of_buckets);

	struct indices_data data[num_threads];
    data[0].thread_id = 0;
    data[0].start = 0;
    data[0].end = chunk_size-1;
	for(int i = 1; i < num_threads; i++){
		int start = current_chunk;
        int end;
        if(current_chunk + chunk_size >= numbers.size()){
            end = numbers.size() - 1;
        }else{
            end = current_chunk + chunk_size - 1;
        }
        data[i].thread_id = i;
        data[i].start = start;
        data[i].end = end;
        current_chunk += chunk_size;
	}
    // global init - end

    // create threads
	for(int i = 1; i < num_threads; i++){
        int ret = pthread_create(&(threads[i]), NULL, &bucket_sort, (void *)(&data[i]));
        if(ret){
            printf("ERROR; pthread_create: %d\n", ret);
            exit(-1);
        }
    }
    bucket_sort((void *)(&data[0]));

    // join threads
    for(int i=1; i<num_threads; i++){
        int ret = pthread_join(threads[i],NULL);
        if(ret){
            printf("ERROR; pthread_join: %d\n", ret);
            exit(-1);
        }
    }

    numbers.clear();
    for(int i = 0; i < num_of_buckets; i++){
        while(!all_buckets[i].empty()){
            numbers.push_back(all_buckets[i].top());
            all_buckets[i].pop();
        }
    }
    clock_gettime(CLOCK_MONOTONIC,&end_time);

    // global clean up
    free(threads);
    pthread_barrier_destroy(&bar1);
}