/**
	Program to sort integers from a file using concurrency
	Sorted output written into another file
	Usage: ./mysort [--name] <sourcefile.txt> -o <outfile.txt> -t <num_threads> --alg=<fjmerge,fjquick,lkbucket>
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
#include "quick_sort.h"
#include "bucket_sort.h"
#include "common.h"


using namespace std;

vector<int> numbers;
struct timespec start_time, end_time;

/**
    Reads a file of integers from a file
    @param string input_file_name The input file to read.
    @return vector<int> numbers from file.
*/
vector<int> read_input(string input_file_name){
    ifstream read(input_file_name);
    string n;
    vector<int> numbers;
    while (read >> n){
        numbers.push_back(stoi( n ));
    }
    return numbers;
}

/**
    Writes a vector of integers to a file
    @param vector<int> numbers sorted numbers.
    @param string output_file_name The output file to write.
    @return void
*/
void write_to_file(vector<int> numbers, string output_file_name){
    ofstream output_file(output_file_name);
    ostream_iterator<signed long long int> output_iterator(output_file, "\n");
    copy(numbers.begin(), numbers.end(), output_iterator);
}

/**
    Display the usage of the program
*/
void show_usage(){
	cout << "Usage: \n" ;
	cout << "./mysort [--name] <sourcefile.txt> -o <outfile.txt> -t <num_threads> --alg=<fjmerge,fjquick,lkbucket> \n" ;
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
    Read numbers from file, sort based on input algorithm and write output to a file
*/
int main(int argc, char *argv[]){

    static struct option long_options[] = {
        {"name", no_argument, NULL, 'n'},
        {"alg", required_argument, NULL, 'a'},
        {0, 0, 0, 0}
    };

    //Variables for options
    bool name = false;
    string input_file_name = "";
    string output_file_name = "";
    int num_threads = 1;
    string algo = "";

    while (true) {
        int option_index = 0;
        int flag_char = getopt_long(argc, argv, "o:t:", long_options, &option_index);
        if (flag_char == -1) {
          break;
        }

        switch (flag_char) {
            case 'n':
                name = true;
                break;
            case 'a':
                algo = std::string(optarg);
                break;
            case 'o':
                output_file_name = std::string(optarg);
                break;
            case 't':
                num_threads = stoi(optarg);
                break;
            case '?':
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    if (optind < argc) {
        input_file_name = std::string(argv[optind]);
    }

    if(name){
        cout << "Shreyas Gopalakrishna\n";
    }

	if(argc > 2){
	    if(input_file_name == "" || output_file_name == "" || algo == ""){
	        show_usage();
	    }else{
	        numbers = read_input(input_file_name);

            // divide array into chunks, each thread will get a chunk
            int chunk_size = ceil(float(numbers.size()) / num_threads);

            // call the respective sort algo
	        if(algo == "fjmerge" || algo == "fjquick"){
	            multithreaded_quick_sort(num_threads, chunk_size);
	        }
	        else if (algo == "lkbucket"){
                multithreaded_bucket_sort(num_threads, chunk_size);
	        }else{
	            cout << "Invalid algorithm type";
	            show_usage();
	        }
	        write_to_file(numbers, output_file_name);
            show_elapsed_time();
	    }
    }
    return 0;
}