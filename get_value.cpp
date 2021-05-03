/*
 * 从无栈协程的外部获取不同类型数据
 */

#include "coroutine_utils.h"
#include <iostream>

class worker : public coroutine_base {
public:
    worker() {
        // run to first yield
        (*this)(0);
    }

    template<class T>
    void operator()(T input) {
        COROUTINE_CHECK(4);

        std::cout << "waiting input for task 1..." << std::endl;
        COROUTINE_YIELD(1,);
        std::cout << "task 1, get input of [type: " << typeid(T).name() << " value:" << input << "]" << std::endl;

        std::cout << "waiting input for task 2..." << std::endl;
        COROUTINE_YIELD(2,);
        std::cout << "task 2, get input of [type: " << typeid(T).name() << " value:" << input << "]" << std::endl;

        std::cout << "waiting input for task 3..." << std::endl;
        COROUTINE_YIELD(3,);
        std::cout << "task 3, get input of [type: " << typeid(T).name() << " value:" << input << "]" << std::endl;

        std::cout << "waiting input for task 4..." << std::endl;
        COROUTINE_YIELD(4,);
        std::cout << "task 4, get input of [type: " << typeid(T).name() << " value:" << input << "]" << std::endl;

        COROUTINE_RETURN();
    }
};

int main() {
    auto w = worker();

    // do something...
    w(1);

    // do something...
    w(2.);

    // do something...
    w("3");

    // do something...
    w('4');
}
