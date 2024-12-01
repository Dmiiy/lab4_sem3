// Pair.h
#ifndef LAB4_SEM3_PAIR_H
#define LAB4_SEM3_PAIR_H

#include <iostream>
#include <utility> // For std::hash if needed
#include <functional> // For std::function if needed

template <typename T, typename K>
class Pair {
public:
    T first;
    K second;

    Pair() : first(), second() {}

    Pair(const T& fir, const K& sec) : first(fir), second(sec) {}

    Pair(const Pair<T, K>& other) : first(other.first), second(other.second) {}

    Pair(Pair<T, K>&& other) noexcept
            : first(std::move(other.first)), second(std::move(other.second)) {}


    Pair<T, K>& operator=(const Pair<T, K>& other) {
        if (this != &other) {
            first = other.first;
            second = other.second;
        }
        return *this;
    }

    Pair<T, K>& operator=(Pair<T, K>&& other) noexcept {
        if (this != &other) {
            first = std::move(other.first);
            second = std::move(other.second);
        }
        return *this;
    }

    ~Pair() = default;

    const T& get_first() const {
        return first;
    }

    const K& get_second() const {
        return second;
    }

    void set_first(const T& fir) {
        first = fir;
    }

    void set_second(const K& sec) {
        second = sec;
    }

    friend bool operator==(const Pair<T, K>& lhs, const Pair<T, K>& rhs) {
        return (lhs.first == rhs.first) && (lhs.second == rhs.second);
    }

    friend bool operator!=(const Pair<T, K>& lhs, const Pair<T, K>& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const Pair<T, K>& lhs, const Pair<T, K>& rhs) {
        return lhs.second < rhs.second;
    }

    friend bool operator>(const Pair<T, K>& lhs, const Pair<T, K>& rhs) {
        return lhs.second > rhs.second;
    }

    friend bool operator<=(const Pair<T, K>& lhs, const Pair<T, K>& rhs) {
        return lhs.second <= rhs.second;
    }

    friend bool operator>=(const Pair<T, K>& lhs, const Pair<T, K>& rhs) {
        return lhs.second >= rhs.second;
    }


};

#endif // LAB4_SEM3_PAIR_H