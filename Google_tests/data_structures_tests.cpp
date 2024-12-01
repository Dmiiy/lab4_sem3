#include <gtest/gtest.h>
#include "../include/data_structures/AVLBinaryTree.h"
#include "../include/data_structures/ISetBinaryTree.h"
#include "../include/data_structures/IDictionaryBinaryTree.h"
#include "../include/data_structures/ISortedSequenceBinaryTree.h"
#include "../include/information_processing/Index.h"
#include "../include/information_processing/Histogram.h"


TEST(BinaryTree, InsertAndFind) {
    AVLBinaryTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    ASSERT_NE(tree.find(5), false);
    ASSERT_NE(tree.find(3), false);
    ASSERT_NE(tree.find(7), false);
    ASSERT_EQ(tree.find(10), false);
}

TEST(BinaryTree, Remove) {
    AVLBinaryTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.remove(3);

    ASSERT_EQ(tree.find(3), false);
    ASSERT_NE(tree.find(5), false);
    ASSERT_NE(tree.find(7), false);
}

TEST(BinaryTree, Height) {
    AVLBinaryTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(2);
    tree.insert(4);

    ASSERT_EQ(tree.height(), 3);
}

TEST(BinaryTree, Balance) {
    AVLBinaryTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(2);
    tree.insert(4);
    tree.insert(6);
    tree.insert(8);

    ASSERT_EQ(tree.height(), 3);
}

TEST(BinaryTree, Iterator) {
    AVLBinaryTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    std::vector<int> values;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        values.push_back(*it);
    }

    std::vector<int> expected = {3, 5, 7};
    ASSERT_EQ(values, expected);
}


TEST(SetBinaryTree, InsertAndFind) {
    ISetBinaryTree<int> set;
    set.insert(5);
    set.insert(3);
    set.insert(7);

    ASSERT_TRUE(set.find(5));
    ASSERT_TRUE(set.find(3));
    ASSERT_TRUE(set.find(7));
    ASSERT_FALSE(set.find(10));
}

TEST(SetBinaryTree, Erase) {
    ISetBinaryTree<int> set;
    set.insert(5);
    set.insert(3);
    set.insert(7);
    set.erase(3);

    ASSERT_FALSE(set.find(3));
    ASSERT_TRUE(set.find(5));
    ASSERT_TRUE(set.find(7));
}

TEST(SetBinaryTree, Union) {
    ISetBinaryTree<int> set1;
    set1.insert(1);
    set1.insert(2);

    ISetBinaryTree<int> set2;
    set2.insert(2);
    set2.insert(3);

    ISet<int>* result = set1.setUnion(set2);

    ASSERT_TRUE(result->find(1));
    ASSERT_TRUE(result->find(2));
    ASSERT_TRUE(result->find(3));
    delete result;
}

TEST(SetBinaryTree, Intersection) {
    ISetBinaryTree<int> set1;
    set1.insert(1);
    set1.insert(2);

    ISetBinaryTree<int> set2;
    set2.insert(2);
    set2.insert(3);

    ISet<int>* result = set1.intersection(set2);

    ASSERT_FALSE(result->find(1));
    ASSERT_TRUE(result->find(2));
    ASSERT_FALSE(result->find(3));
    delete result;
}

TEST(SetBinaryTree, Difference) {
    ISetBinaryTree<int> set1;
    set1.insert(1);
    set1.insert(2);

    ISetBinaryTree<int> set2;
    set2.insert(2);
    set2.insert(3);

    ISet<int>* result = set1.difference(set2);

    ASSERT_TRUE(result->find(1));
    ASSERT_FALSE(result->find(2));
    ASSERT_FALSE(result->find(3));
    delete result;
}

TEST(SetBinaryTree, SubSet) {
    ISetBinaryTree<int> set1;
    set1.insert(1);
    set1.insert(2);

    ISetBinaryTree<int> set2;
    set2.insert(1);
    set2.insert(2);
    set2.insert(3);

    ASSERT_TRUE(set1.subSet(set2));
    ASSERT_FALSE(set2.subSet(set1));
}

TEST(SetBinaryTree, Equal) {
    ISetBinaryTree<int> set1;
    set1.insert(1);
    set1.insert(2);

    ISetBinaryTree<int> set2;
    set2.insert(1);
    set2.insert(2);

    ASSERT_TRUE(set1.equal(set2));
    set2.insert(3);
    ASSERT_FALSE(set1.equal(set2));
}

TEST(IDictionaryBinaryTree, AddAndGet) {
    IDictionaryBinaryTree<int, std::string> dict;
    dict.Add(1, "one");
    dict.Add(2, "two");

    ASSERT_EQ(dict.Get(1), "one");
    ASSERT_EQ(dict.Get(2), "two");
}

