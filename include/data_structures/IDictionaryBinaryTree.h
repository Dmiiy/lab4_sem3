#ifndef LAB3_SEM3_IDICTIONARYBINARYTREE_H
#define LAB3_SEM3_IDICTIONARYBINARYTREE_H

#include <stdexcept>
#include "AVLBinaryTree.h"
#include "IDictionary.h"

template <typename TKey, typename TValue>
class IDictionaryBinaryTree : public IDictionary<TKey, TValue> {
    struct KeyValuePair {
        TKey key;
        TValue value;

        KeyValuePair(TKey k, TValue v) : key(k), value(v) {}

        bool operator<(const KeyValuePair &other) const {
            return key < other.key;
        }

        bool operator>(const KeyValuePair &other) const {
            return key > other.key;
        }

        bool operator==(const KeyValuePair &other) const {
            return key == other.key;
        }
        bool operator<=(const KeyValuePair &other) const {
            return key <= other.key;
        }
        bool operator>=(const KeyValuePair &other) const {
            return key >= other.key;
        }
    };

    AVLBinaryTree<KeyValuePair> tree;

public:
    // Constructor
    IDictionaryBinaryTree() = default;

    // Get value by key
    TValue Get(const TKey &key) const override {
        auto node = tree.findRef(KeyValuePair(key, TValue()));
        if (!node) {
            throw std::out_of_range("Key not found");
        }
        return node->value.value;
    }

    // Get value by key (pointer)
    TValue& GetReference(const TKey &key) {
        auto node = tree.findRef(KeyValuePair(key, TValue()));
        if (!node) {
            throw std::out_of_range("Key not found");
        }
        return node->value.value;
    }

    const TValue& GetReference(const TKey &key) const {
        auto node = tree.findRef(KeyValuePair(key, TValue()));
        if (!node) {
            throw std::out_of_range("Key not found");
        }
        return node->value.value;
    }
    // Check if key exists
    bool ContainsKey(const TKey &key) const override {
        return tree.find(KeyValuePair(key, TValue()));
    }

    // Add key-value pair
    void Add(const TKey &key, const TValue &value) override {
        if (ContainsKey(key)) {
            throw std::invalid_argument("Key already exists");
        }
        tree.insert(KeyValuePair(key, value));
    }

    // Remove key-value pair
    void Remove(const TKey &key) override {
        if (!ContainsKey(key)) {
            throw std::out_of_range("Key not found");
        }
        tree.remove(KeyValuePair(key, TValue()));
    }

    // Get count of elements
    size_t GetCount() const override {
        return tree.getSize();
    }

    // Get all keys
    ArraySequence<TKey> GetKeys() const override {
        ArraySequence<TKey> keys;
        for (const auto &pair : tree) {
            keys.append(pair.key);
        }
        return keys;
    }

    // Get all values
    ArraySequence<TValue> GetValues() const override {
        ArraySequence<TValue> values;
        for (const auto &pair : tree) {
            values.append(pair.value);
        }
        return values;
    }

    class Iterator {
        typename AVLBinaryTree<KeyValuePair>::Iterator iterator;
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = KeyValuePair;
        using pointer = KeyValuePair*;
        using reference = KeyValuePair&;

        explicit Iterator(typename AVLBinaryTree<KeyValuePair>::Iterator it) : iterator(it) {}

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

        friend bool operator==(const Iterator &a, const Iterator &b) {
            return a.iterator == b.iterator;
        }

        friend bool operator!=(const Iterator &a, const Iterator &b) {
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

#endif //LAB3_SEM3_IDICTIONARYBINARYTREE_H