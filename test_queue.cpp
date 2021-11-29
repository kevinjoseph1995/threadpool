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
  std::atomic<int> count;
  std::mutex mutex;
  queue<int> my_queue;
  auto add_element = [&my_queue](const int i) -> void {
    std::this_thread::sleep_for(std::chrono::microseconds(rand() % 1000));
    my_queue.push(i);
  };
  auto remove_element = [&my_queue, &mutex, &count]() -> void {
    while (true) {
      auto ele = my_queue.wait_and_pop();
      std::lock_guard<std::mutex> lk(mutex);
      if (ele) {
        count++;
        std::cout << "count:" << count << " value: " << *ele << "\n";
      }
    }
  };
  std::vector<std::future<void>> async_results;
  async_results.reserve(1000);
  for (int i = 0; i < 1000; i++) {
    async_results.emplace_back(std::async(std::launch::async, add_element, i));
  }
  auto pop_thread_future = std::async(std::launch::async, remove_element);
  for (auto &res : async_results) {
    res.wait();
  }
  pop_thread_future.wait();
}
#pragma clang diagnostic pop
