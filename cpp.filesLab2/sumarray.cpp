#include "../h.filesLab2/sumarray.h"
#include <string>
#include <unordered_map>

using namespace std;

Subarray Sum(const MyArray& arr, int target_sum) {
    Subarray res;
    res.found = false;
    res.start_index = -1;
    res.end_index = -1;
    res.target_sum = target_sum;

    int n = arr.get_size();
    if (n == 0) return res;

    // Хеш-таблица: префиксная_сумма -> индекс окончания префикса
    unordered_map<int, int> prefix_map;
    // Базовый случай: префиксная сумма 0 достигается до начала массива (индекс -1)
    prefix_map[0] = -1;

    int current_sum = 0;

    for (int right = 0; right < n; ++right) {
        current_sum += stoi(arr.get(right));
        int needed = current_sum - target_sum;

        if (prefix_map.find(needed) != prefix_map.end()) {
            res.found = true;
            res.start_index = prefix_map[needed] + 1;
            res.end_index = right;

            for (int i = res.start_index; i <= res.end_index; ++i) {
                res.elements.push_back(stoi(arr.get(i)));
            }
            return res;
        }

        // Сохраняем только первое появление суммы, чтобы найти самый ранний подотрезок
        if (prefix_map.find(current_sum) == prefix_map.end()) {
            prefix_map[current_sum] = right;
        }
    }

    return res;
}