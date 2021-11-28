//
// Created by kevin on 11/27/21.
//
#include <iostream>
#include "queue.h"

int main() {
    queue<int> my_queue;
    my_queue.push(5);
    my_queue.push(6);
    my_queue.push(7);
    my_queue.push(8);
    my_queue.push(9);
    my_queue.push(10);
    my_queue.push(11);


    while (auto val = my_queue.try_pop()) {
        std::cout << *val << "\n";
    }

    my_queue.push(5);
    my_queue.push(6);
    my_queue.push(7);
    my_queue.push(8);
    my_queue.push(9);
    my_queue.push(10);
    my_queue.push(11);


    while (auto val = my_queue.try_pop()) {
        std::cout << *val << "\n";
    }
}
