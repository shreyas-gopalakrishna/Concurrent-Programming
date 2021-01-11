#include <vector>
#include "quick_sort.h"

/**
    Sorts a vector of integers using the quick sort algorithm
    @param vector<int> numbers to be sorted.
    @param int start points to start index.
    @param int end points to end index.
*/
void quick_sort(vector<int> &numbers, int start, int end){
    if(start < end){
        int partition_index = partition(numbers, start, end);
        quick_sort(numbers, start, partition_index-1);
        quick_sort(numbers, partition_index+1, end);
    }
}

/**
    Get the partition index for the vector
    @param vector<int> numbers to be sorted.
    @param int start points to start index.
    @param int end points to end index.
    @param int index partition index.
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