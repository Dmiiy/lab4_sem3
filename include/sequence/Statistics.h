#ifndef STATISTICS_INCLUDED
#define STATISTICS_INCLUDED

#include "ArraySequence.h"
#include "sorting/QuickSorter.h"
#include <cmath>
#include <algorithm>

template<class T>
class Statistics {
public:
    // Метод для подсчета среднего значения
    static double mean(const ArraySequence<T>& data) {
        int length = data.getLength();
        if (length == 0) throw std::runtime_error("Array is empty");

        T sum = 0;
        for (int i = 0; i < length; ++i) {
            sum += data[i];
        }
        return static_cast<double>(sum) / length;
    }

    // Метод для подсчета медианы
    static double median(ArraySequence<T> data) {
        int length = data.getLength();
        if (length == 0) throw std::runtime_error("Array is empty");

        QuickSorter<T> sorter(ascending);
        sorter.sort(&data);

        if (length % 2 == 0) {
            return (static_cast<double>(data[length / 2 - 1]) + data[length / 2]) / 2;
        }
        else {
            return data[length / 2];
        }
    }

    // Метод для подсчета среднеквадратичного значения
    static double meanSquare(const ArraySequence<T>& data) {
        int length = data.getLength();
        if (length == 0) throw std::runtime_error("Array is empty");

        double sumOfSquares = 0;
        for (int i = 0; i < length; ++i) {
            sumOfSquares += static_cast<double>(data[i]) * data[i];
        }
        return sumOfSquares / length;
    }

    // Метод для подсчета среднеквадратического отклонения
    static double standardDeviation(const ArraySequence<T>& data) {
        int length = data.getLength();
        if (length == 0) throw std::runtime_error("Array is empty");

        double avg = mean(data);
        double sumOfSquareDiffs = 0;
        for (int i = 0; i < length; ++i) {
            double diff = static_cast<double>(data[i]) - avg;
            sumOfSquareDiffs += diff * diff;
        }
        return std::sqrt(sumOfSquareDiffs / length);
    }
};

#endif
