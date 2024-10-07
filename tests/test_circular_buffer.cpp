
// g++ --std=c++11 test.cpp

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "fx/util/circular_buffer.h"

TEST_CASE("circular_buffer basic operations") {
    CircularBuffer<int> buffer(5);

    CHECK(buffer.empty());
    CHECK_EQ(buffer.size(), 0);

    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);

    CHECK_EQ(buffer.size(), 3);
    CHECK_FALSE(buffer.empty());
    CHECK_FALSE(buffer.full());

    CHECK_EQ(buffer.front(), 1);
    CHECK_EQ(buffer.back(), 3);

    CHECK_EQ(buffer.pop_front(), 1);
    CHECK_EQ(buffer.size(), 2);
    CHECK_EQ(buffer.front(), 2);
}

TEST_CASE("circular_buffer overflow behavior") {
    CircularBuffer<int> buffer(3);

    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);
    CHECK(buffer.full());

    buffer.push_back(4);
    CHECK(buffer.full());
    CHECK_EQ(buffer.size(), 3);

    CHECK_EQ(buffer.pop_front(), 2);
    CHECK_EQ(buffer.pop_front(), 3);
    CHECK_EQ(buffer.pop_front(), 4);
    CHECK(buffer.empty());

    CHECK_EQ(buffer.pop_front(), 0);  // Returns default-constructed int (0) when empty
}

TEST_CASE("circular_buffer edge cases") {
    CircularBuffer<int> buffer(1);

    CHECK(buffer.empty());
    CHECK_FALSE(buffer.full());

    buffer.push_back(42);
    CHECK_FALSE(buffer.empty());
    CHECK(buffer.full());

    buffer.push_back(43);
    CHECK_EQ(buffer.front(), 43);
    CHECK_EQ(buffer.back(), 43);

    CHECK_EQ(buffer.pop_front(), 43);
    CHECK(buffer.empty());
}

TEST_CASE("circular_buffer clear operation") {
    CircularBuffer<int> buffer(5);

    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);

    CHECK_EQ(buffer.size(), 3);

    buffer.clear();

    CHECK(buffer.empty());
    CHECK_EQ(buffer.size(), 0);

    buffer.push_back(4);
    CHECK_EQ(buffer.front(), 4);
    CHECK_EQ(buffer.back(), 4);
}

TEST_CASE("circular_buffer indexing") {
    CircularBuffer<int> buffer(5);

    buffer.push_back(10);
    buffer.push_back(20);
    buffer.push_back(30);

    CHECK_EQ(buffer[0], 10);
    CHECK_EQ(buffer[1], 20);
    CHECK_EQ(buffer[2], 30);

    buffer.pop_front();
    buffer.push_back(40);

    CHECK_EQ(buffer[0], 20);
    CHECK_EQ(buffer[1], 30);
    CHECK_EQ(buffer[2], 40);
}

TEST_CASE("circular_buffer with custom type") {
    struct CustomType {
        int value;
        CustomType(int v = 0) : value(v) {}
        bool operator==(const CustomType& other) const { return value == other.value; }
    };

    CircularBuffer<CustomType> buffer(3);

    buffer.push_back(CustomType(1));
    buffer.push_back(CustomType(2));
    buffer.push_back(CustomType(3));

    CHECK_EQ(buffer.front().value, 1);
    CHECK_EQ(buffer.back().value, 3);

    buffer.push_back(CustomType(4));

    CHECK_EQ(buffer.pop_front().value, 2);
    CHECK_EQ(buffer.pop_front().value, 3);
    CHECK_EQ(buffer.pop_front().value, 4);
}
