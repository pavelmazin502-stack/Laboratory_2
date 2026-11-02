#include "../h.filesLab2/lfu_cache.h"
#include <iostream>

using namespace std;

void LFUCache::init(size_t cap) {
    capacity = cap;
    size = 0;
    timer = 0;
    entries = new LFUEntry[capacity];
}

void LFUCache::destroy() {
    delete[] entries;
    entries = nullptr;
    size = 0;
    capacity = 0;
}

string LFUCache::get(const string& key) {
    timer++;
    for (size_t i = 0; i < size; ++i) {
        if (entries[i].key == key) {
            entries[i].freq++;
            entries[i].time_step = timer;
            return entries[i].val;
        }
    }
    return "";
}

void LFUCache::put(const string& key, const string& val) {
    if (capacity == 0) return;
    timer++;

    for (size_t i = 0; i < size; ++i) {
        if (entries[i].key == key) {
            entries[i].val = val;
            entries[i].freq++;
            entries[i].time_step = timer;
            return;
        }
    }

    if (size < capacity) {
        entries[size] = {key, val, 1, timer};
        size++;
    } else {
        size_t lfu_idx = 0;
        for (size_t i = 1; i < size; ++i) {
            if (entries[i].freq < entries[lfu_idx].freq ||
               (entries[i].freq == entries[lfu_idx].freq && entries[i].time_step < entries[lfu_idx].time_step)) {
                lfu_idx = i;
            }
        }
        entries[lfu_idx] = {key, val, 1, timer};
    }
}

void LFUCache::print() const {
    cout << "LFU Cache [ ";
    for (size_t i = 0; i < size; ++i) {
        cout << "(" << entries[i].key << ":" << entries[i].val << " f=" << entries[i].freq << ") ";
    }
    cout << "]\n";
}