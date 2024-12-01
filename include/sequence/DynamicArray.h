#ifndef DYNAMICARRAY_INCLUDED
#define DYNAMICARRAY_INCLUDED

#include <iostream>
#include <stdexcept>

template<class T>
class DynamicArray {
private:
    int size;
    int capacity;  // Current capacity of the array
    T *data;
    bool *defined;  // Indicates if an element is defined

    void checkIndex(int index) const {
        if (index < 0 || index >= size) {
            std::cerr << "Index " << index << " out of range 0.." << (size - 1) << "\n";
            throw std::out_of_range("Index out of range");
        }
    }

    // Increases the capacity of the array if necessary
    void ensureCapacity(int minCapacity) {
        if (minCapacity > capacity) {
            int newCapacity = (minCapacity > capacity * 2) ? minCapacity : capacity * 2;
            T *newData = new T[newCapacity];
            bool *newDefined = new bool[newCapacity];

            // Copy existing data to the new array
            for (int i = 0; i < size; ++i) {
                newData[i] = data[i];
                newDefined[i] = defined[i];
            }

            // Initialize the new elements as undefined
            for (int i = size; i < newCapacity; ++i) {
                newDefined[i] = false;
            }

            delete[] data;
            delete[] defined;

            data = newData;
            defined = newDefined;
            capacity = newCapacity;
        }
    }

public:
    // Constructors

    // Constructor with items and count
    DynamicArray(T *items, int count) : size(count), capacity(count) {
        if (size < 0) throw std::invalid_argument("Size < 0");
        data = new T[capacity];
        defined = new bool[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = items[i];
            defined[i] = true;
        }
    }

    // Default constructor with optional initial count
    explicit DynamicArray(int count = 0) : size(count), capacity(count > 0 ? count : 1) {
        if (size < 0) throw std::invalid_argument("Count < 0");
        data = new T[capacity];
        defined = new bool[capacity];
        for (int i = 0; i < size; ++i) {
            defined[i] = false;
        }
    }

    // Copy constructor
    DynamicArray(const DynamicArray<T> &dynamicArray) : size(dynamicArray.size), capacity(dynamicArray.capacity) {
        data = new T[capacity];
        defined = new bool[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = dynamicArray.data[i];
            defined[i] = dynamicArray.defined[i];
        }
    }

    // Copy assignment operator
    DynamicArray<T> &operator=(const DynamicArray<T> &dynamicArray) {
        if (this == &dynamicArray)
            return *this;

        delete[] data;
        delete[] defined;

        size = dynamicArray.size;
        capacity = dynamicArray.capacity;
        data = new T[capacity];
        defined = new bool[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = dynamicArray.data[i];
            defined[i] = dynamicArray.defined[i];
        }
        return *this;
    }

    // Destructor
    ~DynamicArray() {
        delete[] data;
        delete[] defined;
    }

    // Clears the array and resets to initial state
    void clear() {
        delete[] data;
        delete[] defined;

        // Reset array to initial state
        size = 0;
        capacity = 1;
        data = new T[capacity];
        defined = new bool[capacity]{false};
    }

    // Decomposition Methods

    // Gets element at index with bounds checking
    T &get(int index) {
        checkIndex(index);
        if (!defined[index]) {
            throw std::runtime_error("Element with index not defined");
        }
        return data[index];
    }

    // Gets element at index with bounds checking (const version)
    const T &get(int index) const {
        checkIndex(index);
        if (!defined[index]) {
            throw std::runtime_error("Element with index not defined");
        }
        return data[index];
    }

    // Gets the current size of the array
    int getSize() const {
        return size;
    }

    // Operation Methods

    // Sets the value at a specific index with bounds checking
    void set(int index, T value) {
        checkIndex(index);
        data[index] = value;
        defined[index] = true;
    }

    // Overloaded subscript operator (const)
    T operator[](int index) const {
        return get(index);
    }

    // Overloaded subscript operator (non-const)
    T &operator[](int index) {
        checkIndex(index);
        defined[index] = true;
        return data[index];
    }

    // Resizes the array to a new size, initializing new elements as undefined
    void resize(int newSize) {
        if (newSize < 0) throw std::invalid_argument("Negative size error");
        ensureCapacity(newSize);
        // Initialize new elements as undefined
        for (int i = size; i < newSize; ++i) {
            defined[i] = false;
        }
        size = newSize;
    }

    // Shifts the defined flags when resizing (used in prepend and insertAt)
    void define_resize(int newSize) {
        if (newSize < 0) throw std::invalid_argument("Negative size error");
        if (newSize > size) {
            // Shift defined flags to the right, starting from newSize-1 down to 1
            for(int i = newSize -1; i >=1; --i){
                defined[i] = defined[i -1];
            }
            // Set the first element as undefined
            if(newSize >0)
                defined[0] = false;
        }
    }

    // Sets the defined flag for a specific index
    void define_set(int index, bool value) {
        if (index >= 0 && index < size) {
            defined[index] = value;
        } else {
            throw std::out_of_range("Index out of range in define_set");
        }
    }
};

#endif // DYNAMICARRAY_INCLUDED