#include "../h.filesLab2/lfu_cache.h"
#include <iostream>

using namespace std;

void LFUCache::init(int cap) {
    capacity = cap;
    min_freq = 0;
    key_map.clear();
    freq_map.clear();
}

void LFUCache::update_freq(list<Node>::iterator it) {
    int old_freq = it->freq;
    int new_freq = old_freq + 1;
    string k = it->key;
    string v = it->val;

    // Удаляем из старого частотного списка
    freq_map[old_freq].erase(it);
    if (freq_map[old_freq].empty()) {
        freq_map.erase(old_freq);
        if (min_freq == old_freq) {
            min_freq = new_freq;
        }
    }

    // Вставляем в голову нового частотного списка
    freq_map[new_freq].push_front({k, v, new_freq});
    key_map[k] = freq_map[new_freq].begin();
}

void LFUCache::set(const string& key, const string& val) {
    if (capacity <= 0) return;

    // Если ключ уже есть — обновляем значение и повышаем частоту
    if (key_map.find(key) != key_map.end()) {
        auto it = key_map[key];
        it->val = val;
        update_freq(it);
        return;
    }

    // Если кэш заполнен — вытесняем наименее часто используемый элемент
    if (static_cast<int>(key_map.size()) >= capacity) {
        auto& min_list = freq_map[min_freq];
        // Последний элемент в списке — самый старый при данной частоте (LRU)
        string evict_key = min_list.back().key;
        min_list.pop_back();
        if (min_list.empty()) {
            freq_map.erase(min_freq);
        }
        key_map.erase(evict_key);
    }

    // Вставляем новый элемент со счётчиком обращений 1
    min_freq = 1;
    freq_map[1].push_front({key, val, 1});
    key_map[key] = freq_map[1].begin();
}

string LFUCache::get(const string& key) {
    if (key_map.find(key) == key_map.end()) {
        return "-1";
    }

    auto it = key_map[key];
    string val = it->val;
    update_freq(it);
    return val;
}

void LFUCache::print() const {
    cout << "LFU Cache (size=" << key_map.size() << "/" << capacity << "):\n";
    for (const auto& p : freq_map) {
        cout << "  Freq " << p.first << ": ";
        for (const auto& node : p.second) {
            cout << "[" << node.key << ": " << node.val << "] ";
        }
        cout << "\n";
    }
}