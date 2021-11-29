//
// Created by kevin on 11/27/21.
//
#include "queue.h"
#include <future>
#include <iostream>
#include <vector>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main() {
  std::mutex mutex;
  queue<int> my_queue;
  auto add_element = [&my_queue](const int i) -> void { my_queue.push(i); };
  auto remove_element = [&my_queue, &mutex]() -> void {
    while (true) {
      auto ele = my_queue.wait_and_pop();
      std::lock_guard<std::mutex> lk(mutex);
      if (ele) {
        std::cout << *ele << "\n";
      }
    }
  };
  std::vector<std::future<void>> async_results;
  async_results.reserve(1000);
  for (int i = 0; i < 1000; i++) {
    async_results.emplace_back(std::async(std::launch::async, add_element, i));
  }
  async_results.emplace_back(std::async(std::launch::async, remove_element));
  for (auto &res : async_results) {
    res.wait();
  }
}
#pragma clang diagnostic pop
