#include "quick_sort.h"
#include "common.h"

pthread_t* threads;
pthread_barrier_t bar;

struct combine_data{
	int start;
    int mid;
	int end;
};

/**
    Sorts a vector of integers using the quick sort algorithm
    @param struct indices_data which gives the start, end indices
*/
void *quick_sort(void *input){
    // local init - start
	struct indices_data *data = (struct indices_data*)input;
    int start = data->start;
    int end = data->end;
    int tid = data->thread_id;
    // local init - end

    // algo - start
    // pthread_barrier_wait(&bar);
    if(tid==0){
		clock_gettime(CLOCK_MONOTONIC,&start_time);
	}

    if(start < end){
        int partition_index = partition(start, end);

        struct indices_data d[2];
        d[0].start = start;
        d[0].end = partition_index-1;
        d[1].start = partition_index + 1;
        d[1].end = end;
        quick_sort((void *)(&d[0]));
        quick_sort((void *)(&d[1]));
    }

    // pthread_barrier_wait(&bar);
	if(tid==0){
		clock_gettime(CLOCK_MONOTONIC,&end_time);
	}
    // algo - end
    return NULL;
}

/**
    Get the partition index for the vector
    @param int start points to start index.
    @param int end points to end index.
    @return int index partition index.
*/
int partition(int start, int end){
    int pivot = numbers[end];
    int partition_index = start;

    int temp;

    for(int i = start; i < end; i++){
        if(numbers[i] <= pivot){
            temp = numbers[i];
            numbers[i] = numbers[partition_index];
            numbers[partition_index] = temp;
            partition_index++;
        }
    }

    temp = numbers[end];
    numbers[end] = numbers[partition_index];
    numbers[partition_index] = temp;

    return partition_index;
}

/**
	Given 2 arrays which are sorted, combine them into one
    @param void *input with start, mid and end indices
*/
void *combine_two(void *input){
    struct combine_data *data = (struct combine_data*)input;
    int start = data->start;
    int mid = data->mid;
    int end = data->end;

    vector<int> list_1;
    vector<int> list_2;

    for(int i = start; i <= mid; i++){
        list_1.push_back(numbers[i]);
    }
    for(int i = mid+1; i <= end; i++){
        list_2.push_back(numbers[i]);
    }

    size_t current = start, i = 0, j = 0;

    while(i < list_1.size() and j < list_2.size()){
		if(list_1[i]<=list_2[j]){
			numbers[current] = list_1[i];
			i++;
		} else {
			numbers[current] = list_2[j];
			j++;
		}
		current++;
	}

	while(i < list_1.size()) {
		numbers[current] = list_1[i];
		current++;
		i++;
	}

	while(j < list_2.size()) {
		numbers[current] = list_2[j];
		current++;
		j++;
	}
    return NULL;
}

/**
	Given K arrays which are sorted, combine them into one
    @param size_t chunk_size size of each sorted array
    @param int num_threads max threads to use
*/
void combine_k(size_t chunk_size, int num_threads){
    int start = 0;
    size_t mid = 0;
    size_t end = 0;
    int tid = 1;
    int prev_tid = 1;
    int times = 0;

    while(times++ <= log(num_threads)+1){    
        start = 0;
        end = 0;
        mid = 0; 
        while(end < numbers.size()){
            mid = start + chunk_size - 1;
            end = mid + chunk_size;
            if(mid >= numbers.size()){
                mid = numbers.size()-1;
            }
            if(end >= numbers.size()){
                end = numbers.size()-1;
            }

            struct combine_data *d = (struct combine_data *) malloc(sizeof(struct combine_data));
            d->start = start;
            d->end = end;
            d->mid = mid;

            int ret = pthread_create(&(threads[tid]), NULL, &combine_two, (void *)(d));
            if(ret){
                printf("ERROR; pthread_create: %d\n", ret);
                exit(-1);
            }
            tid += 1;

            if(end >= numbers.size() - 1){
                break;
            }
            start = end + 1;
        }
        for(int i = prev_tid; i < tid; i++){
            int ret = pthread_join(threads[i],NULL);
            if(ret){
                printf("ERROR; pthread_join: %d\n", ret);
                exit(-1);
            }
        }
        prev_tid = tid;
        chunk_size *= 2;
    }
}

/**
    Perform quick sort using multi threading
    @param int num_threads max number of threads to use
    @param int chunk_size max no. of elements in each thread
*/
void multithreaded_quick_sort(int num_threads, int chunk_size){

    // global init - start
	threads = (pthread_t*)malloc((num_threads)*sizeof(pthread_t));
    pthread_barrier_init(&bar, NULL, num_threads);
	size_t current_chunk = chunk_size;

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
        int ret = pthread_create(&(threads[i]), NULL, &quick_sort, (void *)(&data[i]));
        if(ret){
            printf("ERROR; pthread_create: %d\n", ret);
            exit(-1);
        }
    }
    quick_sort((void *)(&data[0]));

    // join threads
    for(int i=1; i<num_threads; i++){
        int ret = pthread_join(threads[i],NULL);
        if(ret){
            printf("ERROR; pthread_join: %d\n", ret);
            exit(-1);
        }
    }

    if(num_threads > 1){
        combine_k(chunk_size, num_threads);
    }
    clock_gettime(CLOCK_MONOTONIC,&end_time);

    // global clean up
    free(threads);
    pthread_barrier_destroy(&bar);
}
