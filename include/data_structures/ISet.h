#ifndef LAB3_SEM3_ISET_H
#define LAB3_SEM3_ISET_H

#include <iostream>
#include <stdexcept>

#include "../sequence/ArraySequence.h"

template <typename T>
class ISet {
public:
    virtual ~ISet() = default;

    virtual void insert(const T &value) = 0;
    virtual bool find(const T &value) const = 0;
    virtual void erase(const T &value) = 0;
    virtual int size() const = 0;
    virtual ISet<T>* setUnion(ISet<T> &s) = 0;
    virtual ISet<T>* intersection(ISet<T> &s) = 0;
    virtual ISet<T>* difference(ISet<T> &s) = 0;
    virtual bool subSet(const ISet<T> &set) const = 0;
    virtual bool equal(const ISet<T> &set) const = 0;
};

#endif //LAB3_SEM3_ISET_H