TEST(IDictionaryBinaryTree, Remove) {
    IDictionaryBinaryTree<int, std::string> dict;
    dict.Add(1, "one");
    dict.Add(2, "two");
    dict.Remove(1);

    ASSERT_THROW(dict.Get(1), std::out_of_range);
    ASSERT_EQ(dict.Get(2), "two");
}

TEST(IDictionaryBinaryTree, ContainsKey) {
    IDictionaryBinaryTree<int, std::string> dict;
    dict.Add(1, "one");
    dict.Add(2, "two");

    ASSERT_TRUE(dict.ContainsKey(1));
    ASSERT_TRUE(dict.ContainsKey(2));
    ASSERT_FALSE(dict.ContainsKey(3));
}

TEST(IDictionaryBinaryTree, GetKeys) {
    IDictionaryBinaryTree<int, std::string> dict;
    dict.Add(1, "one");
    dict.Add(2, "two");

    auto keys = dict.GetKeys();
    ASSERT_EQ(keys.getLength(), 2);
    ASSERT_EQ(keys.get(0), 1);
    ASSERT_EQ(keys.get(1), 2);
}

TEST(IDictionaryBinaryTree, GetValues) {
    IDictionaryBinaryTree<int, std::string> dict;
    dict.Add(1, "one");
    dict.Add(2, "two");

    auto values = dict.GetValues();
    ASSERT_EQ(values.getLength(), 2);
    ASSERT_EQ(values.get(0), "one");
    ASSERT_EQ(values.get(1), "two");
}

TEST(ISortedSequenceBinaryTree, AddAndGet) {
    ISortedSequenceBinaryTree<int> seq;
    seq.Add(3);
    seq.Add(1);
    seq.Add(2);

    ASSERT_EQ(seq.Get(0), 1);
    ASSERT_EQ(seq.Get(1), 2);
    ASSERT_EQ(seq.Get(2), 3);
}

TEST(ISortedSequenceBinaryTree, GetFirstAndLast) {
    ISortedSequenceBinaryTree<int> seq;
    seq.Add(3);
    seq.Add(1);
    seq.Add(2);

    ASSERT_EQ(seq.GetFirst(), 1);
    ASSERT_EQ(seq.GetLast(), 3);
}

TEST(ISortedSequenceBinaryTree, IndexOf) {
    ISortedSequenceBinaryTree<int> seq;
    seq.Add(3);
    seq.Add(1);
    seq.Add(2);

    ASSERT_EQ(seq.IndexOf(1), 0);
    ASSERT_EQ(seq.IndexOf(2), 1);
    ASSERT_EQ(seq.IndexOf(3), 2);
    ASSERT_EQ(seq.IndexOf(4), -1);
}

TEST(ISortedSequenceBinaryTree, GetSubsequence) {
    ISortedSequenceBinaryTree<int> seq;
    seq.Add(3);
    seq.Add(1);
    seq.Add(2);

    auto* subseq = seq.GetSubsequence(0, 1);
    ASSERT_EQ(subseq->GetLength(), 2);
    ASSERT_EQ(subseq->Get(0), 1);
    ASSERT_EQ(subseq->Get(1), 2);
    delete subseq;
}

TEST(ISortedSequenceBinaryTree, IsEmptyAndGetLength) {
    ISortedSequenceBinaryTree<int> seq;
    ASSERT_TRUE(seq.IsEmpty());
    ASSERT_EQ(seq.GetLength(), 0);

    seq.Add(1);
    ASSERT_FALSE(seq.IsEmpty());
    ASSERT_EQ(seq.GetLength(), 1);
}

struct TestDataHistogram {
    int value;        // Числовое значение, которое используется для классификации по диапазонам
    std::string category; // Категория для классификации
};

