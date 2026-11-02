#include "../h.filesLab2/Set.h"
#include <iostream>

using namespace std;

static size_t hash_func(const string& s) {
    size_t h = 0;
    for (char c : s) {
        h = h * 31 + static_cast<unsigned char>(c);
    }
    return h;
}

void Set::init(size_t cap) {
    capacity = cap;
    size = 0;
    buckets = new SinglyNode*[capacity];
    for (size_t i = 0; i < capacity; ++i) {
        buckets[i] = nullptr;
    }
}

void Set::destroy() {
    if (!buckets) return;

    for (size_t i = 0; i < capacity; ++i) {
        SinglyNode* curr = buckets[i];
        while (curr != nullptr) {
            SinglyNode* to_delete = curr;
            curr = curr->next;
            delete to_delete;
        }
    }
    delete[] buckets;
    buckets = nullptr;
    size = 0;
    capacity = 0;
}


// Проверка наличия val во множестве
bool Set::contains(const string& val) const {
    if (capacity == 0) return false;
    size_t idx = hash_func(val) % capacity;

    SinglyNode* curr = buckets[idx];
    while (curr != nullptr) {
        if (curr->val == val) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

bool Set::add(const string& val) {
    if (contains(val)) {
        return false;
    }

    size_t idx = hash_func(val) % capacity;

    SinglyNode* node = new SinglyNode;
    node->val = val;
    node->next = buckets[idx];
    buckets[idx] = node;

    size++;
    return true;
}

bool Set::remove(const string& val) {
    if (capacity == 0) return false;
    size_t idx = hash_func(val) % capacity;

    SinglyNode* curr = buckets[idx];
    SinglyNode* prev = nullptr;

    while (curr != nullptr) {
        if (curr->val == val) {
            if (prev == nullptr) {
                buckets[idx] = curr->next;
            } else {
                prev->next = curr->next;
            }
            delete curr;
            size--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

void Set::print() const {
    cout << "{ ";
    bool first = true;
    for (size_t i = 0; i < capacity; ++i) {
        SinglyNode* curr = buckets[i];
        while (curr != nullptr) {
            if (!first) cout << ", ";
            cout << curr->val;
            first = false;
            curr = curr->next;
        }
    }
    cout << " }\n";
}