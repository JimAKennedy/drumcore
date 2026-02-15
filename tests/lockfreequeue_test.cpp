#include <drumcore/lockfreequeue.h>
#include <gtest/gtest.h>

using namespace JKDigital;

TEST(LockFreeQueue, InitiallyEmpty) {
    LockFreeQueue<int, 16> queue;
    EXPECT_TRUE(queue.isEmpty());
    EXPECT_FALSE(queue.isFull());
    EXPECT_EQ(queue.size(), 0u);
}

TEST(LockFreeQueue, PushAndPop) {
    LockFreeQueue<int, 16> queue;
    EXPECT_TRUE(queue.push(42));
    EXPECT_EQ(queue.size(), 1u);

    int value = 0;
    EXPECT_TRUE(queue.pop(value));
    EXPECT_EQ(value, 42);
    EXPECT_TRUE(queue.isEmpty());
}

TEST(LockFreeQueue, PopFromEmpty_ReturnsFalse) {
    LockFreeQueue<int, 16> queue;
    int value = 0;
    EXPECT_FALSE(queue.pop(value));
}

TEST(LockFreeQueue, FillToCapacity) {
    LockFreeQueue<int, 4> queue;
    // Capacity 4, one slot reserved, so 3 pushes
    EXPECT_TRUE(queue.push(1));
    EXPECT_TRUE(queue.push(2));
    EXPECT_TRUE(queue.push(3));
    EXPECT_TRUE(queue.isFull());
    EXPECT_FALSE(queue.push(4));
}

TEST(LockFreeQueue, FIFO_Order) {
    LockFreeQueue<int, 16> queue;
    queue.push(10);
    queue.push(20);
    queue.push(30);

    int value = 0;
    queue.pop(value);
    EXPECT_EQ(value, 10);
    queue.pop(value);
    EXPECT_EQ(value, 20);
    queue.pop(value);
    EXPECT_EQ(value, 30);
}

TEST(LockFreeQueue, Reset) {
    LockFreeQueue<int, 16> queue;
    queue.push(1);
    queue.push(2);
    queue.reset();
    EXPECT_TRUE(queue.isEmpty());
    EXPECT_EQ(queue.size(), 0u);
}

TEST(LockFreeQueue, WrapAround) {
    LockFreeQueue<int, 4> queue;
    // Fill and drain multiple times to test wrap-around
    for (int cycle = 0; cycle < 5; ++cycle) {
        EXPECT_TRUE(queue.push(cycle * 10 + 1));
        EXPECT_TRUE(queue.push(cycle * 10 + 2));
        EXPECT_TRUE(queue.push(cycle * 10 + 3));

        int v = 0;
        EXPECT_TRUE(queue.pop(v));
        EXPECT_EQ(v, cycle * 10 + 1);
        EXPECT_TRUE(queue.pop(v));
        EXPECT_EQ(v, cycle * 10 + 2);
        EXPECT_TRUE(queue.pop(v));
        EXPECT_EQ(v, cycle * 10 + 3);
        EXPECT_TRUE(queue.isEmpty());
    }
}

struct TestStruct {
    int a;
    float b;
};

TEST(LockFreeQueue, StructType) {
    LockFreeQueue<TestStruct, 8> queue;
    TestStruct in{42, 3.14f};
    EXPECT_TRUE(queue.push(in));

    TestStruct out{};
    EXPECT_TRUE(queue.pop(out));
    EXPECT_EQ(out.a, 42);
    EXPECT_FLOAT_EQ(out.b, 3.14f);
}
