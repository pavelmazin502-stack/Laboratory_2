#ifndef LFU_CACHE_H
#define LFU_CACHE_H

#include <string>

struct LFUEntry {
    std::string key;
    std::string val;
    int freq;
    long long time_step;
};

struct LFUCache {
    LFUEntry* entries;
    size_t capacity;
    size_t size;
    long long timer;

    void init(size_t cap);
    void destroy();
    std::string get(const std::string& key);
    void put(const std::string& key, const std::string& val);
    void print() const;
};

#endif