#ifndef SET_H
#define SET_H

#include <string>
#include "h.filesLab1/SinglyList.h"

// Так как у нас множество, будем использовать хеш-таблицу - массив, ячейками которых являются односвязные списки (чтобы все операции выполнялись за O(1))
struct Set{
    SinglyNode** buckets; // массив указателей из цепочки
    size_t capacity;// размер нашей хеш-таблицы
    size_t size; // число уникальных элементов

    void init(size_t cap = 120);
    void destroy();

    bool add(const std::string& val); // SETADD
    bool remove(const std::string& val);// SETDEL
    bool contains(const std::string& val) const;// SET_AT
    void print() const;

};


#endif