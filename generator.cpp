/*
 * 使用无栈协程实现仿python的range生成器
 */


#include "coroutine_utils.h"
#include <iostream>

template<class T>
class range : public coroutine_base {
public:
    explicit range(T end) : _begin(0), _end(end), _step(1) {}

    range(T begin, T end) : _begin(begin), _end(end), _step(1) {}

    range(T begin, T end, T step) : _begin(begin), _end(end), _step(step) {}

    T operator()() {
        COROUTINE_CHECK(1);
        for (_i = _begin; _i < _end; _i += _step) {
            COROUTINE_YIELD(1, _i);
        }
        COROUTINE_RETURN(-1);
    }

private:
    T _begin, _end, _step;
    T _i;
};

int main() {
    {
        std::cout << "range(10): " << std::endl;
        auto r = range(10);
        for (auto i = r(); !r.done(); i = r()) std::cout << i << " ";
        std::cout << std::endl;
    }
    {
        std::cout << "range(1.2, 12.3, 2.3): " << std::endl;
        auto r = range(1.2, 12.3, 2.3);
        for (auto i = r(); !r.done(); i = r()) std::cout << i << " ";
        std::cout << std::endl;
    }
    return 0;
}
