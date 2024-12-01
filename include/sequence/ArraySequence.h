#ifndef ARRAYSEQUENCE_INCLUDED
#define ARRAYSEQUENCE_INCLUDED

#include "DynamicArray.h"
#include "Sequence.h"
#include <stdexcept>

// Assuming Pair.h is correctly implemented and available
#include "Pair.h"

template<class T>
class ArraySequence : public Sequence<T> {
private:
    DynamicArray<T> data;

public:
    // Constructors

    // Constructor with items and count
    ArraySequence(T *items, int count) : data(items, count) {}

    // Default constructor
    ArraySequence() : data() {}

    // Constructor from a DynamicArray
    explicit ArraySequence(const DynamicArray<T> &array) : data(array) {}

    // Decomposition Methods

    // Gets the first element
    T getFirst() const override {
        if (data.getSize() == 0) {
            throw std::out_of_range("Empty sequence");
        }
        return data.get(0);
    }

    // Gets the last element
    T getLast() const override {
        if (data.getSize() == 0) {
            throw std::out_of_range("Empty sequence");
        }
        return data.get(data.getSize() - 1);
    }

    // Gets the element at a specific index
    T get(int index) const override {
        return data.get(index);
    }

    // Overloaded subscript operator (const)
    T operator[](int i) const override {
        return data[i];
    }

    // Overloaded subscript operator (non-const)
    T &operator[](int i) override {
        return data[i];
    }

    // Gets a subsequence from startIndex to endIndex (inclusive)
    Sequence<T> *getSubsequence(int startIndex, int endIndex) const override {
        if (startIndex > endIndex || startIndex < 0 || endIndex >= data.getSize()) {
            throw std::out_of_range("Invalid indices for subsequence");
        }
        int size = endIndex - startIndex + 1;
        DynamicArray<T> da(size);
        for (int i = 0; i < size; ++i) {
            da[i] = get(startIndex + i);
        }
        return new ArraySequence<T>(da);
    }

    // Gets the current length of the sequence
    int getLength() const override {
        return data.getSize();
    }

    // Operation Methods

    // Appends an item to the end of the sequence
    void append(T item) override {
        int size = data.getSize();
        data.resize(size + 1);
        data.set(size, item);
    }

    // Prepends an item to the beginning of the sequence
    void prepend(T item) override {
        int size = data.getSize();
        data.resize(size + 1);
        data.define_resize(size + 1);
        // Shift elements to the right
        for(int i = size -1; i >=0; --i){
            data[i + 1] = data[i];
        }
        data.set(0, item);
        data.define_set(0, true);
    }

    // Inserts an item at a specific index
    void insertAt(T item, int index) override {
        int size = data.getSize();
        if(index < 0 || index > size){
            throw std::out_of_range("Index out of range");
        }
        data.resize(size + 1);
        data.define_resize(size + 1);
        // Shift elements to the right from index
        for(int i = size -1; i >= index; --i){
            data[i +1] = data[i];
        }
        data.set(index, item);
    }

    // Concatenates another sequence to this sequence
    Sequence<T> *concat(Sequence<T> *list) override {
        if(list == nullptr){
            throw std::invalid_argument("List to concatenate is null");
        }

        int newSize = getLength() + list->getLength();
        DynamicArray<T> da(newSize);
        // Copy existing elements
        for (int i = 0; i < getLength(); ++i) {
            da[i] = data.get(i);
            da.define_set(i, true);
        }
        // Copy elements from the other list
        for (int i = 0; i < list->getLength(); ++i) {
            da[i + getLength()] = list->get(i);
            da.define_set(i + getLength(), true);
        }

        return new ArraySequence<T>(da);
    }

    // Removes an element at a specific index
    void removeAt(int index) override {
        int size = data.getSize();
        if(index <0 || index >= size){
            throw std::out_of_range("Index out of range in removeAt");
        }
        for (int i = index + 1; i < size; ++i) {
            data[i - 1] = data[i];
        }
        data.define_resize(size - 1);
        data.resize(size - 1);
    }

    // Clears the sequence
    void clear() override {
        data.clear();
    }
    bool find(T item) const {
        for (int i = 0; i < data.getSize(); ++i) {
            if (data.get(i) == item) {
                return true;
            }
        }
        return false;
    }

    // Virtual Destructor
    virtual ~ArraySequence() {}

    // Functional Methods

    // Applies a function to each element and returns a new sequence
    Sequence<T> *map(T (*f)(T)) const override {
        if(f == nullptr){
            throw std::invalid_argument("Function pointer for map is null");
        }

        Sequence<T> *res = new ArraySequence<T>();
        for (int i = 0; i < getLength(); ++i) {
            res->append(f(data.get(i)));
        }
        return res;
    }

    // Filters elements based on a predicate and returns a new sequence
    Sequence<T> *where(bool (*h)(T)) const override {
        if(h == nullptr){
            throw std::invalid_argument("Function pointer for where is null");
        }

        auto *res = new ArraySequence<T>();
        for (int i = 0; i < getLength(); ++i) {
            T item = data.get(i);
            if (h(item)) {
                res->append(item);
            }
        }
        return res;
    }

    // Reduces the sequence to a single value using a binary function
    T reduce(T (*f)(T, T)) const override {
        if(f == nullptr){
            throw std::invalid_argument("Function pointer for reduce is null");
        }

        if(data.getSize() ==0){
            throw std::invalid_argument("Cannot reduce an empty sequence");
        }
        T result = data.get(0);
        for (int i = 1; i < data.getSize(); ++i) {
            result = f(result, data.get(i));
        }
        return result;
    }
};

#endif // ARRAYSEQUENCE_INCLUDED