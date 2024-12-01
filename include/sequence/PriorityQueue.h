// PriorityQueue.h
#ifndef LAB4_SEM3_PRIORITYQUEUE_H
#define LAB4_SEM3_PRIORITYQUEUE_H

#include "ArraySequence.h"
#include "Pair.h"
#include "Sequence.h"
#include <stdexcept>

// Наследуемся от Sequence<Pair<T, K>>
template<typename T, typename K>
class PriorityQueue : public Sequence<Pair<T, K>> {
private:
    ArraySequence<Pair<T, K>> data; // Хранение элементов очереди

    // Скрытие методов из Sequence путем переопределения и объявления их как private
    Pair<T, K> get(int index) const override {
        throw std::invalid_argument("Operation not supported");
    }

    Pair<T, K> getFirst() const override {
        throw std::invalid_argument("Operation not supported");
    }

    Pair<T, K> getLast() const override {
        throw std::invalid_argument("Operation not supported");
    }

    Sequence<Pair<T, K>>* getSubsequence(int startIndex, int endIndex) const override {
        throw std::invalid_argument("Operation not supported");
    }

    void append(Pair<T, K> item) override {
        Enqueue(item.first, item.second); // Вызов Enqueue при добавлении
    }

    void prepend(Pair<T, K> item) override {
        Enqueue(item.first, item.second); // Вызов Enqueue при добавлении
    }

    void insertAt(Pair<T, K> item, int index) override { // Исправленная сигнатура
        throw std::invalid_argument("Invalid operation");
    }

    void removeAt(int index) override {
        throw std::invalid_argument("Invalid operation");
    }

    Sequence<Pair<T, K>>* concat(Sequence<Pair<T, K>>* list) override {
        throw std::invalid_argument("Operation not supported");
    }

    Sequence<Pair<T, K>>* map(Pair<T, K> (*f)(Pair<T, K>)) const override {
        throw std::invalid_argument("Operation not supported");
    }

    Sequence<Pair<T, K>>* where(bool (*h)(Pair<T, K>)) const override {
        throw std::invalid_argument("Operation not supported");
    }

    Pair<T, K> reduce(Pair<T, K> (*f)(Pair<T, K>, Pair<T, K>)) const override {
        throw std::invalid_argument("Operation not supported");
    }

    Pair<T, K> operator[](int i) const override {
        throw std::invalid_argument("Operation not supported");
    }

    Pair<T, K>& operator[](int i) override {
        throw std::invalid_argument("Operation not supported");
    }

public:
    // Конструктор по умолчанию
    PriorityQueue() = default;

    // Метод добавления элемента с приоритетом
    void Enqueue(T item, K priority) {
        Pair<T, K> newPair(item, priority);
        int i = 0;
        // Найти подходящее место для вставки (больший или равный приоритет соответствует большему или равному значению K)
        while (i < data.getLength() && data[i].second >= priority) {
            ++i;
        }
        data.insertAt(newPair, i);
    }

    // Метод удаления и возврата элемента с наивысшим приоритетом
    Pair<T, K> Dequeue() {
        if (data.getLength() == 0) {
            throw std::out_of_range("Queue is empty");
        }
        Pair<T, K> item = data.getFirst();
        data.removeAt(0);
        return item;
    }

    // Метод просмотра элемента по индексу
    Pair<T, K> Peek(int index) const {
        if (index < 0 || index >= data.getLength()) {
            throw std::out_of_range("Index out of range");
        }
        return data.get(index);
    }

    // Метод просмотра первого элемента (с наивысшим приоритетом)
    Pair<T, K> PeekFirst() const {
        if (data.getLength() == 0) {
            throw std::out_of_range("Queue is empty");
        }
        return data.getFirst();
    }

    // Метод просмотра последнего элемента
    Pair<T, K> PeekLast() const {
        if (data.getLength() == 0) {
            throw std::out_of_range("Queue is empty");
        }
        return data.getLast();
    }

    // Метод проверки, пуста ли очередь
    bool isEmpty() const {
        return data.getLength() == 0;
    }

    // Метод получения количества элементов в очереди
    int size() const {
        return data.getLength();
    }

    // Метод очистки очереди
    void clear() override {
        data.clear();
    }

    // Переопределение getLength()
    int getLength() const override {
        return data.getLength();
    }

    // Деструктор
    virtual ~PriorityQueue()  {}
};

#endif //LAB4_SEM3_PRIORITYQUEUE_H