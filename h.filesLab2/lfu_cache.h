#ifndef LFU_CACHE_H
#define LFU_CACHE_H

#include <string>
#include <unordered_map>
#include <list>

class LFUCache {
private:
    struct Node {
        std::string key;
        std::string val;
        int freq;
    };

    int capacity;
    int min_freq;
    // key -> итератор узла в списке частоты
    std::unordered_map<std::string, std::list<Node>::iterator> key_map;
    // freq -> список узлов с такой частотой (в порядке LRU: свежие в начале)
    std::unordered_map<int, std::list<Node>> freq_map;

    void update_freq(std::list<Node>::iterator it);

public:
    LFUCache() : capacity(0), min_freq(0) {}
    void init(int cap);
    void set(const std::string& key, const std::string& val);
    std::string get(const std::string& key);
    void print() const;
};

#endif