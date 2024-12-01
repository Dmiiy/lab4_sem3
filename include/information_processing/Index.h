#ifndef LAB3_SEM3_INDEX_H
#define LAB3_SEM3_INDEX_H

#include <functional>
#include <stdexcept>
#include <tuple>
#include <optional>
#include "../sequence/ArraySequence.h"
#include "../data_structures/IDictionaryBinaryTree.h"

template <typename TValue, typename... TKeyParts>
class Index {
private:
    using TKey = std::tuple<TKeyParts...>;
    IDictionaryBinaryTree<TKey, TValue> index;
    std::tuple<std::function<TKeyParts(const TValue&)>...> keyExtractors;

public:
    Index(
            const ArraySequence<TValue>& data,
            std::tuple<std::function<TKeyParts(const TValue&)>...> keyExtractors)
    : keyExtractors(std::move(keyExtractors)) {
        for (int i = 0; i < data.getLength(); ++i) {
            const TValue& item = data.get(i);
            Add(item);
        }
    }

    void Add(const TValue& value) {
        TKey key = createCompositeKey(value);
        if (index.ContainsKey(key)) {
            throw std::invalid_argument("Key already exists");
        }
        index.Add(key, value);
    }

    void Remove(const TKey& key) {
        if (!index.ContainsKey(key)) {
            throw std::out_of_range("Key not found");
        }
        index.Remove(key);
    }

    std::optional<TValue> Search(const TKey& key) const {
        try {
            return index.Get(key);
        } catch (const std::out_of_range&) {
            return std::nullopt;
        }
    }

    IDictionaryBinaryTree<TKey, TValue> SearchRange(const TKey& startKey, const TKey& endKey) const {
        IDictionaryBinaryTree<TKey, TValue> result;
        for (auto it = index.begin(); it != index.end(); ++it) {
            if (compareCompositeKeys(it->key, startKey, endKey)) {
                result.Add(it->key, it->value);
            }
        }
        return result;
    }

    const IDictionaryBinaryTree<TKey, TValue>& GetAll() const {
        return index;
    }

    ArraySequence<TKey> GetAllKeys() const {
        return index.GetKeys();
    }

    ArraySequence<TValue> GetAllValues() const {
        return index.GetValues();
    }

private:

    TKey createCompositeKey(const TValue& value) const {
        return createCompositeKeyImpl(value, std::index_sequence_for<TKeyParts...>{});
    }

    template <std::size_t... I>
    TKey createCompositeKeyImpl(const TValue& value, std::index_sequence<I...>) const {
        return std::make_tuple(std::get<I>(keyExtractors)(value)...);
    }

    bool compareCompositeKeys(const TKey& key, const TKey& startKey, const TKey& endKey) const {
        return compareCompositeKeysImpl(key, startKey, endKey, std::index_sequence_for<TKeyParts...>{});
    }

    template <std::size_t... I>
    bool compareCompositeKeysImpl(const TKey& key, const TKey& startKey, const TKey& endKey, std::index_sequence<I...>) const {
        return (isWithinRange(std::get<I>(key), std::get<I>(startKey), std::get<I>(endKey)) && ...);
    }

    template <typename T>
    bool isWithinRange(const T& value, const T& start, const T& end) const {
        return value >= start && value <= end;
    }

};
#endif //LAB3_SEM3_INDEX_H
