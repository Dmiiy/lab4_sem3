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

        KeyValuePair(const TKey& k, const TValue& v) : key(k), value(v) {}

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
    // Конструктор по умолчанию
    IDictionaryBinaryTree() = default;

    // Конструктор копирования
    IDictionaryBinaryTree(const IDictionaryBinaryTree& other) : tree(other.tree) {}

    // Оператор присваивания
    IDictionaryBinaryTree& operator=(const IDictionaryBinaryTree& other) {
        if (this != &other) {
            tree = other.tree;
        }
        return *this;
    }

    // Конструктор перемещения
    IDictionaryBinaryTree(IDictionaryBinaryTree&& other) noexcept : tree(std::move(other.tree)) {}

    // Оператор перемещения
    IDictionaryBinaryTree& operator=(IDictionaryBinaryTree&& other) noexcept {
        if (this != &other) {
            tree = std::move(other.tree);
        }
        return *this;
    }

    // Получение значения по ключу
    TValue Get(const TKey &key) const override {
        auto node = tree.findRef(KeyValuePair(key, TValue()));
        if (!node) {
            throw std::out_of_range("Key not found");
        }
        return node->value.value;
    }

    // Получение ссылки на значение по ключу
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

    // Проверка наличия ключа
    bool ContainsKey(const TKey &key) const override {
        return tree.find(KeyValuePair(key, TValue()));
    }

    // Добавление пары ключ-значение
    void Add(const TKey &key, const TValue &value) override {
        if (ContainsKey(key)) {
            throw std::invalid_argument("Key already exists");
        }
        tree.insert(KeyValuePair(key, value));
    }

    // Удаление по ключу
    void Remove(const TKey &key) override {
        if (!ContainsKey(key)) {
            throw std::out_of_range("Key not found");
        }
        tree.remove(KeyValuePair(key, TValue()));
    }

    // Очистка словаря
    void Clear() {
        tree.~AVLBinaryTree();
        new (&tree) AVLBinaryTree<KeyValuePair>();
    }

    // Получение количества элементов
    size_t GetCount() const override {
        return tree.getSize();
    }

    // Получение всех ключей
    ArraySequence<TKey> GetKeys() const override {
        ArraySequence<TKey> keys;
        for (const auto &pair : tree) {
            keys.append(pair.key);
        }
        return keys;
    }

    // Получение всех значений
    ArraySequence<TValue> GetValues() const override {
        ArraySequence<TValue> values;
        for (const auto &pair : tree) {
            values.append(pair.value);
        }
        return values;
    }

    // Итератор
    class Iterator {
        using TreeIterator = typename AVLBinaryTree<KeyValuePair>::Iterator;
        TreeIterator iterator;
    public:
        explicit Iterator(TreeIterator it) : iterator(it) {}

        KeyValuePair& operator*() const {
            return *iterator;
        }

        KeyValuePair* operator->() {
            return &(*iterator);
        }

        Iterator& operator++() {
            ++iterator;
            return *this;
        }

        bool operator!=(const Iterator &other) const {
            return iterator != other.iterator;
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