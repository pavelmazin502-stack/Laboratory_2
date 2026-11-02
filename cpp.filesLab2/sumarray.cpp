#include "../h.filesLab2/sumarray.h"
#include <string>

using namespace std;

Subarray Sum(const MyArray& arr, int target_sum) {
    Subarray res;
    res.found = false;
    res.start_index = -1;
    res.end_index = -1;
    res.target_sum = target_sum;

    int n = arr.get_size();
    if (n == 0) return res;

    int left = 0;
    int current_sum = 0;

    for (int right = 0; right < n; ++right) {
        int val = stoi(arr.get(right));
        current_sum += val;

        while (current_sum > target_sum && left <= right) {
            current_sum -= stoi(arr.get(left));
            left++;
        }

        if (current_sum == target_sum) {
            res.found = true;
            res.start_index = left;
            res.end_index = right;
            for (int i = left; i <= right; ++i) {
                res.elements.push_back(stoi(arr.get(i)));
            }
            return res;
        }
    }

    return res;
}