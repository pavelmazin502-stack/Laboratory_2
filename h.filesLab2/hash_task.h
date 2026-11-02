#ifndef HASH_TASK_H
#define HASH_TASK_H

#include <string>
#include "../h.filesLab1/SinglyList.h"

// 1. Хеш-таблица с методом цепочек
struct ChainedHashTable {
    SinglyNode** buckets;
    size_t capacity;
    size_t collisions;

    void init(size_t cap = 101);
    void destroy();
    void insert(const std::string& key, const std::string& val);
    std::string find(const std::string& key) const;
};

// 2. Хеш-таблица с кукушкиным хешированием (Cuckoo Hashing)
struct CuckooEntry {
    std::string key;
    std::string val;
    bool occupied;
};

struct CuckooHashTable {
    CuckooEntry* table1;
    CuckooEntry* table2;
    size_t capacity;
    size_t evictions;

    void init(size_t cap = 101);
    void destroy();
    bool insert(const std::string& key, const std::string& val);
    std::string find(const std::string& key) const;
};

// 3. Задача: количество сотрудников в отделах
struct EmployeeStats {
    int total_employees;
    int collisions_count;
};

EmployeeStats count_employees(const std::string* dept_keys, int count);

#endif