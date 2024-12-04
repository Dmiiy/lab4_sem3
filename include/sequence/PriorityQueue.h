#ifndef LAB4_SEM3_PRIORITYQUEUE_H
#define LAB4_SEM3_PRIORITYQUEUE_H

#include "../data_structures/AVLBinaryTree.h"
#include "Pair.h"
#include "Sequence.h"
#include <stdexcept>

template<typename T, typename K>
class PriorityQueue : public Sequence<Pair<T, K>> {
private:
    AVLBinaryTree<Pair<T, K>> data;

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
        Enqueue(item.first, item.second);
    }

    void prepend(Pair<T, K> item) override {
        Enqueue(item.first, item.second);
    }

    void insertAt(Pair<T, K> item, int index) override {
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
    PriorityQueue() = default;

    void Enqueue(T item, K priority) {
        Pair<T, K> newPair(item, priority);
        data.insert(newPair);
    }

    Pair<T, K> Dequeue() {
        if (data.getSize() == 0) {
            throw std::out_of_range("Queue is empty");
        }
        Pair<T, K> maxItem = data.getMax();
        data.remove(maxItem);
        return maxItem;
    }

    Pair<T, K> Peek(int index) const {
        if (data.getSize() == 0) {
            throw std::out_of_range("Queue is empty");
        }

        auto it = data.begin();
        for (int i = 0; i < index && it != data.end(); ++i) {
            ++it;
        }

        if (it == data.end()) {
            throw std::out_of_range("Index out of range");
        }

        return *it;
    }

    Pair<T, K> PeekFirst() const {
        if (data.getSize() == 0) {
            throw std::out_of_range("Queue is empty");
        }
        return data.getMax();
    }

    Pair<T, K> PeekLast() const {
        if (data.getSize() == 0) {
            throw std::out_of_range("Queue is empty");
        }
        return data.getMin();
    }

    bool isEmpty() const {
        return data.getSize() == 0;
    }

    int size() const {
        return data.getSize();
    }

    void clear() override {
        data = AVLBinaryTree<Pair<T, K>>();
    }

    int getLength() const override {
        return data.getSize();
    }

    virtual ~PriorityQueue() {}
};

#endif //LAB4_SEM3_PRIORITYQUEUE_H