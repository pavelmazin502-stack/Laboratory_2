#include "../h.filesLab2/hash_task.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <map>
#include <set>

using namespace std;
using namespace std::chrono;

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

// =======================================================
// 1. РЕАЛИЗАЦИЯ МЕТОДА ЦЕПОЧЕК (Chaining)
// =======================================================

void ChainedHashTable::init(size_t cap) {
    capacity = cap;
    collisions = 0;
    buckets = new HashNode*[capacity];
    for (size_t i = 0; i < capacity; ++i) {
        buckets[i] = nullptr;
    }
}

void ChainedHashTable::destroy() {
    if (!buckets) return;
    for (size_t i = 0; i < capacity; ++i) {
        HashNode* curr = buckets[i];
        while (curr != nullptr) {
            HashNode* nxt = curr->next;
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
    HashNode* node = new HashNode;
    node->val = key + ":" + val;
    node->next = buckets[idx];
    buckets[idx] = node;
}

string ChainedHashTable::find(const string& key) const {
    size_t idx = hash1(key, capacity);
    HashNode* curr = buckets[idx];
    string prefix = key + ":";
    while (curr != nullptr) {
        if (curr->val.rfind(prefix, 0) == 0) {
            return curr->val.substr(prefix.size());
        }
        curr = curr->next;
    }
    return "";
}

void ChainedHashTable::add_report(const string& manager, const string& emp) {
    size_t idx = hash1(manager, capacity);
    HashNode* curr = buckets[idx];

    while (curr != nullptr) {
        if (curr->key == manager) {
            curr->direct_reports.push_back(emp);
            return;
        }
        curr = curr->next;
    }

    if (buckets[idx] != nullptr) {
        collisions++;
    }

    HashNode* node = new HashNode;
    node->key = manager;
    node->direct_reports.push_back(emp);
    node->next = buckets[idx];
    buckets[idx] = node;
}

const vector<string>* ChainedHashTable::get_reports(const string& manager) const {
    size_t idx = hash1(manager, capacity);
    HashNode* curr = buckets[idx];
    while (curr != nullptr) {
        if (curr->key == manager) {
            return &(curr->direct_reports);
        }
        curr = curr->next;
    }
    return nullptr;
}

// =======================================================
// 2. РЕАЛИЗАЦИЯ КУКУШКИНОГО ХЕШИРОВАНИЯ (Cuckoo Hashing)
// =======================================================

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

// =======================================================
// 3. ПОДСЧЕТ СОТРУДНИКОВ (Вариант 5)
// =======================================================

static int count_all_subordinates(const string& mgr, const ChainedHashTable& ht, map<string, int>& memo) {
    if (memo.find(mgr) != memo.end()) {
        return memo[mgr];
    }

    int count = 0;
    const vector<string>* direct = ht.get_reports(mgr);
    if (direct != nullptr) {
        for (const string& sub : *direct) {
            if (sub != mgr) {
                count += 1 + count_all_subordinates(sub, ht, memo);
            }
        }
    }

    memo[mgr] = count;
    return count;
}

EmployeeStats count_employees(const string* pairs, size_t count) {
    ChainedHashTable ht;
    ht.init(101);

    set<string> all_people;

    for (size_t i = 0; i + 1 < count; i += 2) {
        string emp = pairs[i];
        string mgr = pairs[i + 1];

        all_people.insert(emp);
        all_people.insert(mgr);

        if (emp != mgr) {
            ht.add_report(mgr, emp);
        }
    }

    EmployeeStats stats;
    stats.total_employees = all_people.size();
    stats.collisions_count = ht.collisions;

    map<string, int> memo;
    for (const string& person : all_people) {
        stats.subordinate_counts[person] = count_all_subordinates(person, ht, memo);
    }

    ht.destroy();
    return stats;
}

// =======================================================
// 4. ЭМПИРИЧЕСКИЙ АНАЛИЗ ХЕШИРОВАНИЯ
// =======================================================

void run_hash_benchmark_row(size_t n, size_t cap) {
    vector<string> keys(n);
    for (size_t i = 0; i < n; ++i) {
        keys[i] = "employee_key_" + to_string(i * 17 + 3);
    }

    ChainedHashTable chained;
    chained.init(cap);

    auto t1 = high_resolution_clock::now();
    for (size_t i = 0; i < n; ++i) {
        chained.insert(keys[i], to_string(i));
    }
    auto t2 = high_resolution_clock::now();

    auto t3 = high_resolution_clock::now();
    for (size_t i = 0; i < n; ++i) {
        volatile string res = chained.find(keys[i]);
    }
    auto t4 = high_resolution_clock::now();

    double chain_ins = duration<double, micro>(t2 - t1).count();
    double chain_fnd = duration<double, micro>(t4 - t3).count();
    int chain_col = chained.collisions;
    chained.destroy();

    CuckooHashTable cuckoo;
    cuckoo.init(cap);

    auto t5 = high_resolution_clock::now();
    size_t successful_cuckoo = 0;
    for (size_t i = 0; i < n; ++i) {
        if (cuckoo.insert(keys[i], to_string(i))) {
            successful_cuckoo++;
        }
    }
    auto t6 = high_resolution_clock::now();

    auto t7 = high_resolution_clock::now();
    for (size_t i = 0; i < successful_cuckoo; ++i) {
        volatile string res = cuckoo.find(keys[i]);
    }
    auto t8 = high_resolution_clock::now();

    double cuckoo_ins = duration<double, micro>(t6 - t5).count();
    double cuckoo_fnd = duration<double, micro>(t8 - t7).count();
    int cuckoo_evict = cuckoo.evictions;
    cuckoo.destroy();

    cout << "| " << setw(7) << n 
         << " | " << setw(5) << cap 
         << " | " << setw(11) << fixed << setprecision(1) << chain_ins 
         << " | " << setw(9) << chain_fnd 
         << " | " << setw(9) << chain_col 
         << " | " << setw(11) << cuckoo_ins 
         << " | " << setw(9) << cuckoo_fnd 
         << " | " << setw(10) << cuckoo_evict << " |\n";
}

void run_hash_benchmark() {
    cout << "------------------------------------------------------------------------------------------------------\n";
    cout << "|         |       |        Метод цепочек (Chaining)        |       Кукушкино хеширование (Cuckoo)    |\n";
    cout << "|    N    |  Cap  | Вставка(мкс) | Поиск(мкс) | Коллизии  | Вставка(мкс) | Поиск(мкс) | Вытеснения |\n";
    cout << "------------------------------------------------------------------------------------------------------\n";
    run_hash_benchmark_row(100, 500);
    run_hash_benchmark_row(200, 500);
    run_hash_benchmark_row(300, 500);
    run_hash_benchmark_row(450, 500);
    cout << "------------------------------------------------------------------------------------------------------\n";
}