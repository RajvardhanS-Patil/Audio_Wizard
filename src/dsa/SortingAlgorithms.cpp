#include "SortingAlgorithms.hpp"
#include <algorithm>

// ==========================================
// 1. BUBBLE SORT IMPLEMENTATION
// ==========================================
void BubbleSort::sort(DynamicArray<int>& arr) const {
    size_t n = arr.size();
    if (n <= 1) return;

    for (size_t i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                arr.swap(j, j + 1);
                swapped = true;
            }
        }
        if (!swapped) break; // Early termination if already sorted
    }
}

std::vector<SortingStep> BubbleSort::generateSteps(const DynamicArray<int>& inputArr) const {
    std::vector<SortingStep> steps;
    DynamicArray<int> arr = inputArr;
    size_t n = arr.size();

    if (n <= 1) {
        SortingStep step;
        step.type = SortingStepType::SORTED;
        step.description = "Array is already sorted.";
        step.snapshot = arr.toVector();
        steps.push_back(step);
        return steps;
    }

    for (size_t i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (size_t j = 0; j < n - i - 1; ++j) {
            // Compare step
            SortingStep compStep;
            compStep.type = SortingStepType::COMPARE;
            compStep.index1 = j;
            compStep.index2 = j + 1;
            compStep.description = "Comparing elements at [" + std::to_string(j) + "] (" + std::to_string(arr[j]) +
                                  ") and [" + std::to_string(j + 1) + "] (" + std::to_string(arr[j + 1]) + ")";
            compStep.snapshot = arr.toVector();
            steps.push_back(compStep);

            if (arr[j] > arr[j + 1]) {
                arr.swap(j, j + 1);
                swapped = true;

                SortingStep swapStep;
                swapStep.type = SortingStepType::SWAP;
                swapStep.index1 = j;
                swapStep.index2 = j + 1;
                swapStep.description = "Swapped " + std::to_string(arr[j + 1]) + " and " + std::to_string(arr[j]);
                swapStep.snapshot = arr.toVector();
                steps.push_back(swapStep);
            } else {
                SortingStep noSwapStep;
                noSwapStep.type = SortingStepType::NO_SWAP;
                noSwapStep.index1 = j;
                noSwapStep.index2 = j + 1;
                noSwapStep.description = "In order (" + std::to_string(arr[j]) + " <= " + std::to_string(arr[j + 1]) + "), no swap needed.";
                noSwapStep.snapshot = arr.toVector();
                steps.push_back(noSwapStep);
            }
        }

        SortingStep passStep;
        passStep.type = SortingStepType::PASS_COMPLETE;
        passStep.description = "Pass " + std::to_string(i + 1) + " complete. Element at end is sorted.";
        passStep.snapshot = arr.toVector();
        steps.push_back(passStep);

        if (!swapped) break;
    }

    SortingStep finalStep;
    finalStep.type = SortingStepType::SORTED;
    finalStep.description = "Bubble Sort Complete! Array fully ordered.";
    finalStep.snapshot = arr.toVector();
    steps.push_back(finalStep);

    return steps;
}

// ==========================================
// 2. MERGE SORT IMPLEMENTATION
// ==========================================
void MergeSort::sort(DynamicArray<int>& arr) const {
    if (arr.size() <= 1) return;
    mergeSortInternal(arr, 0, arr.size() - 1);
}

void MergeSort::mergeSortInternal(DynamicArray<int>& arr, size_t left, size_t right) const {
    if (left >= right) return;
    size_t mid = left + (right - left) / 2;
    mergeSortInternal(arr, left, mid);
    mergeSortInternal(arr, mid + 1, right);
    mergeInternal(arr, left, mid, right);
}

void MergeSort::mergeInternal(DynamicArray<int>& arr, size_t left, size_t mid, size_t right) const {
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;

    std::vector<int> L(n1), R(n2);
    for (size_t i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (size_t j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    size_t i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr.update(k++, L[i++]);
        } else {
            arr.update(k++, R[j++]);
        }
    }
    while (i < n1) arr.update(k++, L[i++]);
    while (j < n2) arr.update(k++, R[j++]);
}

std::vector<SortingStep> MergeSort::generateSteps(const DynamicArray<int>& inputArr) const {
    std::vector<SortingStep> steps;
    DynamicArray<int> arr = inputArr;
    if (arr.size() > 1) {
        generateMergeSortSteps(arr, 0, arr.size() - 1, steps);
    }
    SortingStep finalStep;
    finalStep.type = SortingStepType::SORTED;
    finalStep.description = "Merge Sort Complete! Array merged and ordered.";
    finalStep.snapshot = arr.toVector();
    steps.push_back(finalStep);
    return steps;
}

void MergeSort::generateMergeSortSteps(DynamicArray<int>& arr, size_t left, size_t right, std::vector<SortingStep>& steps) const {
    if (left >= right) return;

    size_t mid = left + (right - left) / 2;

    SortingStep splitStep;
    splitStep.type = SortingStepType::SPLIT;
    splitStep.rangeStart = left;
    splitStep.rangeEnd = right;
    splitStep.description = "Split subarray [" + std::to_string(left) + ".." + std::to_string(right) + "] into [" +
                             std::to_string(left) + ".." + std::to_string(mid) + "] and [" + std::to_string(mid + 1) + ".." + std::to_string(right) + "]";
    splitStep.snapshot = arr.toVector();
    steps.push_back(splitStep);

    generateMergeSortSteps(arr, left, mid, steps);
    generateMergeSortSteps(arr, mid + 1, right, steps);
    generateMergeSteps(arr, left, mid, right, steps);
}

