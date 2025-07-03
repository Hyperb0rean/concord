#pragma once

#include <atomic>
#include <utility>

#include "axis/container/intrusive_list.hpp"

namespace concord::cord::sync::detail {
template<typename T>
class MPSCQueue {
    using Node = axis::IntrusiveListNode<T>;

  public:
    auto push(T* new_head) -> void {
        Node* old_head = head();
        do {
            static_cast<Node*>(new_head)->_next = old_head;
        } while (!_head.compare_exchange_weak(
            old_head,
            new_head,
            std::memory_order::acq_rel,
            std::memory_order::acquire
        ));
    }

    auto pop() -> T* {
        if (tail() == nullptr) {
            clear([this](T* item) {
                Node* new_tail = static_cast<Node*>(item);
                new_tail->_next = _tail;
                _tail = new_tail;
            });
        }
        if (tail() == nullptr) {
            return nullptr;
        }

        Node* next_tail = _tail->_next;
        T* item = _tail->item();
        _tail = next_tail;
        return item;
    }

  private:
    auto head [[nodiscard]] () const noexcept -> Node* {
        return _head.load(std::memory_order::acquire);
    }

    auto tail [[nodiscard]] () const noexcept -> Node* {
        return _tail;
    }

    template<class F>
    auto clear(F&& callback) -> void {
        Node* tail = _head.exchange(nullptr);
        while (tail != nullptr) {
            Node* next_tail = tail->_next;
            callback(tail->item());
            tail = next_tail;
        }
    }

    Node* _tail {nullptr};
    std::atomic<Node*> _head {nullptr};
};

} // namespace concord::cord::sync::detail