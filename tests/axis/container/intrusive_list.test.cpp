#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <axis/container/intrusive_list.hpp>

using axis::IntrusiveList;
using axis::IntrusiveListNode;

struct Item : IntrusiveListNode<Item> {
  explicit Item(int i)
      : i{i},
        blob('a', 100) {
    blob += std::to_string(i);
  }

  int i;
  std::string blob;

  Item(const Item&) = delete;
  auto operator=(const Item&) -> Item& = delete;
};

struct EmptyItem : IntrusiveListNode<EmptyItem> {};

template <class... Args>
auto MakeList(Args*... args) {
  IntrusiveList<std::common_type_t<Args...>> list;
  (list.PushBack(args), ...);
  return list;
}

TEST(IntrusiveListTest, EmptyList) {
  IntrusiveList<Item> list;
  ASSERT_TRUE(list.IsEmpty());
  ASSERT_TRUE(list.Size() == 0);

  Item item{1};
  ASSERT_FALSE(item.IsLinked());
}

TEST(IntrusiveListTest, StaticChecks) {
  static_assert(std::default_initializable<IntrusiveListNode<EmptyItem>>);
  static_assert(!std::copy_constructible<IntrusiveListNode<EmptyItem>>);
  static_assert(std::move_constructible<IntrusiveListNode<EmptyItem>>);

  using L = IntrusiveList<EmptyItem>;
  static_assert(!std::copy_constructible<L>);
  static_assert(std::move_constructible<L>);
  static_assert(std::bidirectional_iterator<L::Iterator>);
}

TEST(IntrusiveListTest, PushBack) {
  Item item1{1}, item2{2};
  IntrusiveList<Item> list;
  list.PushBack(&item1);
  ASSERT_FALSE(list.IsEmpty());
  list.PushBack(&item2);
  ASSERT_TRUE(list.Size() == 2);
}

TEST(IntrusiveListTest, PushFront) {
  Item item1{1}, item2{2};
  IntrusiveList<Item> list;
  list.PushFront(&item1);
  ASSERT_FALSE(list.IsEmpty());
  list.PushFront(&item2);
  ASSERT_TRUE(list.Size() == 2);
}

TEST(IntrusiveListTest, Iteration) {
  Item item1{1}, item2{2}, item3{3};
  auto list = MakeList(&item1, &item2, &item3);

  auto i = 0;
  for (auto it = list.Begin(); it != list.End(); ++it) {
    ASSERT_TRUE(it->i == ++i);
  }
  ASSERT_TRUE(i == 3);

  i = 0;
  for (const auto& v : list) {
    ASSERT_TRUE(v.i == ++i);
  }
  ASSERT_TRUE(i == 3);

  i = 0;
  for (auto& v : list) {
    ASSERT_TRUE(v.i++ == ++i);
  }
  ASSERT_TRUE(i == 3);
}

TEST(IntrusiveListTest, Unlink) {
  Item item1{1}, item2{2}, item3{3};
  auto list = MakeList(&item1, &item2, &item3);
  item3.Unlink();
  ASSERT_TRUE(list.Size() == 2);
  ASSERT_FALSE(list.IsEmpty());

  item1.Unlink();
  ASSERT_TRUE(list.Size() == 1);
  ASSERT_FALSE(list.IsEmpty());

  item2.Unlink();
  ASSERT_TRUE(list.Size() == 0);
  ASSERT_TRUE(list.IsEmpty());
}

TEST(IntrusiveListTest, Destructor) {
  Item item1{1}, item2{2}, item3{3};
  {
    auto list = MakeList(&item1, &item2, &item3);
    ASSERT_TRUE(item1.IsLinked());
    ASSERT_TRUE(item2.IsLinked());
    ASSERT_TRUE(item3.IsLinked());
  }
  ASSERT_FALSE(item1.IsLinked());
  ASSERT_FALSE(item2.IsLinked());
  ASSERT_FALSE(item3.IsLinked());
}

TEST(IntrusiveListTest, IterOperations) {
  Item item1{10}, item2{20}, item3{30}, item4{40};
  auto list = MakeList(&item1, &item2, &item3, &item4);
  ASSERT_TRUE(std::next(list.Begin(), 4) == list.End());
  ASSERT_TRUE(std::prev(list.End(), 4) == list.Begin());

  auto it = list.Begin();
  ASSERT_TRUE((it++)->i == 10);
  ASSERT_TRUE((++it)->i == 30);

  it = std::prev(list.End());
  ASSERT_TRUE((it--)->i == 40);
  ASSERT_TRUE((--it)->i == 20);

  it = list.Begin();
  Item front{0};
  list.PushFront(&front);
  ASSERT_TRUE(std::prev(it) == list.Begin());

  it = std::prev(list.End());
  Item back{0};
  list.PushBack(&back);
  ASSERT_TRUE(std::next(it) == std::prev(list.End()));
}

TEST(IntrusiveListTest, IterValidation) {
  Item item1{10}, item2{20}, item3{30}, item4{40}, item5{50};
  auto list = MakeList(&item1, &item2, &item3, &item4, &item5);
  auto it1 = list.Begin();
  auto it2 = std::next(it1);
  auto it5 = std::prev(list.End());
  auto it4 = std::prev(it5);
  auto it3 = std::next(it2);

  ASSERT_TRUE(it2 == std::prev(it3));
  ASSERT_TRUE(it3 == std::next(it2));

  for (auto i = 0; const auto& it : {it1, it2, it3, it4, it5}) {
    ASSERT_TRUE(it->i == 10 * ++i);
    ASSERT_TRUE((*it).i == 10 * i);
  }

  item3.Unlink();
  ASSERT_TRUE(list.Size() == 4);
  ASSERT_TRUE(it2 == std::prev(it4));
  ASSERT_TRUE(it4 == std::next(it2));

  auto list2 = std::move(list);
  item1.Unlink();
  ASSERT_TRUE(list2.Size() == 3);
  ASSERT_TRUE(it2 == list2.Begin());
  ASSERT_TRUE(it4 == std::next(it2));
  ASSERT_TRUE(it4 == std::prev(list2.End(), 2));

  list = std::move(list2);
  item5.Unlink();
  ASSERT_TRUE(list.Size() == 2);
  ASSERT_TRUE(it2 == list.Begin());
  ASSERT_TRUE(it4 == std::prev(list.End()));

  it2->Unlink();
  item4.Unlink();
  ASSERT_TRUE(list.Size() == 0);
  ASSERT_TRUE(list.Begin() == list.End());
  for (const auto* p : {&item1, &item2, &item3, &item4, &item5}) {
    ASSERT_FALSE(p->IsLinked());
  }
}

TEST(IntrusiveListTest, Move) {
  Item item1{1}, item2{2}, item3{3};
  auto list1 = MakeList(&item1, &item2, &item3);

  auto list2 = std::move(list1);
  ASSERT_FALSE(list2.IsEmpty());
  ASSERT_TRUE(list1.IsEmpty());

  list1 = std::move(list2);
  ASSERT_FALSE(list1.IsEmpty());
  ASSERT_TRUE(list2.IsEmpty());
}

// TODO: Add tagged tests