void MergeSort::generateMergeSteps(DynamicArray<int>& arr, size_t left, size_t mid, size_t right, std::vector<SortingStep>& steps) const {
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;

    std::vector<int> L(n1), R(n2);
    for (size_t i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (size_t j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    size_t i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        SortingStep compStep;
        compStep.type = SortingStepType::COMPARE;
        compStep.index1 = left + i;
        compStep.index2 = mid + 1 + j;
        compStep.rangeStart = left;
        compStep.rangeEnd = right;
        compStep.description = "Comparing left " + std::to_string(L[i]) + " and right " + std::to_string(R[j]);
        compStep.snapshot = arr.toVector();
        steps.push_back(compStep);

        if (L[i] <= R[j]) {
            arr.update(k++, L[i++]);
        } else {
            arr.update(k++, R[j++]);
        }
    }
    while (i < n1) arr.update(k++, L[i++]);
    while (j < n2) arr.update(k++, R[j++]);

    SortingStep mergeStep;
    mergeStep.type = SortingStepType::MERGE;
    mergeStep.rangeStart = left;
    mergeStep.rangeEnd = right;
    mergeStep.description = "Merged subarray [" + std::to_string(left) + ".." + std::to_string(right) + "] in sorted order.";
    mergeStep.snapshot = arr.toVector();
    steps.push_back(mergeStep);
}

// ==========================================
// 3. QUICK SORT IMPLEMENTATION
// (Deterministic pivot strategy: last element arr[high])
// ==========================================
void QuickSort::sort(DynamicArray<int>& arr) const {
    if (arr.size() <= 1) return;
    quickSortInternal(arr, 0, static_cast<int>(arr.size()) - 1);
}

void QuickSort::quickSortInternal(DynamicArray<int>& arr, int low, int high) const {
    if (low < high) {
        int pi = partitionInternal(arr, low, high);
        quickSortInternal(arr, low, pi - 1);
        quickSortInternal(arr, pi + 1, high);
    }
}

int QuickSort::partitionInternal(DynamicArray<int>& arr, int low, int high) const {
    int pivot = arr[high]; // Deterministic pivot strategy: last element
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            i++;
            arr.swap(i, j);
        }
    }
    arr.swap(i + 1, high);
    return i + 1;
}

std::vector<SortingStep> QuickSort::generateSteps(const DynamicArray<int>& inputArr) const {
    std::vector<SortingStep> steps;
    DynamicArray<int> arr = inputArr;
    if (arr.size() > 1) {
        generateQuickSortSteps(arr, 0, static_cast<int>(arr.size()) - 1, steps);
    }
    SortingStep finalStep;
    finalStep.type = SortingStepType::SORTED;
    finalStep.description = "Quick Sort Complete! Pivot partitioning finished.";
    finalStep.snapshot = arr.toVector();
    steps.push_back(finalStep);
    return steps;
}

void QuickSort::generateQuickSortSteps(DynamicArray<int>& arr, int low, int high, std::vector<SortingStep>& steps) const {
    if (low < high) {
        int pi = generatePartitionSteps(arr, low, high, steps);
        generateQuickSortSteps(arr, low, pi - 1, steps);
        generateQuickSortSteps(arr, pi + 1, high, steps);
    }
}

int QuickSort::generatePartitionSteps(DynamicArray<int>& arr, int low, int high, std::vector<SortingStep>& steps) const {
    int pivot = arr[high];
    size_t pivotIdx = static_cast<size_t>(high);

    SortingStep pivotStep;
    pivotStep.type = SortingStepType::PIVOT_SET;
    pivotStep.pivotIndex = pivotIdx;
    pivotStep.rangeStart = low;
    pivotStep.rangeEnd = high;
    pivotStep.description = "Chosen pivot: " + std::to_string(pivot) + " at index [" + std::to_string(high) + "]";
    pivotStep.snapshot = arr.toVector();
    steps.push_back(pivotStep);

    int i = low - 1;
    for (int j = low; j < high; ++j) {
        SortingStep compStep;
        compStep.type = SortingStepType::COMPARE;
        compStep.index1 = j;
        compStep.pivotIndex = pivotIdx;
        compStep.rangeStart = low;
        compStep.rangeEnd = high;
        compStep.description = "Comparing element " + std::to_string(arr[j]) + " with pivot " + std::to_string(pivot);
        compStep.snapshot = arr.toVector();
        steps.push_back(compStep);

        if (arr[j] < pivot) {
            i++;
            if (i != j) {
                arr.swap(i, j);
                SortingStep swapStep;
                swapStep.type = SortingStepType::SWAP;
                swapStep.index1 = i;
                swapStep.index2 = j;
                swapStep.pivotIndex = pivotIdx;
                swapStep.rangeStart = low;
                swapStep.rangeEnd = high;
                swapStep.description = "Moved smaller element " + std::to_string(arr[i]) + " to left partition";
                swapStep.snapshot = arr.toVector();
                steps.push_back(swapStep);
            }
        }
    }

    arr.swap(i + 1, high);
    SortingStep pivotPlaceStep;
    pivotPlaceStep.type = SortingStepType::SWAP;
    pivotPlaceStep.index1 = i + 1;
    pivotPlaceStep.index2 = high;
    pivotPlaceStep.pivotIndex = i + 1;
    pivotPlaceStep.rangeStart = low;
    pivotPlaceStep.rangeEnd = high;
    pivotPlaceStep.description = "Placed pivot " + std::to_string(pivot) + " at correct position index [" + std::to_string(i + 1) + "]";
    pivotPlaceStep.snapshot = arr.toVector();
    steps.push_back(pivotPlaceStep);

    return i + 1;
}
