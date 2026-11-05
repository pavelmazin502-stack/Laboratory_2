#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

// Заголовки всех заданий ЛР2
#include "h.filesLab2/stack.h"
#include "h.filesLab2/Set.h"
#include "h.filesLab2/partition.h"
#include "h.filesLab2/sumarray.h"
#include "h.filesLab2/recover_tree.h"
#include "h.filesLab2/hash_task.h"
#include "h.filesLab2/lfu_cache.h"

// Структуры из ЛР1
#include "h.filesLab1/Array.h"
#include "h.filesLab1/CompleteTree.h"

using namespace std;

int main(int argc, char* argv[]) {
    string filename = "";
    string query = "";

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filename = argv[++i];
        } else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        }
    }

    if (query.empty()) {
        cerr << "Ошибка: параметр --query отсутствует\n";
        return 1;
    }

    // Состояния структур
    static map<string, Set> sets;
    static map<string, MyArray> arrays;
    static CompleteTree tree;
    static bool tree_init_flag = false;
    static LFUCache lfu;
    static bool lfu_init_flag = false;

    if (!tree_init_flag) { 
        tree.init(); 
        tree_init_flag = true; 
    }
    if (!lfu_init_flag) { 
        lfu.init(3); 
        lfu_init_flag = true; 
    }

    stringstream ss_all(query);
    string single_cmd;

    while (getline(ss_all, single_cmd, ';')) {
        vector<string> tokens;
        istringstream ss(single_cmd);
        string token;
        while (ss >> token) {
            tokens.push_back(token);
        }

        if (tokens.empty()) {
            continue;
        }

        string cmd = tokens[0];

        // Задание 1: RPN
        if (cmd == "RPN" && tokens.size() > 1) {
            string expr = "";
            for (size_t i = 1; i < tokens.size(); ++i) {
                expr += tokens[i] + (i + 1 < tokens.size() ? " " : "");
            }
            try { 
                cout << "-> " << evaluate_rpn(expr) << "\n"; 
            } catch (const exception& e) { 
                cerr << "-> ERROR: " << e.what() << "\n"; 
            }

        // Задание 2: SET
        } else if (cmd == "SETADD" && tokens.size() >= 3) {
            if (sets.find(tokens[1]) == sets.end()) {
                sets[tokens[1]].init();
            }
            cout << "-> " << (sets[tokens[1]].add(tokens[2]) ? "OK" : "EXISTS") << "\n";
        } else if (cmd == "SETDEL" && tokens.size() >= 3) {
            if (sets.find(tokens[1]) != sets.end()) {
                cout << "-> " << (sets[tokens[1]].remove(tokens[2]) ? "OK" : "NOT_FOUND") << "\n";
            } else {
                cout << "-> NOT_FOUND\n";
            }
        } else if (cmd == "SET_AT" && tokens.size() >= 3) {
            bool found = (sets.find(tokens[1]) != sets.end() && sets[tokens[1]].contains(tokens[2]));
            cout << "-> " << (found ? "TRUE" : "FALSE") << "\n";
        } else if (cmd == "SETPRINT" && tokens.size() >= 2) {
            if (sets.find(tokens[1]) != sets.end()) {
                sets[tokens[1]].print();
            } else {
                cout << "{ }\n";
            }

        // Задание 3: Partition
        } else if (cmd == "SUBSETS") {
            vector<int> nums;
            for (size_t i = 1; i < tokens.size(); ++i) {
                nums.push_back(stoi(tokens[i]));
            }
            PartitionResult r = difference(nums);
            cout << "Подмножество 1: { "; 
            for (int x : r.subset1) {
                cout << x << " "; 
            }
            cout << "} (Сумма: " << r.sum1 << ")\n";

            cout << "Подмножество 2: { "; 
            for (int x : r.subset2) {
                cout << x << " "; 
            }
            cout << "} (Сумма: " << r.sum2 << ")\n";
            cout << "Минимальная разница: " << r.diff << "\n";

        // Задание 4: Subarray Sum
        } else if (cmd == "MPUSH" && tokens.size() >= 3) {
            if (arrays.find(tokens[1]) == arrays.end()) {
                arrays[tokens[1]].init();
            }
            arrays[tokens[1]].push_back(tokens[2]);
            cout << "-> " << tokens[2] << "\n";
        } else if (cmd == "FINDSUM" && tokens.size() >= 3) {
            if (arrays.find(tokens[1]) != arrays.end()) {
                Subarray res = Sum(arrays[tokens[1]], stoi(tokens[2]));
                if (res.found) {
                    cout << "-> Найдено с " << res.start_index << " по " << res.end_index << ": [ ";
                    for (int x : res.elements) {
                        cout << x << " ";
                    }
                    cout << "]\n";
                } else {
                    cout << "-> Не найдено\n";
                }
            }

        // Задание 5: BST
        } else if (cmd == "TINSERT" && tokens.size() >= 2) {
            tree.insert(tokens[1]);
            cout << "-> " << tokens[1] << "\n";
        } else if (cmd == "TRECOVER") {
            recover_tree(tree.root);
            cout << "-> Дерево восстановлено\n";
        } else if (cmd == "TPRINT") {
            tree.print();

        // Задание 6: Иерархия сотрудников
        } else if (cmd == "EMP_COUNT") {
            vector<string> pairs;
            for (size_t i = 1; i < tokens.size(); ++i) {
                pairs.push_back(tokens[i]);
            }
            EmployeeStats st = count_employees(pairs.data(), pairs.size());

            bool first = true;
            for (const auto& p : st.subordinate_counts) {
                if (!first) cout << ", ";
                cout << p.first << "-" << p.second;
                first = false;
            }
            cout << "\n";

        // Задание 6: Бенчмарк хеширования
        } else if (cmd == "HASH_BENCH") {
            run_hash_benchmark();

        // Задание 7: LFU-кэш
        } else if (cmd == "LFU_INIT" && tokens.size() >= 2) {
            lfu.init(stoi(tokens[1]));
            cout << "-> LFU емкость установлена в " << tokens[1] << "\n";
        } else if ((cmd == "SET" || cmd == "LFU_PUT") && tokens.size() >= 3) {
            lfu.set(tokens[1], tokens[2]);
        } else if ((cmd == "GET" || cmd == "LFU_GET") && tokens.size() >= 2) {
            cout << lfu.get(tokens[1]) << " ";
        } else if (cmd == "LFU_PRINT") {
            lfu.print();

        } else {
            cout << "Неизвестная команда: " << cmd << "\n";
        }
    } // конец while

    cout << "\n";
    return 0;
} // конец main