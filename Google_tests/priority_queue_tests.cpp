// PriorityQueue_test.cpp
#include "gtest/gtest.h"
#include "../include/sequence/PriorityQueue.h"

// TEST_NAME: PriorityQueue, EnqueueAndSize
TEST(PriorityQueue, EnqueueAndSize) {
    PriorityQueue<int, int> pq;

// Изначально очередь пуста
    ASSERT_TRUE(pq.isEmpty());
    ASSERT_EQ(pq.size(), 0);

// Добавляем элементы
    pq.Enqueue(10, 1); // Приоритет 1
    pq.Enqueue(20, 3); // Приоритет 3
    pq.Enqueue(30, 2); // Приоритет 2

// Проверяем размер и состояние очереди
    ASSERT_FALSE(pq.isEmpty());
    ASSERT_EQ(pq.size(), 3);
}

// TEST_NAME: PriorityQueue, Dequeue
TEST(PriorityQueue, Dequeue) {
    PriorityQueue<int, int> pq;

// Добавляем элементы с разными приоритетами
    pq.Enqueue(10, 1); // Наименьший приоритет
    pq.Enqueue(20, 3); // Наивысший приоритет
    pq.Enqueue(30, 2);

// Dequeue должен вернуть элемент с наивысшим приоритетом (20)
    Pair<int, int> first = pq.Dequeue();
    ASSERT_EQ(first.first, 20);
    ASSERT_EQ(first.second, 3);
    ASSERT_EQ(pq.size(), 2);

// Следующий Dequeue должен вернуть элемент с приоритетом 2 (30)
    Pair<int, int> second = pq.Dequeue();
    ASSERT_EQ(second.first, 30);
    ASSERT_EQ(second.second, 2);
    ASSERT_EQ(pq.size(), 1);

// Последний Dequeue должен вернуть оставшийся элемент (10)
    Pair<int, int> third = pq.Dequeue();
    ASSERT_EQ(third.first, 10);
    ASSERT_EQ(third.second, 1);
    ASSERT_EQ(pq.size(), 0);

// Теперь очередь должна быть пуста
    ASSERT_TRUE(pq.isEmpty());
}

// TEST_NAME: PriorityQueue, PeekFirstAndPeekLast
TEST(PriorityQueue, PeekFirstAndPeekLast) {
    PriorityQueue<int, int> pq;

    pq.Enqueue(10, 1);
    pq.Enqueue(20, 3);
    pq.Enqueue(30, 2);

// PeekFirst должен вернуть элемент с наивысшим приоритетом (20)
    Pair<int, int> first = pq.PeekFirst();
    ASSERT_EQ(first.first, 20);
    ASSERT_EQ(first.second, 3);

// PeekLast должен вернуть элемент с наименьшим приоритетом (10)
    Pair<int, int> last = pq.PeekLast();
    ASSERT_EQ(last.first, 10);
    ASSERT_EQ(last.second, 1);

// Размер очереди не должен измениться
    ASSERT_EQ(pq.size(), 3);
}

// TEST_NAME: PriorityQueue, IsEmpty
TEST(PriorityQueue, IsEmpty) {
    PriorityQueue<int, int> pq;

    ASSERT_TRUE(pq.isEmpty());

    pq.Enqueue(10, 1);
    ASSERT_FALSE(pq.isEmpty());

    pq.Dequeue();
    ASSERT_TRUE(pq.isEmpty());
}

// TEST_NAME: PriorityQueue, DequeueException
TEST(PriorityQueue, DequeueException) {
    PriorityQueue<int, int> pq;

// Попытка Dequeue из пустой очереди должна выбросить исключение
    ASSERT_THROW(pq.Dequeue(), std::out_of_range);

// Добавляем элемент и удаляем его
    pq.Enqueue(10, 1);
    Pair<int, int> item = pq.Dequeue();
    ASSERT_EQ(item.first, 10);
    ASSERT_EQ(item.second, 1);

// Очередь снова пуста
    ASSERT_THROW(pq.Dequeue(), std::out_of_range);
}

// TEST_NAME: PriorityQueue, Clear
TEST(PriorityQueue, Clear) {
    PriorityQueue<int, int> pq;

    pq.Enqueue(10, 1);
    pq.Enqueue(20, 3);
    pq.Enqueue(30, 2);

    ASSERT_EQ(pq.size(), 3);
    ASSERT_FALSE(pq.isEmpty());

// Очищаем очередь
    pq.clear();
    ASSERT_EQ(pq.size(), 0);
    ASSERT_TRUE(pq.isEmpty());

// Попытка Dequeue после очистки должна выбросить исключение
    ASSERT_THROW(pq.Dequeue(), std::out_of_range);
}

