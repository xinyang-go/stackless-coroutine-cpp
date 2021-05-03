cmake_minimum_required(VERSION 3.12)
project(stackless_coroutine_goto)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

add_executable(stackless_coroutine_generator generator.cpp)

add_executable(stackless_coroutine_async async.cpp)
target_link_libraries(stackless_coroutine_async Threads::Threads)

add_executable(stackless_coroutine_get_value get_value.cpp)