// Тест на распределение элементов по диапазонам
TEST(HistogramTest, DistributionAcrossRanges) {
    ArraySequence<TestDataHistogram> data;
    data.append({5, "A"});
    data.append({10, "B"});
    data.append({15, "A"});
    data.append({20, "C"});
    data.append({25, "A"});

    ArraySequence<std::pair<int, int>> ranges;
    ranges.append({0,10});
    ranges.append({10,20});
    ranges.append({20,30});
    auto criteria = [](const TestDataHistogram& item) { return item.value; };
    auto classifier = [](const TestDataHistogram& item) { return item.category; };

    Histogram<int, std::string, TestDataHistogram> histogram(data, ranges, criteria, classifier);

    const auto& result = histogram.getHistogram();

    // Проверка диапазонов
    ASSERT_TRUE(result.ContainsKey({0, 10}));
    ASSERT_TRUE(result.ContainsKey({10, 20}));
    ASSERT_TRUE(result.ContainsKey({20, 30}));

    // Проверка диапазона {0, 10}
    const auto& range1 = result.GetReference({0, 10});
    ASSERT_TRUE(range1.ContainsKey("A"));
    const auto& statsA1 = range1.GetReference("A");
    ASSERT_EQ(statsA1.count, 1);
    ASSERT_EQ(statsA1.sum, 5);
    ASSERT_TRUE(statsA1.min.has_value());
    ASSERT_EQ(statsA1.min.value(), 5);
    ASSERT_TRUE(statsA1.max.has_value());
    ASSERT_EQ(statsA1.max.value(), 5);

    // Проверка диапазона {10, 20}
    const auto& range2 = result.GetReference({10, 20});
    ASSERT_TRUE(range2.ContainsKey("A"));
    ASSERT_TRUE(range2.ContainsKey("B"));
    const auto& statsA2 = range2.GetReference("A");
    const auto& statsB2 = range2.GetReference("B");

    ASSERT_EQ(statsA2.count, 1);
    ASSERT_EQ(statsA2.sum, 15);
    ASSERT_EQ(statsB2.count, 1);
    ASSERT_EQ(statsB2.sum, 10);

    // Проверка диапазона {20, 30}
    const auto& range3 = result.GetReference({20, 30});
    ASSERT_TRUE(range3.ContainsKey("A"));
    ASSERT_TRUE(range3.ContainsKey("C"));
    const auto& statsA3 = range3.GetReference("A");
    const auto& statsC3 = range3.GetReference("C");

    ASSERT_EQ(statsA3.count, 1);
    ASSERT_EQ(statsA3.sum, 25);
    ASSERT_EQ(statsC3.count, 1);
    ASSERT_EQ(statsC3.sum, 20);
}

// Тест на обработку пустой последовательности
TEST(HistogramTest, EmptySequence) {
    ArraySequence<TestDataHistogram> data;

    ArraySequence<std::pair<int, int>> ranges;
    ranges.append({0,10});
    ranges.append({10,20});
    ranges.append({20,30});

    auto criteria = [](const TestDataHistogram& item) { return item.value; };
    auto classifier = [](const TestDataHistogram& item) { return item.category; };

    Histogram<int, std::string, TestDataHistogram> histogram(data, ranges, criteria, classifier);

    const auto& result = histogram.getHistogram();

    // Проверяем, что диапазоны существуют, но пустые
    ASSERT_TRUE(result.ContainsKey({0, 10}));
    ASSERT_TRUE(result.ContainsKey({10, 20}));
    ASSERT_TRUE(result.ContainsKey({20, 30}));

    const auto& range1 = result.GetReference({0, 10});
    ASSERT_EQ(range1.GetCount(), 0);

    const auto& range2 = result.GetReference({10, 20});
    ASSERT_EQ(range2.GetCount(), 0);

    const auto& range3 = result.GetReference({20, 30});
    ASSERT_EQ(range3.GetCount(), 0);
}

// Тест на граничные значения
TEST(HistogramTest, BoundaryValues) {
    ArraySequence<TestDataHistogram> data;
    data.append({0, "A"});
    data.append({9, "B"});
    data.append({10, "A"});
    data.append({19, "C"});
    data.append({20, "B"});
    data.append({29, "A"});

    ArraySequence<std::pair<int, int>> ranges;
    ranges.append({0,10});
    ranges.append({10,20});
    ranges.append({20,30});

    auto criteria = [](const TestDataHistogram& item) { return item.value; };
    auto classifier = [](const TestDataHistogram& item) { return item.category; };

    Histogram<int, std::string, TestDataHistogram> histogram(data, ranges, criteria, classifier);

    const auto& result = histogram.getHistogram();

    // Проверяем распределение граничных значений
    const auto& range1 = result.GetReference({0, 10});
    ASSERT_TRUE(range1.ContainsKey("A"));
    ASSERT_TRUE(range1.ContainsKey("B"));
    ASSERT_EQ(range1.GetReference("A").count, 1);
    ASSERT_EQ(range1.GetReference("B").count, 1);

    const auto& range2 = result.GetReference({10, 20});
    ASSERT_TRUE(range2.ContainsKey("A"));
    ASSERT_TRUE(range2.ContainsKey("C"));
    ASSERT_EQ(range2.GetReference("A").count, 1);
    ASSERT_EQ(range2.GetReference("C").count, 1);

    const auto& range3 = result.GetReference({20, 30});
    ASSERT_TRUE(range3.ContainsKey("A"));
    ASSERT_TRUE(range3.ContainsKey("B"));
    ASSERT_EQ(range3.GetReference("A").count, 1);
    ASSERT_EQ(range3.GetReference("B").count, 1);
}

