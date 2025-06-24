#pragma once

#include <cstddef>
#include <iterator>

namespace axis {

struct IntrusiveListDefaultTag {};

template<typename T, typename Tag = IntrusiveListDefaultTag>
struct IntrusiveListNode {
    auto is_linked [[nodiscard]] () const noexcept -> bool {
        return _next != nullptr;
    }

    auto unlink() noexcept -> void {
        if (_next) {
            _next->_prev = _prev;
        }
        if (_prev) {
            _prev->_next = _next;
        }
        _next = _prev = nullptr;
    }

    auto link_before(IntrusiveListNode* next) noexcept -> void {
        _prev = next->_prev;
        _prev->_next = this;
        _next = next;
        next->_prev = this;
    }

    auto item [[nodiscard]] () noexcept -> T* {
        return static_cast<T*>(this);
    }

    IntrusiveListNode() = default;

    // Non copyable
    IntrusiveListNode(const IntrusiveListNode&) = delete;
    auto operator=(const IntrusiveListNode&) -> IntrusiveListNode& = delete;

    IntrusiveListNode(IntrusiveListNode&&) noexcept = default;
    auto operator=(IntrusiveListNode&&) noexcept
        -> IntrusiveListNode& = default;

    virtual ~IntrusiveListNode() {
        unlink();
    }

    IntrusiveListNode* _next = nullptr;
    IntrusiveListNode* _prev = nullptr;
};

template<class T, typename Tag = IntrusiveListDefaultTag>
class IntrusiveList final: private IntrusiveListNode<T, Tag> {
    template<class Node, class Item>
    class IteratorImpl {
      public:
        using iterator_category = std::bidirectional_iterator_tag; // NOLINT
        using value_type = Item; // NOLINT
        using difference_type = ptrdiff_t; // NOLINT
        using pointer = value_type*; // NOLINT
        using reference = value_type&; // NOLINT

        explicit IteratorImpl(Node* val) noexcept : _current(val) {}

        auto operator++() -> IteratorImpl& {
            _current = _current->_next;
            return *this;
        }

        auto operator++(int) -> IteratorImpl {
            auto temp = *this;
            _current = _current->_next;
            return temp;
        }

        auto operator--() -> IteratorImpl& {
            _current = _current->_prev;
            return *this;
        }

        auto operator--(int) -> IteratorImpl {
            auto temp = *this;
            _current = _current->_prev;
            return temp;
        }

        auto operator*() const -> reference {
            return *static_cast<pointer>(_current);
        }

        auto operator->() const -> pointer {
            return static_cast<pointer>(_current);
        }

        auto operator==(const IteratorImpl& other) const noexcept -> bool {
            return _current == other._current;
        }

        auto operator!=(const IteratorImpl& other) const noexcept -> bool {
            return _current != other._current;
        }

      private:
        Node* _current;
    };

  public:
    using Iterator = IteratorImpl<IntrusiveListNode<T, Tag>, T>;
    using ConstIterator =
        IteratorImpl<const IntrusiveListNode<T, Tag>, const T>;

    auto begin [[nodiscard]] () -> Iterator {
        return Iterator {this->_next};
    }

    auto end [[nodiscard]] () -> Iterator {
        return Iterator {static_cast<IntrusiveListNode<T, Tag>*>(this)};
    }

    auto begin [[nodiscard]] () const -> ConstIterator {
        return ConstIterator {
            static_cast<const IntrusiveListNode<T, Tag>*>(this->_next)
        };
    }

    auto end [[nodiscard]] () const -> ConstIterator {
        return ConstIterator {static_cast<const IntrusiveListNode<T, Tag>*>(this
        )};
    }

  public:
    IntrusiveList() {
        this->_next = this->_prev = this;
    };

    // must unlink all elements from list
    ~IntrusiveList() {
        while (pop_back()) {}
    }

    IntrusiveList(const IntrusiveList&) = delete;
    auto operator=(const IntrusiveList&) -> IntrusiveList& = delete;

    IntrusiveList(IntrusiveList&& other) noexcept {
        this->_next = this->_prev = this;
        append(other);
    };

    auto operator=(IntrusiveList&& other) noexcept -> IntrusiveList& {
        while (pop_back()) {}
        append(other);
        return *this;
    }

  public:
    auto is_empty [[nodiscard]] () const noexcept -> bool {
        return this->_next == this;
    }

    // this method is allowed to be O(n)
    auto size() const -> std::size_t {
        return std::distance(begin(), end());
    }

  public:
    // note that IntrusiveList doesn't own elements,
    // and never copies or moves T
    auto push_back(T* elem) noexcept -> void {
        elem->link_before(this);
    }

    auto push_front(T* elem) noexcept -> void {
        elem->link_before(this->_next);
    }

    auto pop_back() noexcept -> T* {
        if (is_empty()) {
            return nullptr;
        }
        auto* back = this->_prev;
        back->unlink();
        return back->item();
    }

    auto pop_front() noexcept -> T* {
        if (is_empty()) {
            return nullptr;
        }
        auto* front = this->_next;
        front->unlink();
        return front->item();
    }

    auto append(IntrusiveList& other) noexcept -> void {
        auto* other_front = other._next;
        auto* other_back = other._prev;

        other_back->_next = this;
        other_front->_prev = this->_prev;

        auto* back = this->_prev;

        this->_prev = other_back;
        back->_next = other_front;

        other._prev = other._next = &other;
    }
};
} // namespace axis
