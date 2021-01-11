#include <vector>
#include <cmath>
#include "merge_sort.h"

/**
    Sorts a vector of integers using the merge sort algorithm
    @param vector<int> numbers to be sorted.
    @return vector<int> numbers after merge sort.
*/
vector<int> merge_sort(vector<int> numbers){
    int size = numbers.size();

    if(size <= 1) {
        return numbers;
    }

    int middle = floor(size/2);
    vector<int> left(numbers.begin(), numbers.begin() + middle);
    vector<int> right(numbers.begin() + middle, numbers.end());

    return merge(merge_sort(left), merge_sort(right));
}

/**
    Merge two unsorted vectors into a sorted vector
    @param vector<int> left to be sorted.
    @param vector<int> right to be sorted.
    @return vector<int> numbers after merging.
*/
vector<int> merge(vector<int> left,
    vector<int> right){

    int left_size = left.size();
    int right_size = right.size();

    int left_index = 0;
    int right_index = 0;

    vector<int> sorted;

    bool useLeft;
    for (int i = 0; i < left_size + right_size; i++) {

        if( left_index == left_size || right_index == right_size){
            break;
        }

        if(left[left_index] < right[right_index]){
            sorted.push_back(left[left_index]);
            left_index++;
        }else{
            sorted.push_back(right[right_index]);
            right_index++;
        }
    }

    if(left_index < left_size){
        for(int i = left_index; i < left_size; i++){
            sorted.push_back(left[i]);
        }
    }else if(right_index < right_size){
        for(int i = right_index; i < right_size; i++){
            sorted.push_back(right[i]);
        }
    }
    return sorted;
}