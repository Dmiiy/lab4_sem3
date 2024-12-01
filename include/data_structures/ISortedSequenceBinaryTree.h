#ifndef LAB3_SEM3_ISORTEDSEQUENCEBINARYTREE_H
#define LAB3_SEM3_ISORTEDSEQUENCEBINARYTREE_H

#include <iostream>
#include <stdexcept>
#include <vector>
#include "AVLBinaryTree.h"
#include "ISortedSequence.h"

template <typename TElement>
class ISortedSequenceBinaryTree : public ISortedSequence<TElement> {
    AVLBinaryTree<TElement> tree;

public:
    // Constructor
    ISortedSequenceBinaryTree() = default;

    // Get the length of the sequence
    size_t GetLength() const override {
        return tree.getSize();
    }

    // Check if the sequence is empty
    bool IsEmpty() const override {
        return tree.getSize() == 0;
    }

    // Get element by index (in-order traversal)
    TElement Get(int index)const override {
        if (index < 0 || index >= tree.getSize()) {
            throw std::out_of_range("Index out of range");
        }
        auto it = tree.begin();
        std::advance(it, index);
        return *it;
    }

    // Add element with automatic sorting
    void Add(const TElement& element) override {
        tree.insert(element);
    }

    // Get the first element
    TElement GetFirst() const override {
        if (IsEmpty()) {
            throw std::out_of_range("Sequence is empty");
        }
        return tree.getMin();
    }

    // Get the last element
    TElement GetLast() const override {
        if (IsEmpty()) {
            throw std::out_of_range("Sequence is empty");
        }
        return tree.getMax();
    }


    // Get the index of an element (or -1 if not found)
    int IndexOf(const TElement& element) const override {
        int index = 0;
        for (auto it = tree.begin(); it != tree.end(); ++it, ++index) {
            if (*it == element) {
                return index;
            }
        }
        return -1;
    }

    // Get a subsequence
    ISortedSequence<TElement>* GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex >= tree.getSize() || startIndex > endIndex) {
            throw std::out_of_range("Invalid subsequence range");
        }
        auto* subsequence = new ISortedSequenceBinaryTree<TElement>();
        auto it = tree.begin();
        std::advance(it, startIndex);
        for (int i = startIndex; i <= endIndex; ++i, ++it) {
            subsequence->Add(*it);
        }
        return subsequence;
    }

    class Iterator {
        typename AVLBinaryTree<TElement>::Iterator iterator;
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = TElement;
        using pointer = TElement*;
        using reference = TElement&;

        explicit Iterator(typename AVLBinaryTree<TElement>::Iterator it) : iterator(it) {}

        reference operator*() const {
            return *iterator;
        }

        pointer operator->() {
            return iterator.operator->();
        }

        Iterator& operator++() {
            ++iterator;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const Iterator& a, const Iterator& b) {
            return a.iterator == b.iterator;
        }

        friend bool operator!=(const Iterator& a, const Iterator& b) {
            return a.iterator != b.iterator;
        }
    };

    Iterator begin() const {
        return Iterator(tree.begin());
    }

    Iterator end() const {
        return Iterator(tree.end());
    }
};

#endif //LAB3_SEM3_ISORTEDSEQUENCEBINARYTREE_H