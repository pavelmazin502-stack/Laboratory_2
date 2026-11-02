#ifndef SUMARRAY_H
#define SUMARRAY_H

#include "../h.filesLab1/Array.h"
#include <vector>

struct Subarray{
    bool found; // Нашли ли мы такой отрезок = заданному числу
    int start_index;
    int end_index;
    int target_sum; 
    std::vector<int> elements; 
};

Subarray Sum(const MyArray& arr, int target_sum);

#endif