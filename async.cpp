/*
 * 使用无栈协程实现异步调用链
 */


#include "coroutine_utils.hpp"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

using namespace std::chrono;

class async_work : public coroutine_base, public std::enable_shared_from_this<async_work> {
public:
    async_work(std::string name) : _name(std::move(name)) {}

    void operator()() {
        COROUTINE_BEGIN
        COROUTINE_YIELD(run());
        std::cout << _name << " work 1 done. back to thread: " << std::this_thread::get_id() << std::endl;
        COROUTINE_YIELD(run());
        std::cout << _name << " work 2 done. back to thread: " << std::this_thread::get_id() << std::endl;
        COROUTINE_YIELD(run());
        std::cout << _name << " work 3 done. back to thread: " << std::this_thread::get_id() << std::endl;
        COROUTINE_YIELD(run());
        std::cout << _name << " work 4 done. back to thread: " << std::this_thread::get_id() << std::endl;
        COROUTINE_YIELD(run());
        std::cout << _name << " work 5 done. back to thread: " << std::this_thread::get_id() << std::endl;
        COROUTINE_END
    }

    void run() {
        std::thread([](auto ptr) {
            std::cout << ptr->_name << " working..." << std::endl;
            std::this_thread::sleep_for(1s);
            (*ptr)();
        }, shared_from_this()).detach();
    }

private:
    std::string _name;
};

int main() {
    std::cout << "launch w1" << std::endl;
    auto w1 = std::make_shared<async_work>("w1");
    (*w1)();

    std::this_thread::sleep_for(0.5s);
    {
        std::cout << "launch w2" << std::endl;
        auto w2 = std::make_shared<async_work>("w2");
        (*w2)();
    }

    std::cout << "waiting..." << std::endl;
    std::this_thread::sleep_for(10s);
}