// Структура для тестовых данных
struct TestData {
    int id;               // Уникальный идентификатор
    std::string category; // Категория
    int value;            // Значение
};

// Тесты для `Index`
TEST(IndexTest, BasicAddAndSearch) {
    ArraySequence<TestData> data;
    data.append({1, "A", 10});
    data.append({2, "B", 20});
    data.append({3, "A", 30});

    // Определяем ключ как кортеж (id, категория)
    auto keyExtractors = std::make_tuple(
            [](const TestData& item) { return item.id; },
            [](const TestData& item) { return item.category; }
    );

    Index<TestData, int, std::string> index(data, keyExtractors);

    // Проверяем поиск существующих элементов
    auto result1 = index.Search(std::make_tuple(1, "A"));
    ASSERT_TRUE(result1.has_value());
    ASSERT_EQ(result1->value, 10);

    auto result2 = index.Search(std::make_tuple(3, "A"));
    ASSERT_TRUE(result2.has_value());
    ASSERT_EQ(result2->value, 30);

    // Проверяем поиск несуществующего элемента
    auto result3 = index.Search(std::make_tuple(4, "B"));
    ASSERT_FALSE(result3.has_value());
}

TEST(IndexTest, AddDuplicateKeyThrows) {
    ArraySequence<TestData> data;
    data.append({1, "A", 10});
    data.append({2, "B", 20});

    auto keyExtractors = std::make_tuple(
            [](const TestData& item) { return item.id; },
            [](const TestData& item) { return item.category; }
    );

    Index<TestData, int, std::string> index(data, keyExtractors);

    // Добавляем элемент с уникальным ключом
    index.Add({3, "C", 30});

    // Попытка добавить элемент с уже существующим ключом
    ASSERT_THROW(index.Add({1, "A", 40}), std::invalid_argument);
}

TEST(IndexTest, RemoveAndSearch) {
    ArraySequence<TestData> data;
    data.append({1, "A", 10});
    data.append({2, "B", 20});
    data.append({3, "A", 30});

    auto keyExtractors = std::make_tuple(
            [](const TestData& item) { return item.id; },
            [](const TestData& item) { return item.category; }
    );

    Index<TestData, int, std::string> index(data, keyExtractors);

    // Удаляем элемент
    index.Remove(std::make_tuple(2, "B"));

    // Проверяем, что удаленный элемент больше не находится
    auto result = index.Search(std::make_tuple(2, "B"));
    ASSERT_FALSE(result.has_value());

    // Проверяем, что другие элементы остались
    ASSERT_TRUE(index.Search(std::make_tuple(1, "A")).has_value());
    ASSERT_TRUE(index.Search(std::make_tuple(3, "A")).has_value());
}

TEST(IndexTest, SearchRange) {
    ArraySequence<TestData> data;
    data.append({1, "A", 10});
    data.append({2, "B", 20});
    data.append({3, "A", 30});
    data.append({4, "C", 40});
    data.append({5, "A", 50});

    auto keyExtractors = std::make_tuple(
            [](const TestData& item) { return item.id; },
            [](const TestData& item) { return item.category; }
    );

    Index<TestData, int, std::string> index(data, keyExtractors);

    // Поиск по диапазону ключей
    auto rangeResult = index.SearchRange(std::make_tuple(2, "A"), std::make_tuple(4, "C"));

    ASSERT_EQ(rangeResult.GetCount(), 3);

    auto keys = rangeResult.GetKeys();
    ASSERT_EQ(keys[0], std::make_tuple(2, "B"));
    ASSERT_EQ(keys[1], std::make_tuple(3, "A"));
    ASSERT_EQ(keys[2], std::make_tuple(4, "C"));
}

TEST(IndexTest, GetAllKeysAndValues) {
    ArraySequence<TestData> data;
    data.append({1, "A", 10});
    data.append({2, "B", 20});
    data.append({3, "A", 30});

    auto keyExtractors = std::make_tuple(
            [](const TestData& item) { return item.id; },
            [](const TestData& item) { return item.category; }
    );

    Index<TestData, int, std::string> index(data, keyExtractors);

    auto allKeys = index.GetAllKeys();
    auto allValues = index.GetAllValues();

    ASSERT_EQ(allKeys.getLength(), 3);
    ASSERT_EQ(allValues.getLength(), 3);

    ASSERT_EQ(allKeys[0], std::make_tuple(1, "A"));
    ASSERT_EQ(allKeys[1], std::make_tuple(2, "B"));
    ASSERT_EQ(allKeys[2], std::make_tuple(3, "A"));

    ASSERT_EQ(allValues[0].value, 10);
    ASSERT_EQ(allValues[1].value, 20);
    ASSERT_EQ(allValues[2].value, 30);
}

