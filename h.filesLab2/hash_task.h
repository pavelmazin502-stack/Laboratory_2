#ifndef HASH_TASK_H
#define HASH_TASK_H

#include <string>
#include <vector>
#include <map>

// Узел переименован в HashNode во избежание конфликта с SinglyList.h
struct HashNode {
    std::string val;
    std::string key;
    std::vector<std::string> direct_reports;
    HashNode* next;
};

class ChainedHashTable {
public:
    HashNode** buckets;
    size_t capacity;
    int collisions;

    void init(size_t cap);
    void destroy();
    void insert(const std::string& key, const std::string& val);
    std::string find(const std::string& key) const;

    void add_report(const std::string& manager, const std::string& emp);
    const std::vector<std::string>* get_reports(const std::string& manager) const;
};

struct CuckooEntry {
    std::string key;
    std::string val;
    bool occupied;
};

class CuckooHashTable {
public:
    CuckooEntry* table1;
    CuckooEntry* table2;
    size_t capacity;
    int evictions;

    void init(size_t cap);
    void destroy();
    bool insert(const std::string& key, const std::string& val);
    std::string find(const std::string& key) const;
};

struct EmployeeStats {
    int total_employees;
    int collisions_count;
    std::map<std::string, int> subordinate_counts;
};

EmployeeStats count_employees(const std::string* pairs, size_t count);
void run_hash_benchmark();

#endif