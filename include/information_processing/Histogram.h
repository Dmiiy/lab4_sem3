#ifndef LAB3_SEM3_HISTOGRAM_H
#define LAB3_SEM3_HISTOGRAM_H

#include <functional>
#include <optional>
#include <algorithm>
#include "../data_structures/IDictionaryBinaryTree.h"

template <typename T, typename ClassReturn, typename Class>
class Histogram {
public:
    using Range = std::pair<T, T>;
    using Criteria = std::function<T(const Class&)>;
    using Classifier = std::function<ClassReturn(const Class&)>;

    struct Stats {
        int count = 0;
        T sum = T();
        std::optional<T> min;
        std::optional<T> max;
        ArraySequence<T> elements;

        void update(const T& value) {
            count++;
            sum += value;
            if (!min || value < *min) min = value;
            if (!max || value > *max) max = value;
            elements.append(value);
        }
    };

    Histogram(const ArraySequence<Class>& sequence,
              const ArraySequence<Range>& ranges,
              Criteria criteria,
              Classifier classifier)
            : criteria(criteria), classifier(classifier) {
        for (int i = 0; i < ranges.getLength(); ++i) {
            histogram.Add(ranges[i], IDictionaryBinaryTree<ClassReturn, Stats>());
        }
        buildHistogram(sequence);
    }

    const IDictionaryBinaryTree<Range, IDictionaryBinaryTree<ClassReturn, Stats>>& getHistogram() const {
        return histogram;
    }

private:
    void buildHistogram(const ArraySequence<Class>& sequence) {
        for (int i = 0; i < sequence.getLength(); ++i) {
            const Class& item = sequence[i];
            T value = criteria(item);
            ClassReturn className = classifier(item);

            auto rangeKeys = histogram.GetKeys();
            for (int j = 0; j < rangeKeys.getLength(); ++j) {
                const auto& range = rangeKeys[j];

                if (value >= range.first && value < range.second) {
                    if (!histogram.ContainsKey(range)) {
                        throw std::logic_error("Range not found in histogram");
                    }

                    auto& classStats = histogram.GetReference(range);
                    if (!classStats.ContainsKey(className)) {
                        classStats.Add(className, Stats{});
                    }

                    // Обновляем статистику
                    auto& stats = classStats.GetReference(className);
                    stats.update(value);

                    break;
                }
            }
        }
    }

    Criteria criteria;
    Classifier classifier;
    IDictionaryBinaryTree<Range, IDictionaryBinaryTree<ClassReturn, Stats>> histogram;
};

#endif