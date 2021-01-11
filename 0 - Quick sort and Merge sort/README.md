## Quick sort and Merge sort

The program attached sorts a file of integers and stores the sorted integers into a new an output file. The code uses merge or quick sort algorithm based on the input provided. The package includes a makefile which complies code and also cleans up the binaries.

### Compilation instructions
``` g++ main.cpp merge_sort.cpp quick_sort.cpp -o mysort ```

### Execution instructions
The code execution format as given below
``` ./mysort [--name] <sourcefile.txt> -o <outfile.txt> --alg=<merge,quick> ```

Only using the name option will print my name. Running it without the name option but rest of them will sort and output the file. Using all the options will print my name as well as ouput a sorted file.
Examples:

``` ./mysort [--name] ```

``` ./mysort [--name] numbers.txt -o ouput.txt --alg=merge ``` 

``` ./mysort numbers.txt -o ouput.txt --alg=quick ```
