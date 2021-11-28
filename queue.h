//
// Created by kevin on 11/27/21.
//

#ifndef THREAD_POOL_QUEUE_H
#define THREAD_POOL_QUEUE_H

#include <memory>
#include <concepts>

template<std::copy_constructible T>
class queue {
private:
    struct node {
        T data_;
        std::unique_ptr<node> next_;

        explicit node(T data) : data_(std::move(data)) {}
    };

    std::unique_ptr<node> head_ = nullptr;
    node *tail_ = nullptr;
public:
    queue() = default;

    queue(const queue &other) = delete;

    queue &operator=(const queue &other) = delete;

    std::shared_ptr<T> try_pop() {
        if (!head_) {
            // Queue empty
            return nullptr;
        } else {
            // Move data out of head
            const auto ret = std::make_shared<T>(std::move(head_->data_));
            head_ = std::move(head_->next_);
            if (!head_) { tail_ = nullptr; }
            return ret;
        }

    }

    void push(T value) {
        auto new_node = std::make_unique<node>(value);
        node *new_tail = new_node.get();
        if (tail_) {
            tail_->next_ = std::move(new_node);
        } else {
            head_ = std::move(new_node);
        }
        tail_ = new_tail;
    }

    [[nodiscard]] bool empty() {
        return !head_;
    }
};


#endif //THREAD_POOL_QUEUE_H
