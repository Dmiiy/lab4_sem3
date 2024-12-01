
#ifndef LAB3_SEM3_IDICTIONARY_H
#define LAB3_SEM3_IDICTIONARY_H

#include <iostream>
#include <stdexcept>

#include "../sequence/ArraySequence.h"

template <typename TKey, typename TValue>
class IDictionary {
public:
    virtual ~IDictionary() = default;

    virtual TValue Get(const TKey& key) const = 0;

    virtual bool ContainsKey(const TKey& key) const = 0;

    virtual void Add(const TKey& key, const TValue& value) = 0;

    virtual void Remove(const TKey& key) = 0;

    virtual size_t GetCount() const = 0;

    virtual ArraySequence<TKey> GetKeys() const = 0;

    virtual ArraySequence<TValue> GetValues() const = 0;
};

#endif //LAB3_SEM3_IDICTIONARY_H
