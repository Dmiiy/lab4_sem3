#ifndef LAB3_SEM3_ISORTEDSEQUENCE_H
#define LAB3_SEM3_ISORTEDSEQUENCE_H

#include <iostream>
#include <stdexcept>
#include <vector>
#include <utility>
#include "..\sequence\ArraySequence.h"

template <typename TElement>
class ISortedSequence {
public:

    virtual ~ISortedSequence() = default;

    virtual size_t GetLength() const = 0;

    virtual bool IsEmpty() const = 0;

    virtual TElement Get(int index) const = 0;

    virtual void Add(const TElement& element) = 0;

    virtual TElement GetFirst() const = 0;

    virtual TElement GetLast() const = 0;

    virtual int IndexOf(const TElement& element) const = 0;

    virtual ISortedSequence<TElement>* GetSubsequence(int startIndex, int endIndex) const = 0;
};

#endif //LAB3_SEM3_ISORTEDSEQUENCE_H
