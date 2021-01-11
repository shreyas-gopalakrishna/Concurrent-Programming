/**
	Implemening locking algorithms and barrier
    Test-and-set lock, Test-and-test-and-set lock, Ticket lock, MCS Lock and Sense-reversal barrier
	Usage: ./counter --name -t NUM_THREADS -i=NUM_ITERATIONS --bar=<sense,pthread> --lock=<tas,ttas,ticket,mcs,pthread> -o output.txt
*/
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <getopt.h>
#include <pthread.h>
#include <iterator>
#include <stdlib.h>
#include <unistd.h>
#include <atomic>
#include "locks.h"

#include <sys/types.h>
#include <sys/syscall.h>

using namespace std;

int counter = 0;
int NUM_ITERATIONS;
int NUM_THREADS; 
struct timespec start_time, end_time;

/*
    Locks
*/
locks *lk;
mutex mtx;

bool mcs_flag = false;
bool pthread_flag = false;

/*
    Barrier
*/
barrier *sense_barrier;
pthread_barrier_t pthread_bar;
bool pthread_bar_flag = false;

/**
    Writes the counter value to file
    @param string output_file_name The output file to write.
    @return void
*/
void write_to_file(string output_file_name){
    ofstream output_file(output_file_name, ofstream::out);
    output_file << counter;
    output_file.close();
}

/**
    Display the usage of the program
*/
void show_usage(){
	cout << "Usage: \n" ;
	cout << "./counter --name -t NUM_THREADS -i=NUM_ITERATIONS --bar=<sense,pthread> --lock=<tas,ttas,ticket,mcs,pthread> -o output.txt \n" ;
	exit(EXIT_FAILURE);
}

/**
    Display elapsed time
*/
void show_elapsed_time(){
    unsigned long long elapsed_ns;
	elapsed_ns = (end_time.tv_sec-start_time.tv_sec)*1000000000 + (end_time.tv_nsec-start_time.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
}

/**
    Counter thread
 */
void *counter_thread(void *input){
    int iterations = *(int*)input;
    clock_gettime(CLOCK_MONOTONIC,&start_time);
    
    for(int i = 0; i < iterations; i++){
        node *thread_node = new node();

        // Lock and unlock using the appropriate user input
        if(mcs_flag){
            lk->lock(thread_node);
        }else if(pthread_flag){
            mtx.lock();
        }else{
            lk->lock();
        }

        counter++;

        if(mcs_flag){
            lk->unlock(thread_node);
        }else if(pthread_flag){
            mtx.unlock();
        }else{
            lk->unlock();
        }
    }
    clock_gettime(CLOCK_MONOTONIC,&end_time);
    return NULL;
}

/**
    Counter main
 */
void counter_main(int num_threads, int num_iterations){

    // global init - start
	pthread_t* threads = (pthread_t*)malloc((num_threads)*sizeof(pthread_t));

    // create threads
	for(int i = 1; i < num_threads; i++){
        int ret = pthread_create(&(threads[i]), NULL, &counter_thread, (&num_iterations));
        if(ret){
            printf("ERROR; pthread_create: %d\n", ret);
            exit(-1);
        }
    }
    counter_thread(&num_iterations);

    // join threads
    for(int i = 1; i < num_threads; i++){
        int ret = pthread_join(threads[i],NULL);
        if(ret){
            printf("ERROR; pthread_join: %d\n", ret);
            exit(-1);
        }
    }

    // global clean up
    free(threads);

}

void *barrier_thread(void *input){
    int my_tid = *(int*)input;
    clock_gettime(CLOCK_MONOTONIC,&start_time);
    for(int i = 0; i < NUM_ITERATIONS * NUM_THREADS; i++){
        if(i % NUM_THREADS == my_tid){
            counter++;
        }

        // Using barrier to sync across multiple threads
        if(pthread_bar_flag == true){
            pthread_barrier_wait(&pthread_bar);
        }else{
            sense_barrier->wait();
        } 
    }
    clock_gettime(CLOCK_MONOTONIC,&end_time);
    return NULL;
}

void barrier_main(int num_threads){
    // global init - start
	pthread_t* threads = (pthread_t*)malloc((num_threads)*sizeof(pthread_t));

    // create threads
	for(int i = 1; i < num_threads; i++){
        int *tid = (int*)malloc(sizeof (int));
        *tid = i; 
        int ret = pthread_create(&(threads[i]), NULL, &barrier_thread, (tid));
        if(ret){
            printf("ERROR; pthread_create: %d\n", ret);
            exit(-1);
        }
    }
    int *tid = (int*)malloc(sizeof (int));
    *tid = 0; 
    barrier_thread(tid);

    // join threads
    for(int i = 1; i < num_threads; i++){
        int ret = pthread_join(threads[i],NULL);
        if(ret){
            printf("ERROR; pthread_join: %d\n", ret);
            exit(-1);
        }
    }

    // global clean up
    free(threads);
}

/**
    Increase counter by number of iterations in all threads and write output to a file
*/
int main(int argc, char *argv[]){

    static struct option long_options[] = {
        {"name", no_argument, NULL, 'n'},
        {"bar", required_argument, NULL, 'b'},
        {"lock", required_argument, NULL, 'l'},
        {0, 0, 0, 0}
    };

    //Variables for options
    bool name = false;
    string output_file_name = "";
    int num_threads = 1;
    int num_iterations = 1;
    string bar = "";
    string lock = "";

    while (true) {
        int option_index = 0;
        int flag_char = getopt_long(argc, argv, "t:i:o:", long_options, &option_index);
        if (flag_char == -1) {
          break;
        }

        switch (flag_char) {
            case 'n':
                name = true;
                break;
            case 'b':
                bar = std::string(optarg);
                break;
            case 'l':
                lock = std::string(optarg);
                break;
            case 'o':
                output_file_name = std::string(optarg);
                break;
            case 't':
                num_threads = stoi(optarg);
                NUM_THREADS = num_threads;
                break;
            case 'i':
                num_iterations = stoi(optarg);
                NUM_ITERATIONS = num_iterations;
                break;
            case '?':
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    if(name){
        cout << "Shreyas Gopalakrishna\n";
    }

	if(argc > 2){
	    if(output_file_name == "" || ( bar == "" && lock == "")){
	        show_usage();
	    }else if (bar != "" and lock != ""){
            show_usage();
        }else{
            if(bar != ""){
                if(bar == "sense"){
                    sense_barrier = new barrier(NUM_THREADS-1);
                    barrier_main(num_threads);
                }else if (bar == "pthread"){
                    pthread_bar_flag = true;
                    pthread_barrier_init(&pthread_bar, NULL, NUM_THREADS);
                    barrier_main(num_threads);
                    pthread_barrier_destroy(&pthread_bar);
                }
            }else{
                if (lock == "tas"){
                    lk = new test_and_set();
                }else if (lock == "ttas"){
                    lk = new test_and_test_and_set();
                }else if (lock == "ticket"){
                    lk = new ticket_lock();
                }else if (lock == "mcs"){
                    mcs_flag = true;
                    lk = new mcs();
                }else if (lock == "pthread"){
                    pthread_flag = true;
                }else{
                    show_usage();
                }
                counter_main(num_threads, num_iterations);
            }
            
	        write_to_file(output_file_name);
            show_elapsed_time();
	    }
    }
    return 0;
}