#include <cstdlib>

#include "../include/sequence/ArraySequence.h"
#include "../include/sequence/DynamicArray.h"
#include "../include/sequence/Sequence.h"
#include "lib/googletest/include/gtest/gtest.h"
#include "../include/sequence/Common.h"

// Проверяем базовые операции DynamicArray и ArraySequence
TEST(DynamicArray, basic_operations) {
    DynamicArray<int> array(3);
    ArraySequence<int> da(array);
    ASSERT_EQ(3, da.getLength());  // Проверяем размер

    // Задаем значения
    da[0] = 11;
    da[1] = 22;
    da[2] = 33;

    ASSERT_EQ(11, da.get(0));
    ASSERT_EQ(22, da.get(1));
    ASSERT_EQ(33, da.get(2));

    // Проверка индексации через оператор
    ASSERT_EQ(11, da[0]);
    ASSERT_EQ(22, da[1]);
    ASSERT_EQ(33, da[2]);

    // Изменение значений
    da[0] = 101;
    ASSERT_EQ(101, da[0]);
    ASSERT_EQ(3, da.getLength());

    // Вставка элементов
    da.insertAt(1, 0);
    ASSERT_EQ(4, da.getLength());
    ASSERT_EQ(1, da[0]);
    ASSERT_EQ(101, da[1]);
    ASSERT_EQ(22, da[2]);
    ASSERT_EQ(33, da[3]);
}

// Тест на удаление элемента
TEST(ArraySequence, removeAt) {
    int data[] = {11, 22, 33};
    ArraySequence<int> da(data, 3);

    da.removeAt(1);
    ASSERT_EQ(2, da.getLength());
    ASSERT_EQ(11, da[0]);
    ASSERT_EQ(33, da[1]);

    da.removeAt(1);
    ASSERT_EQ(1, da.getLength());
    ASSERT_EQ(11, da[0]);

    da.removeAt(0);
    ASSERT_EQ(0, da.getLength());
}

// Тест на изменение размера DynamicArray
TEST(DynamicArray, resize) {
    DynamicArray<int> da(0);
    ASSERT_EQ(0, da.getSize());
    for (int size = 1; size <= 10; size++) {
        da.resize(size);
        ASSERT_EQ(size, da.getSize());
    }
}

// Инициализация массива и копирование
TEST(DynamicArray, array_initialization) {
    int data[] = {111, 222};
    DynamicArray<int> da(data, 2);

    ASSERT_EQ(2, da.getSize());
    ASSERT_EQ(111, da.get(0));
    ASSERT_EQ(222, da.get(1));

    // Копирующий конструктор
    DynamicArray<int> d2(da);
    ASSERT_EQ(da.getSize(), d2.getSize());
    ASSERT_EQ(111, d2.get(0));
    ASSERT_EQ(222, d2.get(1));

    // Уменьшение и увеличение размера
    d2.resize(1);
    ASSERT_EQ(1, d2.getSize());
    ASSERT_EQ(111, d2.get(0));

    d2.resize(3);
    ASSERT_EQ(3, d2.getSize());
    d2.set(1, 221);
    d2.set(2, 331);
    ASSERT_EQ(221, d2.get(1));
    ASSERT_EQ(331, d2.get(2));
}

// Тест конструктора ArraySequence
TEST(ArraySequence, constructor) {
    Sequence<int> *s = new ArraySequence<int>();
    ASSERT_EQ(0, s->getLength());
    for (int x = 1; x <= 10; x++) {
        s->append(x);
        ASSERT_EQ(x, s->getLength());
        ASSERT_EQ(x, s->getLast());
    }
    delete s;
}

// Тест операций append и prepend
TEST(ArraySequence, ArraySequence_Append_Prepend_element) {
    int data[] = {111, 222};
    Sequence<int> *s = new ArraySequence<int>(data, 2);

    ASSERT_EQ(2, s->getLength());
    ASSERT_EQ(111, s->get(0));
    ASSERT_EQ(222, s->get(1));

    s->append(333);
    ASSERT_EQ(3, s->getLength());
    ASSERT_EQ(333, s->get(2));

    s->prepend(10);
    ASSERT_EQ(4, s->getLength());
    ASSERT_EQ(10, s->get(0));
    ASSERT_EQ(111, s->get(1));

    delete s;
}

// Создание ArraySequence из DynamicArray
TEST(ArraySequence, create_from_DynamicArray) {
    int data[] = {123, 234, 345};
    DynamicArray<int> da(data, 3);
    Sequence<int> *s = new ArraySequence<int>(da);

    ASSERT_EQ(3, s->getLength());
    ASSERT_EQ(123, s->get(0));
    ASSERT_EQ(345, s->getLast());

    s->append(333);
    ASSERT_EQ(4, s->getLength());
    ASSERT_EQ(333, s->getLast());

    delete s;
}

// Сцепление двух ArraySequence
TEST(ArraySequence, concat) {
    int data1[] = {11, 22};
    DynamicArray<int> da1(data1, 2);
    Sequence<int> *s1 = new ArraySequence<int>(da1);

    int data2[] = {33};
    DynamicArray<int> da2(data2, 1);
    Sequence<int> *s2 = new ArraySequence<int>(da2);

    Sequence<int> *res = s1->concat(s2);
    ASSERT_EQ(3, res->getLength());
    ASSERT_EQ(11, res->get(0));
    ASSERT_EQ(33, res->get(2));

    delete s1;
    delete s2;
    delete res;
}
