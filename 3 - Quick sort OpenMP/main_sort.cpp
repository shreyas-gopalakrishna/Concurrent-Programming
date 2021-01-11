/**
    Program to sort integers from a file using OpenMP
    Sorted output written into another file
    Usage: ./mysort [--name] <sourcefile.txt> -o <outfile.txt>
*/
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <getopt.h>
#include <iterator>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

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
    cout << "./mysort [--name] <sourcefile.txt> -o <outfile.txt> \n" ;
    exit(EXIT_FAILURE);
}


/**
    Get the partition index for the vector
    @param vector numbers to partition
    @param int start points to start index.
    @param int end points to end index.
    @return int index partition index.
*/
int partition(vector<int> &numbers, int start, int end){
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
    Perform quick sort using OpenMP multi threading
    @param vector numbers to quick sort
    @param int start points to start index.
    @param int end points to end index.
*/
void openmp_quick_sort(vector<int> &numbers, int start, int end){
    int size  = numbers.size();

    if(end >= size){
        end = size - 1;
    }

    if(start < end){
        int partition_index = partition(numbers, start, end);

        openmp_quick_sort(numbers, start, partition_index-1);
        
        openmp_quick_sort(numbers, partition_index+1, end);
    }
}

/**
    Given 2 arrays which are sorted, combine them into one
    @param void *input with start, mid and end indices
*/
void combine_two(int start, int mid, int end){
    int size  = numbers.size();
    if(start >= size){
        return;
    }
    if(mid >= size){
        mid = size-1;
    }
    if(end >= size){
        end = size-1;
    }

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
}

/**
    Given K arrays which are sorted, combine them into one using OpenMP
    @param int chunk_size size of each sorted array
    @param int num_threads max threads to use
*/
void combine_k(int chunk_size, int num_threads){
    int times = 0;
    int size  = numbers.size();

    while(times++ <= log(num_threads) + 1){
        #pragma omp for
        for(int start = 0; start < size; start = start + (2 * chunk_size)){
            combine_two(start, start + chunk_size - 1, start + (2 * chunk_size) - 1);
        }

        chunk_size *= 2;
    }
}


/**
    Read numbers from file, sort based on input algorithm and write output to a file
*/
int main(int argc, char *argv[]){

    static struct option long_options[] = {
        {"name", no_argument, NULL, 'n'},
        {0, 0, 0, 0}
    };

    //Variables for options
    bool name = false;
    string input_file_name = "";
    string output_file_name = "";

    while (true) {
        int option_index = 0;
        int flag_char = getopt_long(argc, argv, "o:", long_options, &option_index);
        if (flag_char == -1) {
          break;
        }

        switch (flag_char) {
            case 'n':
                name = true;
                break;
            case 'o':
                output_file_name = std::string(optarg);
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
        if(input_file_name == "" || output_file_name == ""){
            show_usage();
        }else{
            numbers = read_input(input_file_name);

            // divide array into chunks, each thread will get a chunk
            int chunk_size = ceil(float(numbers.size()) / omp_get_max_threads());
            int size  = numbers.size();

            clock_gettime(CLOCK_MONOTONIC,&start_time);
            #pragma omp parallel default(none) shared(numbers, chunk_size, size)
            {
                #pragma omp for
                for(int start = 0; start < size; start = start + chunk_size){
                    openmp_quick_sort(numbers, start, start + chunk_size - 1);
                }
                if(omp_get_max_threads() > 1){
                    combine_k(chunk_size, omp_get_max_threads());
                }
            }
            
            clock_gettime(CLOCK_MONOTONIC,&end_time);

            write_to_file(numbers, output_file_name);
        }
    }
    return 0;
}