// TEST_NAME: PriorityQueue, PeekException
TEST(PriorityQueue, PeekException) {
    PriorityQueue<int, int> pq;

// Попытка PeekFirst из пустой очереди должна выбросить исключение
    ASSERT_THROW(pq.PeekFirst(), std::out_of_range);

// Попытка PeekLast из пустой очереди должна выбросить исключение
    ASSERT_THROW(pq.PeekLast(), std::out_of_range);

// Добавляем элемент и проверяем Peek
    pq.Enqueue(10, 1);
    Pair<int, int> first = pq.PeekFirst();
    Pair<int, int> last = pq.PeekLast();
    ASSERT_EQ(first.first, 10);
    ASSERT_EQ(last.first, 10);
    ASSERT_EQ(first.second, 1);
    ASSERT_EQ(last.second, 1);
}

// TEST_NAME: PriorityQueue, MultipleEnqueueDequeue
TEST(PriorityQueue, MultipleEnqueueDequeue) {
    PriorityQueue<std::string, int> pq;

    pq.Enqueue("Task1", 2);
    pq.Enqueue("Task2", 5);
    pq.Enqueue("Task3", 1);
    pq.Enqueue("Task4", 3);
    pq.Enqueue("Task5", 4);

    ASSERT_EQ(pq.size(), 5);

// Ожидаемый порядок Dequeue: Task2 (5), Task5 (4), Task4 (3), Task1 (2), Task3 (1)
    Pair<std::string, int> first = pq.Dequeue();
    ASSERT_EQ(first.first, "Task2");
    ASSERT_EQ(first.second, 5);

    Pair<std::string, int> second = pq.Dequeue();
    ASSERT_EQ(second.first, "Task5");
    ASSERT_EQ(second.second, 4);

    Pair<std::string, int> third = pq.Dequeue();
    ASSERT_EQ(third.first, "Task4");
    ASSERT_EQ(third.second, 3);

    Pair<std::string, int> fourth = pq.Dequeue();
    ASSERT_EQ(fourth.first, "Task1");
    ASSERT_EQ(fourth.second, 2);

    Pair<std::string, int> fifth = pq.Dequeue();
    ASSERT_EQ(fifth.first, "Task3");
    ASSERT_EQ(fifth.second, 1);

    ASSERT_TRUE(pq.isEmpty());
}

// TEST_NAME: PriorityQueue, Peek
TEST(PriorityQueue, Peek) {
    PriorityQueue<int, int> pq;

    pq.Enqueue(100, 10);
    pq.Enqueue(200, 20);
    pq.Enqueue(300, 15);

// Проверка Peek по индексу
    Pair<int, int> peek0 = pq.Peek(0); // Task with priority 20
    ASSERT_EQ(peek0.first, 200);
    ASSERT_EQ(peek0.second, 20);

    Pair<int, int> peek1 = pq.Peek(1); // Task with priority 15
    ASSERT_EQ(peek1.first, 300);
    ASSERT_EQ(peek1.second, 15);

    Pair<int, int> peek2 = pq.Peek(2); // Task with priority 10
    ASSERT_EQ(peek2.first, 100);
    ASSERT_EQ(peek2.second, 10);

// Проверка выхода за пределы
    ASSERT_THROW(pq.Peek(-1), std::out_of_range);
    ASSERT_THROW(pq.Peek(3), std::out_of_range);
}

// TEST_NAME: PriorityQueue, InsertionOrderWithSamePriority
TEST(PriorityQueue, InsertionOrderWithSamePriority) {
    PriorityQueue<int, int> pq;

// Добавляем несколько элементов с одинаковым приоритетом
    pq.Enqueue(1, 5);
    pq.Enqueue(2, 5);
    pq.Enqueue(3, 5);

    ASSERT_EQ(pq.size(), 3);

// Ожидаемый порядок Dequeue: 1, 2, 3 (FIFO для одинаковых приоритетов)
    Pair<int, int> first = pq.Dequeue();
    ASSERT_EQ(first.first, 1);
    ASSERT_EQ(first.second, 5);

    Pair<int, int> second = pq.Dequeue();
    ASSERT_EQ(second.first, 2);
    ASSERT_EQ(second.second, 5);

    Pair<int, int> third = pq.Dequeue();
    ASSERT_EQ(third.first, 3);
    ASSERT_EQ(third.second, 5);
}
