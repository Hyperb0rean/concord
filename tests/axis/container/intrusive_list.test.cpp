#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <axis/container/intrusive_list.hpp>

using axis::IntrusiveList;
using axis::IntrusiveListNode;

struct Item: IntrusiveListNode<Item> {
    explicit Item(int i) : i {i}, blob('a', 100) {
        blob += std::to_string(i);
    }

    int i;
    std::string blob;

    Item(const Item&) = delete;
    auto operator=(const Item&) -> Item& = delete;
};

struct EmptyItem: IntrusiveListNode<EmptyItem> {};

template<class... Args>
auto make_list(Args*... args) {
    IntrusiveList<std::common_type_t<Args...>> list;
    (list.push_back(args), ...);
    return list;
}

TEST(IntrusiveListTest, EmptyList) {
    IntrusiveList<Item> list;
    ASSERT_TRUE(list.is_empty());
    ASSERT_TRUE(list.size() == 0);

    Item item {1};
    ASSERT_FALSE(item.is_linked());
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

TEST(IntrusiveListTest, push_back) {
    Item item1 {1}, item2 {2};
    IntrusiveList<Item> list;
    list.push_back(&item1);
    ASSERT_FALSE(list.is_empty());
    list.push_back(&item2);
    ASSERT_TRUE(list.size() == 2);
}

TEST(IntrusiveListTest, push_front) {
    Item item1 {1}, item2 {2};
    IntrusiveList<Item> list;
    list.push_front(&item1);
    ASSERT_FALSE(list.is_empty());
    list.push_front(&item2);
    ASSERT_TRUE(list.size() == 2);
}

TEST(IntrusiveListTest, Iteration) {
    Item item1 {1}, item2 {2}, item3 {3};
    auto list = make_list(&item1, &item2, &item3);

    auto i = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
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

TEST(IntrusiveListTest, unlink) {
    Item item1 {1}, item2 {2}, item3 {3};
    auto list = make_list(&item1, &item2, &item3);
    item3.unlink();
    ASSERT_TRUE(list.size() == 2);
    ASSERT_FALSE(list.is_empty());

    item1.unlink();
    ASSERT_TRUE(list.size() == 1);
    ASSERT_FALSE(list.is_empty());

    item2.unlink();
    ASSERT_TRUE(list.size() == 0);
    ASSERT_TRUE(list.is_empty());
}

TEST(IntrusiveListTest, Destructor) {
    Item item1 {1}, item2 {2}, item3 {3};
    {
        auto list = make_list(&item1, &item2, &item3);
        ASSERT_TRUE(item1.is_linked());
        ASSERT_TRUE(item2.is_linked());
        ASSERT_TRUE(item3.is_linked());
    }
    ASSERT_FALSE(item1.is_linked());
    ASSERT_FALSE(item2.is_linked());
    ASSERT_FALSE(item3.is_linked());
}

TEST(IntrusiveListTest, IterOperations) {
    Item item1 {10}, item2 {20}, item3 {30}, item4 {40};
    auto list = make_list(&item1, &item2, &item3, &item4);
    ASSERT_TRUE(std::next(list.begin(), 4) == list.end());
    ASSERT_TRUE(std::prev(list.end(), 4) == list.begin());

    auto it = list.begin();
    ASSERT_TRUE((it++)->i == 10);
    ASSERT_TRUE((++it)->i == 30);

    it = std::prev(list.end());
    ASSERT_TRUE((it--)->i == 40);
    ASSERT_TRUE((--it)->i == 20);

    it = list.begin();
    Item front {0};
    list.push_front(&front);
    ASSERT_TRUE(std::prev(it) == list.begin());

    it = std::prev(list.end());
    Item back {0};
    list.push_back(&back);
    ASSERT_TRUE(std::next(it) == std::prev(list.end()));
}

TEST(IntrusiveListTest, IterValidation) {
    Item item1 {10}, item2 {20}, item3 {30}, item4 {40}, item5 {50};
    auto list = make_list(&item1, &item2, &item3, &item4, &item5);
    auto it1 = list.begin();
    auto it2 = std::next(it1);
    auto it5 = std::prev(list.end());
    auto it4 = std::prev(it5);
    auto it3 = std::next(it2);

    ASSERT_TRUE(it2 == std::prev(it3));
    ASSERT_TRUE(it3 == std::next(it2));

    for (auto i = 0; const auto& it : {it1, it2, it3, it4, it5}) {
        ASSERT_TRUE(it->i == 10 * ++i);
        ASSERT_TRUE((*it).i == 10 * i);
    }

    item3.unlink();
    ASSERT_TRUE(list.size() == 4);
    ASSERT_TRUE(it2 == std::prev(it4));
    ASSERT_TRUE(it4 == std::next(it2));

    auto list2 = std::move(list);
    item1.unlink();
    ASSERT_TRUE(list2.size() == 3);
    ASSERT_TRUE(it2 == list2.begin());
    ASSERT_TRUE(it4 == std::next(it2));
    ASSERT_TRUE(it4 == std::prev(list2.end(), 2));

    list = std::move(list2);
    item5.unlink();
    ASSERT_TRUE(list.size() == 2);
    ASSERT_TRUE(it2 == list.begin());
    ASSERT_TRUE(it4 == std::prev(list.end()));

    it2->unlink();
    item4.unlink();
    ASSERT_TRUE(list.size() == 0);
    ASSERT_TRUE(list.begin() == list.end());
    for (const auto* p : {&item1, &item2, &item3, &item4, &item5}) {
        ASSERT_FALSE(p->is_linked());
    }
}

TEST(IntrusiveListTest, Move) {
    Item item1 {1}, item2 {2}, item3 {3};
    auto list1 = make_list(&item1, &item2, &item3);

    auto list2 = std::move(list1);
    ASSERT_FALSE(list2.is_empty());
    ASSERT_TRUE(list1.is_empty());

    list1 = std::move(list2);
    ASSERT_FALSE(list1.is_empty());
    ASSERT_TRUE(list2.is_empty());
}

// TODO: Add tagged tests
