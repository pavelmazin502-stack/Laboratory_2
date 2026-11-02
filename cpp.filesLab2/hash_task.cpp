#include "h.filesLab2/hash_task.h"
#include <iostream>

using namespace std;

static size_t hash1(const string& s, size_t cap) {
    size_t h = 0;
    for (char c : s) {
        h = (h * 31 + static_cast<unsigned char>(c)) % cap;
    }
    return h;
}

static size_t hash2(const string& s, size_t cap) {
    size_t h = 0;
    for (char c : s) {
        h = (h * 37 + static_cast<unsigned char>(c)) % cap;
    }
    return h;
}

// Реализация цепочек
void ChainedHashTable::init(size_t cap) {
    capacity = cap;
    collisions = 0;
    buckets = new SinglyNode*[capacity];
    for (size_t i = 0; i < capacity; ++i) buckets[i] = nullptr;
}

void ChainedHashTable::destroy() {
    if (!buckets) return;
    for (size_t i = 0; i < capacity; ++i) {
        SinglyNode* curr = buckets[i];
        while (curr != nullptr) {
            SinglyNode* nxt = curr->next;
            delete curr;
            curr = nxt;
        }
    }
    delete[] buckets;
    buckets = nullptr;
}

void ChainedHashTable::insert(const string& key, const string& val) {
    size_t idx = hash1(key, capacity);
    if (buckets[idx] != nullptr) {
        collisions++;
    }
    SinglyNode* node = new SinglyNode;
    node->val = key + ":" + val;
    node->next = buckets[idx];
    buckets[idx] = node;
}

string ChainedHashTable::find(const string& key) const {
    size_t idx = hash1(key, capacity);
    SinglyNode* curr = buckets[idx];
    while (curr != nullptr) {
        if (curr->val.rfind(key + ":", 0) == 0) {
            return curr->val.substr(key.size() + 1);
        }
        curr = curr->next;
    }
    return "";
}

// Реализация кукушки
void CuckooHashTable::init(size_t cap) {
    capacity = cap;
    evictions = 0;
    table1 = new CuckooEntry[capacity];
    table2 = new CuckooEntry[capacity];
    for (size_t i = 0; i < capacity; ++i) {
        table1[i].occupied = false;
        table2[i].occupied = false;
    }
}

void CuckooHashTable::destroy() {
    delete[] table1;
    delete[] table2;
    table1 = nullptr;
    table2 = nullptr;
}

bool CuckooHashTable::insert(const string& key, const string& val) {
    string cur_k = key;
    string cur_v = val;

    for (size_t step = 0; step < capacity; ++step) {
        size_t idx1 = hash1(cur_k, capacity);
        if (!table1[idx1].occupied) {
            table1[idx1] = {cur_k, cur_v, true};
            return true;
        }
        evictions++;
        swap(cur_k, table1[idx1].key);
        swap(cur_v, table1[idx1].val);

        size_t idx2 = hash2(cur_k, capacity);
        if (!table2[idx2].occupied) {
            table2[idx2] = {cur_k, cur_v, true};
            return true;
        }
        evictions++;
        swap(cur_k, table2[idx2].key);
        swap(cur_v, table2[idx2].val);
    }
    return false;
}

string CuckooHashTable::find(const string& key) const {
    size_t idx1 = hash1(key, capacity);
    if (table1[idx1].occupied && table1[idx1].key == key) return table1[idx1].val;
    size_t idx2 = hash2(key, capacity);
    if (table2[idx2].occupied && table2[idx2].key == key) return table2[idx2].val;
    return "";
}

// Подсчет сотрудников
EmployeeStats count_employees(const string* dept_keys, int count) {
    ChainedHashTable ht;
    ht.init(101);
    for (int i = 0; i < count; ++i) {
        ht.insert(dept_keys[i], "1");
    }
    EmployeeStats stats;
    stats.total_employees = count;
    stats.collisions_count = ht.collisions;
    ht.destroy();
    return stats;
}