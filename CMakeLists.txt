cmake_minimum_required(VERSION 3.20)
project(thread_pool)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-stdlib=libc++ -std=c++20)
add_link_options(-stdlib=libc++)

#add_library(thread_pool SHARED queue.cpp)
#if (MSVC)
#    target_compile_options(thread_pool PRIVATE /W4 /WX)
#else ()
#    target_compile_options(thread_pool PRIVATE -Wall -Wextra -Wpedantic -Werror)
#endif ()

add_executable(test_queue test_queue.cpp)
target_link_libraries(test_queue pthread)