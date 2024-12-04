#include "gtest/gtest.h"
#include "../include/sequence/PriorityQueue.h"

// Basic Structure Tests
TEST(PriorityQueue, BasicStructure) {
    PriorityQueue<int, int> pq;
    ASSERT_TRUE(pq.isEmpty());
    ASSERT_EQ(pq.size(), 0);

    pq.Enqueue(1, 1);
    ASSERT_FALSE(pq.isEmpty());
    ASSERT_EQ(pq.size(), 1);

    pq.clear();
    ASSERT_TRUE(pq.isEmpty());
    ASSERT_EQ(pq.size(), 0);
}

// Enqueue Tests
TEST(PriorityQueue, EnqueueOperations) {
    PriorityQueue<int, int> pq;

    // Test single enqueue
    pq.Enqueue(1, 1);
    ASSERT_EQ(pq.PeekFirst().first, 1);

    // Test multiple enqueues with increasing priority
    pq.Enqueue(2, 2);
    pq.Enqueue(3, 3);
    ASSERT_EQ(pq.PeekFirst().first, 3);
    ASSERT_EQ(pq.PeekFirst().second, 3);

    // Test enqueue with lower priority
    pq.Enqueue(0, 0);
    ASSERT_EQ(pq.PeekLast().first, 0);
    ASSERT_EQ(pq.size(), 4);
}

// Dequeue Tests
TEST(PriorityQueue, DequeueOperations) {
    PriorityQueue<int, int> pq;

    // Setup test data
    pq.Enqueue(1, 1);
    pq.Enqueue(2, 2);
    pq.Enqueue(3, 3);

    // Test dequeue order
    auto item = pq.Dequeue();
    ASSERT_EQ(item.first, 3);
    ASSERT_EQ(item.second, 3);

    item = pq.Dequeue();
    ASSERT_EQ(item.first, 2);
    ASSERT_EQ(item.second, 2);

    item = pq.Dequeue();
    ASSERT_EQ(item.first, 1);
    ASSERT_EQ(item.second, 1);

    ASSERT_TRUE(pq.isEmpty());
}

// Peek Operations Tests
TEST(PriorityQueue, PeekOperations) {
    PriorityQueue<int, int> pq;

    pq.Enqueue(1, 1);
    pq.Enqueue(2, 2);
    pq.Enqueue(3, 3);

    // Test PeekFirst
    ASSERT_EQ(pq.PeekFirst().first, 3);
    ASSERT_EQ(pq.PeekFirst().second, 3);

    // Test PeekLast
    ASSERT_EQ(pq.PeekLast().first, 1);
    ASSERT_EQ(pq.PeekLast().second, 1);

    // Verify size hasn't changed
    ASSERT_EQ(pq.size(), 3);
}

// Exception Tests
TEST(PriorityQueue, ExceptionHandling) {
    PriorityQueue<int, int> pq;

    // Test empty queue operations
    ASSERT_THROW(pq.Dequeue(), std::out_of_range);
    ASSERT_THROW(pq.PeekFirst(), std::out_of_range);
    ASSERT_THROW(pq.PeekLast(), std::out_of_range);

    pq.Enqueue(1, 1);
    pq.Dequeue();

    // Test after becoming empty
    ASSERT_THROW(pq.Dequeue(), std::out_of_range);
}

// Complex Operations Test
TEST(PriorityQueue, ComplexOperations) {
    PriorityQueue<std::string, int> pq;

    // Mixed operations
    pq.Enqueue("Low", 1);
    pq.Enqueue("High", 3);
    pq.Enqueue("Medium", 2);

    ASSERT_EQ(pq.PeekFirst().first, "High");
    pq.Enqueue("VeryHigh", 4);
    ASSERT_EQ(pq.PeekFirst().first, "VeryHigh");

    auto item = pq.Dequeue();
    ASSERT_EQ(item.first, "VeryHigh");
    ASSERT_EQ(pq.size(), 3);

    pq.clear();
    ASSERT_TRUE(pq.isEmpty());
}

// Stress Test
TEST(PriorityQueue, StressTest) {
    PriorityQueue<int, int> pq;
    const int TEST_SIZE = 1000;

    // Insert many elements
    for (int i = 0; i < TEST_SIZE; i++) {
        pq.Enqueue(i, TEST_SIZE - i);
    }

    ASSERT_EQ(pq.size(), TEST_SIZE);

    // Verify order
    int lastPriority = TEST_SIZE + 1;
    for (int i = 0; i < TEST_SIZE; i++) {
        auto item = pq.Dequeue();
        ASSERT_LT(item.second, lastPriority);
        lastPriority = item.second;
    }

    ASSERT_TRUE(pq.isEmpty());
}