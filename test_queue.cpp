//
// Created by kevin on 11/27/21.
//
#include "queue.h"
#include <future>
#include <iostream>
#include <vector>

int main() {
  std::mutex mutex;
  queue<int> my_queue;
  auto add_element = [&my_queue](const int i) -> void { my_queue.push(i); };
  auto remove_element = [&my_queue, &mutex]() -> void {
    while (!my_queue.empty()) {
      auto ele = my_queue.try_pop();
      if (ele) {
        std::lock_guard<std::mutex> lk(mutex);
        std::cout << *ele << "\n";
      }
    }
  };
  std::vector<std::future<void>> async_results;
  for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) {
      async_results.emplace_back(std::async(std::launch::async, add_element, i));
    } else {
      async_results.emplace_back(
          std::async(std::launch::async, remove_element));
    }
  }
  for (auto &res : async_results) {
    res.wait();
  }
}
