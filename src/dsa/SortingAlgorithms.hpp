#pragma once

#include "DynamicArray.hpp"
#include <string>
#include <vector>

enum class SortingStepType {
    COMPARE,
    SWAP,
    NO_SWAP,
    PIVOT_SET,
    SPLIT,
    MERGE,
    PASS_COMPLETE,
    SORTED
};

struct SortingStep {
    SortingStepType type{SortingStepType::COMPARE};
    size_t index1{0};
    size_t index2{0};
    size_t rangeStart{0};
    size_t rangeEnd{0};
    size_t pivotIndex{0};
    std::string description;
    std::vector<int> snapshot;
};

class IAlgorithm {
public:
    virtual ~IAlgorithm() = default;
    virtual const char* getName() const = 0;
    virtual const char* getDescription() const = 0;
    virtual const char* getTimeComplexityAvg() const = 0;
    virtual const char* getTimeComplexityWorst() const = 0;
    virtual const char* getTimeComplexityBest() const = 0;
    virtual const char* getSpaceComplexity() const = 0;
};

class SortingAlgorithm : public IAlgorithm {
public:
    virtual ~SortingAlgorithm() = default;

    // Genuine sorting execution on array
    virtual void sort(DynamicArray<int>& arr) const = 0;

    // Generates visualization steps from genuine sorting steps
    virtual std::vector<SortingStep> generateSteps(const DynamicArray<int>& inputArr) const = 0;
};

// 1. Genuine Bubble Sort
class BubbleSort : public SortingAlgorithm {
public:
    const char* getName() const override { return "Bubble Sort"; }
    const char* getDescription() const override { return "Compare nearby items and swap them when they are out of order."; }
    const char* getTimeComplexityAvg() const override { return "O(n²)"; }
    const char* getTimeComplexityWorst() const override { return "O(n²)"; }
    const char* getTimeComplexityBest() const override { return "O(n)"; }
    const char* getSpaceComplexity() const override { return "O(1)"; }

    void sort(DynamicArray<int>& arr) const override;
    std::vector<SortingStep> generateSteps(const DynamicArray<int>& inputArr) const override;
};

// 2. Genuine Merge Sort
class MergeSort : public SortingAlgorithm {
public:
    const char* getName() const override { return "Merge Sort"; }
    const char* getDescription() const override { return "Divide array into halves, recursively sort them, and merge sorted halves."; }
    const char* getTimeComplexityAvg() const override { return "O(n log n)"; }
    const char* getTimeComplexityWorst() const override { return "O(n log n)"; }
    const char* getTimeComplexityBest() const override { return "O(n log n)"; }
    const char* getSpaceComplexity() const override { return "O(n)"; }

    void sort(DynamicArray<int>& arr) const override;
    std::vector<SortingStep> generateSteps(const DynamicArray<int>& inputArr) const override;

private:
    void mergeSortInternal(DynamicArray<int>& arr, size_t left, size_t right) const;
    void mergeInternal(DynamicArray<int>& arr, size_t left, size_t mid, size_t right) const;

    void generateMergeSortSteps(DynamicArray<int>& arr, size_t left, size_t right, std::vector<SortingStep>& steps) const;
    void generateMergeSteps(DynamicArray<int>& arr, size_t left, size_t mid, size_t right, std::vector<SortingStep>& steps) const;
};

// 3. Genuine Quick Sort (Deterministic pivot strategy: last element)
class QuickSort : public SortingAlgorithm {
public:
    const char* getName() const override { return "Quick Sort"; }
    const char* getDescription() const override { return "Select a pivot element, partition array around pivot, and recursively sort subarrays."; }
    const char* getTimeComplexityAvg() const override { return "O(n log n)"; }
    const char* getTimeComplexityWorst() const override { return "O(n²) [When array is already sorted and last element chosen]"; }
    const char* getTimeComplexityBest() const override { return "O(n log n)"; }
    const char* getSpaceComplexity() const override { return "O(log n)"; }

    void sort(DynamicArray<int>& arr) const override;
    std::vector<SortingStep> generateSteps(const DynamicArray<int>& inputArr) const override;

private:
    void quickSortInternal(DynamicArray<int>& arr, int low, int high) const;
    int partitionInternal(DynamicArray<int>& arr, int low, int high) const;

    void generateQuickSortSteps(DynamicArray<int>& arr, int low, int high, std::vector<SortingStep>& steps) const;
    int generatePartitionSteps(DynamicArray<int>& arr, int low, int high, std::vector<SortingStep>& steps) const;
};
