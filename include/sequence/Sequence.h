#ifndef SEQUENCE_INCLUDED
#define SEQUENCE_INCLUDED

template<class T>
class Sequence {
public:
    //декомпозиция
    virtual T getFirst() const = 0;

    virtual T getLast() const = 0;

    virtual T get(int index) const = 0;

    //перегруженные операторы для заданного индекса
    virtual T operator[](int i) const = 0;

    virtual T &operator[](int i) = 0;

    virtual Sequence<T> *getSubsequence(int startIndex, int endIndex) const = 0;

    virtual int getLength() const = 0;

    //операции
    virtual void append(T item) = 0;

    virtual void prepend(T item) = 0;
    virtual void clear() = 0;

    virtual void insertAt(T item, int index) = 0;

    virtual void removeAt(int index) = 0;

    virtual Sequence<T> *concat(Sequence<T> *list) = 0;

    virtual Sequence<T> *map(T (*f)(T)) const = 0;

    virtual Sequence<T> *where(bool (*h)(T)) const = 0;

    virtual T reduce(T (*f)(T, T)) const = 0;

    //деструктор
    virtual ~Sequence() = default;
};

#endif
