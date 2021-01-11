/**
	Program to sort integers from a file
	Output written into another file
	Usage: ./mysort [--name] <sourcefile.txt> -o <outfile.txt> --alg=<merge,quick>
*/
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <getopt.h>
#include "merge_sort.h"
#include "quick_sort.h"


using namespace std;

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
	cout << "./mysort [--name] <sourcefile.txt> -o <outfile.txt> --alg=<merge,quick> \n" ;
	exit(EXIT_FAILURE);
}


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
    string algo = "";

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
            case 'a':
                algo = std::string(optarg);
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
	    if(input_file_name == "" || output_file_name == "" || algo == ""){
	        show_usage();
	    }else{
	        vector<int> numbers = read_input(input_file_name);

	        if(algo == "merge"){
	            numbers = merge_sort(numbers);
	        }
	        else if (algo == "quick"){
	            quick_sort(numbers, 0, numbers.size()-1);
	        }else{
	            cout << "Invalid algorithm type";
	            show_usage();
	        }
	        write_to_file(numbers, output_file_name);
	    }
    }
    return 0;
}