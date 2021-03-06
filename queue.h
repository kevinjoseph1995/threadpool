//
// Created by kevin on 11/27/21.
//

#ifndef THREAD_POOL_QUEUE_H
#define THREAD_POOL_QUEUE_H

#include <concepts>
#include <memory>
#include <mutex>

template <std::copy_constructible T> class queue {
public:
  // Initialize with dummy node at the beginning. The invariance that is
  // guaranteed at the end of every queue operation is that the "tail_" pointer
  // always points to a dummy node at the end of the linked list
  queue() : head_(std::make_unique<node>()), tail_(head_.get()) {}

  queue(const queue &other) = delete;

  queue &operator=(const queue &other) = delete;

  [[nodiscard]] std::unique_ptr<T> try_pop() {
    std::lock_guard<std::mutex> lk(head_mutex_);
    // Check if the current tail is the same as head.
    // In the scenario where there was a concurrent push call and head and tail
    // are the same, note that by using the get_tail helper that's protected by
    // a mutex we get a tail that was ordered before or after the push. If it
    // was before we try_pop -> fail and return. If it was ordered after the
    // push we would pop_out that node or some other subsequently pushed node.
    if (head_.get() == get_tail()) {
      return nullptr;
    } else {
      auto ret = std::move(head_->data_);
      head_ = std::move(head_->next_);
      return std::move(ret);
    }
  }

  [[nodiscard]] std::unique_ptr<T> wait_and_pop() {
    std::unique_lock<std::mutex> lk(head_mutex_);
    data_cv_.template wait(lk,
                           [&]() -> bool { return head_.get() != get_tail(); });
    auto ret = std::move(head_->data_);
    head_ = std::move(head_->next_);
    return std::move(ret);
  }

  void push(T value) {
    auto new_node = std::make_unique<node>();
    new_node->data_ = nullptr;
    auto *new_tail = new_node.get();
    {
      std::lock_guard<std::mutex> lk(tail_mutex_);
      tail_->data_ = std::make_unique<T>(std::move(value));
      tail_->next_ = std::move(new_node);
      tail_ = new_tail;
    }
    data_cv_.notify_one();
  }

  [[nodiscard]] bool empty() {
    std::lock_guard<std::mutex> lk(head_mutex_);
    return head_.get() == get_tail();
  }

private:
  struct node {
    std::unique_ptr<T> data_;
    std::unique_ptr<node> next_;
  };
  mutable std::mutex head_mutex_;
  std::unique_ptr<node> head_ = nullptr;

  mutable std::mutex tail_mutex_;
  node *tail_ = nullptr;
  std::condition_variable data_cv_;

private:
  [[nodiscard]] node *get_tail() const {
    std::lock_guard<std::mutex> lk(std::mutex);
    return tail_;
  }
};

#endif // THREAD_POOL_QUEUE_H
