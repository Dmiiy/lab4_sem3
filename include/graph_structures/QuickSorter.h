#ifndef LAB2_SEM3_QUICKSORTER_H
#define LAB2_SEM3_QUICKSORTER_H

#include <functional>
#include "ISorter.h"

template <typename T>
class QuickSorter : public ISorter<T> {
private:
    std::function<bool(const T&, const T&)> comparator;

public:
    QuickSorter(std::function<bool(const T&, const T&)> comp)
            : comparator(comp) {}

    void sort(ArraySequence<T> *sequence) override {
        if (sequence==nullptr) {
            throw std::invalid_argument("Sequence is null");
        }
        QuickSort(sequence, 0, sequence->getLength() - 1);
    }

private:
    void QuickSort(ArraySequence<T> *sequence, int low, int high) {
        if (low < high) {
            int pivotIndex = partition(sequence, low, high);
            QuickSort(sequence, low, pivotIndex - 1);
            QuickSort(sequence, pivotIndex + 1, high);
        }
    }

    // Функция для разбиения массива относительно опорного элемента
    int partition(ArraySequence<T> *sequence, int low, int high) {
        T pivot = (*sequence)[high];
        int i = low;

        for (int j = low; j < high; ++j) {
            if (comparator((*sequence)[j], pivot)) {
                std::swap((*sequence)[i], (*sequence)[j]);
                ++i;
            }
        }
        std::swap((*sequence)[i], (*sequence)[high]);
        return i;
    }
};

#endif // LAB2_SEM3_QUICKSORTER_H
