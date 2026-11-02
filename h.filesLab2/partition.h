#ifndef PARTITION_H
#define PARTITION_H

#include <vector>

using namespace std;

struct PartitionResult{
    vector<int> subset1; // Инициализируем класс который состоит из 2 подмножетсв
    vector<int> subset2;

    int sum1;
    int sum2;
    int diff;// Разница между суммами подмножеств

};

PartitionResult difference(const vector<int>& numbers); // Функция разбиения на подмножества


#endif