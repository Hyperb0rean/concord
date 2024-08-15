#pragma once

#include <cstddef>
#include <iterator>

namespace axis {

struct IntrusiveListDefaultTag {};

template <typename T, typename Tag = IntrusiveListDefaultTag>
struct IntrusiveListNode {
  auto IsLinked [[nodiscard]] () const noexcept -> bool {
    return next_ != nullptr;
  }

  auto Unlink() noexcept -> void {
    if (next_) {
      next_->prev_ = prev_;
    }
    if (prev_) {
      prev_->next_ = next_;
    }
    next_ = prev_ = nullptr;
  }

  auto LinkBefore(IntrusiveListNode* next) noexcept -> void {
    prev_ = next->prev_;
    prev_->next_ = this;
    next_ = next;
    next->prev_ = this;
  }

  auto Item [[nodiscard]] () noexcept -> T* {
    return static_cast<T*>(this);
  }

  IntrusiveListNode() = default;

  // Non copyable
  IntrusiveListNode(const IntrusiveListNode&) = delete;
  auto operator=(const IntrusiveListNode&) -> IntrusiveListNode& = delete;

  IntrusiveListNode(IntrusiveListNode&&) noexcept = default;
  auto operator=(IntrusiveListNode&&) noexcept -> IntrusiveListNode& = default;

  virtual ~IntrusiveListNode() {
    Unlink();
  }

  IntrusiveListNode* next_ = nullptr;
  IntrusiveListNode* prev_ = nullptr;
};

template <class T, typename Tag = IntrusiveListDefaultTag>
class IntrusiveList final : private IntrusiveListNode<T, Tag> {
  template <class Node, class Item>
  class IteratorImpl {
   public:
    using iterator_category = std::bidirectional_iterator_tag;  // NOLINT
    using value_type = Item;                                    // NOLINT
    using difference_type = ptrdiff_t;                          // NOLINT
    using pointer = value_type*;                                // NOLINT
    using reference = value_type&;                              // NOLINT

    IteratorImpl() = default;
    IteratorImpl(const IteratorImpl& it) = default;
    IteratorImpl(IteratorImpl&& it) = default;
    auto operator=(const IteratorImpl& it) -> IteratorImpl& = default;
    auto operator=(IteratorImpl&& it) -> IteratorImpl& = default;
    ~IteratorImpl() = default;

    explicit IteratorImpl(Node* val) noexcept
        : current_(val) {
    }

    auto operator++() -> IteratorImpl& {
      current_ = current_->next_;
      return *this;
    }
    auto operator++(int) -> IteratorImpl {
      auto temp = *this;
      current_ = current_->next_;
      return temp;
    }
    auto operator--() -> IteratorImpl& {
      current_ = current_->prev_;
      return *this;
    }
    auto operator--(int) -> IteratorImpl {
      auto temp = *this;
      current_ = current_->prev_;
      return temp;
    }

    auto operator*() const -> reference {
      return *static_cast<pointer>(current_);
    }

    auto operator->() const -> pointer {
      return static_cast<pointer>(current_);
    }

    auto operator==(const IteratorImpl& other) const noexcept -> bool {
      return current_ == other.current_;
    }
    auto operator!=(const IteratorImpl& other) const noexcept -> bool {
      return current_ != other.current_;
    }

   private:
    Node* current_;
  };

 public:
  using Iterator = IteratorImpl<IntrusiveListNode<T, Tag>, T>;
  using ConstIterator = IteratorImpl<const IntrusiveListNode<T, Tag>, const T>;

  auto Begin [[nodiscard]] () -> Iterator {
    return Iterator{this->next_};
  }
  auto End [[nodiscard]] () -> Iterator {
    return Iterator{static_cast<IntrusiveListNode<T, Tag>*>(this)};
  }

  auto Begin [[nodiscard]] () const -> ConstIterator {
    return ConstIterator{
        static_cast<const IntrusiveListNode<T, Tag>*>(this->next_)};
  }
  auto End [[nodiscard]] () const -> ConstIterator {
    return ConstIterator{static_cast<const IntrusiveListNode<T, Tag>*>(this)};
  }

 public:
  IntrusiveList() {
    this->next_ = this->prev_ = this;
  };

  // must unlink all elements from list
  ~IntrusiveList() {
    while (PopBack()) {
    }
  }

  IntrusiveList(const IntrusiveList&) = delete;
  auto operator=(const IntrusiveList&) -> IntrusiveList& = delete;

  IntrusiveList(IntrusiveList&& other) noexcept {
    this->next_ = this->prev_ = this;
    Append(other);
  };
  auto operator=(IntrusiveList&& other) noexcept -> IntrusiveList& {
    while (PopBack()) {
    }
    Append(other);
    return *this;
  }

 public:
  auto IsEmpty [[nodiscard]] () const noexcept -> bool {
    return this->next_ == this;
  }

  // this method is allowed to be O(n)
  auto Size() const -> std::size_t {
    return std::distance(Begin(), End());
  }

 public:
  // note that IntrusiveList doesn't own elements,
  // and never copies or moves T
  auto PushBack(T* elem) -> void {
    elem->LinkBefore(this);
  }
  auto PushFront(T* elem) -> void {
    elem->LinkBefore(this->next_);
  }

  auto PopBack() -> T* {
    if (IsEmpty()) {
      return nullptr;
    }
    auto* back = this->prev_;
    back->Unlink();
    return back->Item();
  }
  auto PopFront() -> T* {
    if (IsEmpty()) {
      return nullptr;
    }
    auto* front = this->next_;
    front->Unlink();
    return front->Item();
  }

  auto Append(IntrusiveList& other) noexcept -> void {
    auto* other_front = other.next_;
    auto* other_back = other.prev_;

    other_back->next_ = this;
    other_front->prev_ = this->prev_;

    auto* back = this->prev_;

    this->prev_ = other_back;
    back->next_ = other_front;

    other.prev_ = other.next_ = &other;
  }
};
}  // namespace axis

template <typename T, typename Tag>
auto begin(axis::IntrusiveList<T, Tag>& list)  // NOLINT
    -> axis::IntrusiveList<T, Tag>::Iterator {
  return list.Begin();
}

template <typename T, typename Tag>
auto end(axis::IntrusiveList<T, Tag>& list)  // NOLINT
    -> axis::IntrusiveList<T, Tag>::Iterator {
  return list.End();
}

template <typename T, typename Tag>
auto begin(const axis::IntrusiveList<T, Tag>& list)  // NOLINT
    -> axis::IntrusiveList<T, Tag>::ConstIterator {
  return list.Begin();
}

template <typename T, typename Tag>
auto end(const axis::IntrusiveList<T, Tag>& list)  // NOLINT
    -> axis::IntrusiveList<T, Tag>::ConstIterator {
  return list.End();
}
