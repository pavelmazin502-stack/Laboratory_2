#include "../h.filesLab2/stack.h"
#include "../h.filesLab1/Stack.h"

#include <string>
#include <stdexcept>

using namespace std;

double evaluate_rpn(const string& expression){
    Stack st;
    st.init(); // Обнуление указателей

    string token = ""; // временное хранилище под одно слово
    
    for (size_t i = 0; i <= expression.size(); i++){
        if (i < expression.size() && expression[i] != ' '){
            token += expression[i];
        } else {
            if (!token.empty()){
                if (token == "+" || token == "-" || token == "*" || token == "/"){
                    double b = stod(st.pop()); // Снимаем с вершины стека
                    double a = stod(st.pop()); // Снимаем
                    double res = 0.0; 
                    
                    if (token == "+"){
                        res = a + b;
                    } else if (token == "-"){
                        res = a-b;
                    } else if (token == "*"){
                        res = a * b;
                    } else if (token == "/"){
                        if (b == 0.0){
                            st.destroy();
                            throw runtime_error("Деление на ноль!");
                        }
                        res = a / b;
                    }
                    st.push(to_string(res));
                } else {
                    st.push(token);
                }
            }
            token = "";
        }
    }
    double final_res = stod(st.pop());
    st.destroy(); // Освобождение памяти списка стека

    return final_res;
}
