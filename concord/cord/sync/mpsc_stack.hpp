#pragma once

#include <atomic>
#include <utility>

#include "axis/container/intrusive_list.hpp"

namespace concord::cord::sync::detail {
template<typename T>
class MPSCStack {
    using Node = axis::IntrusiveListNode<T>;

  public:
    auto push(T* new_head) -> bool {
        Node* old_head = head();
        do {
            if (old_head == &_stub) {
                return false;
            }
            static_cast<Node*>(new_head)->_next = old_head;
        } while (!_head.compare_exchange_weak(
            old_head,
            new_head,
            std::memory_order::acq_rel,
            std::memory_order::acquire
        ));
        return true;
    }

    template<class F>
    auto close(F&& callback) -> void {
        Node* tail = _head.exchange(&_stub, std::memory_order::acq_rel);
        if (tail == &_stub) {
            return;
        }
        while (tail != nullptr) {
            Node* next_tail = tail->_next;
            callback(tail->item());
            tail = next_tail;
        }
    }

    auto is_closed [[nodiscard]] () const noexcept -> bool {
        return head() == &_stub;
    }

  private:
    auto head [[nodiscard]] () const noexcept -> Node* {
        return _head.load(std::memory_order::acquire);
    }

    Node _stub;
    std::atomic<Node*> _head {nullptr};
};

} // namespace concord::cord::sync::detail