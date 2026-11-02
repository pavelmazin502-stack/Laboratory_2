#include "../h.filesLab2/partition.h"
#include <cmath>

using namespace std;

PartitionResult difference(const vector<int>& numbers){
    PartitionResult res; // минимальная разница

    if (numbers.empty()){
        res.sum1 = 0;
        res.sum2 = 0;
        res.diff = 0;
        return res;
    }

    int total_sum = 0; // Общая сумма 
    for (int x : numbers){
        total_sum += x;
    }
    int target = total_sum/2;

    vector<bool> dp(target+1, false); // Это как коробочки, если можем собрать такую сумму то true
    vector<vector<int>> subset(target+1);

    dp[0] = true; // Нулевую сумму всегда можно собрать

    for (int x:numbers){
        for (int s = target; s >= x; --s){ // Проходим сначала к коробке 17
            if (dp[s-x] && !dp[s]){
                dp[s] = true;
                subset[s] = subset[s-x];
                subset[s].push_back(x);
            }
        }
    }

    int best_sum = 0; // Самая большая сумма
    for (int s = target; s >= 0; --s){
        if (dp[s]){
            best_sum = s;
            break;
        }
    }

    //Первое подмножество у нас уже готово в subsets[best_s]
    res.subset1 = subset[best_sum];
    res.sum1 = best_sum;

    //Всё, что не вошло в subset1, отправляем в subset2
    vector<int> remaining = numbers;
    for (int x : res.subset1) {
        for (auto it = remaining.begin(); it != remaining.end(); ++it) {
            if (*it == x) {
                remaining.erase(it);
                break;
            }
        }
    }

    res.subset2 = remaining;
    res.sum2 = total_sum - res.sum1;
    res.diff = abs(res.sum1 - res.sum2);

    return res;
}