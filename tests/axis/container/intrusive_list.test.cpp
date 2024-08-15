#include <gtest/gtest.h>
#include <axis/container/intrusive_list.hpp>

using axis::IntrusiveList;
using axis::IntrusiveListNode;

struct Item : public IntrusiveListNode<Item> {
  explicit Item(int i)
      : i{i},
        blob('a', 100) {
    blob += std::to_string(i);
  }

  int i;
  std::string blob;  // put nontrivial field here, hopefully incorrect code
                     // will hit double free

  Item(const Item&) = delete;
  auto operator=(const Item&) -> Item& = delete;
};

template <class... Args>
auto MakeList(Args*... args) {
  IntrusiveList<std::common_type_t<Args...>> list;
  (list.PushBack(args), ...);
  return list;
}

TEST(IntrusiveListTest, EmptyListWorks) {
  IntrusiveList<Item> list;
  ASSERT_TRUE(list.IsEmpty());
  ASSERT_TRUE(list.Size() == 0);

  Item item{1};
  ASSERT_FALSE(item.IsLinked());
}