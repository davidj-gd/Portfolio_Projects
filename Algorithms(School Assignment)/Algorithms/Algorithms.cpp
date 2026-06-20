// main.cpp
#include <vector>
#include <iostream>

struct SortResult
{
    std::vector<int> sorted;
    int comparisons;
    int swaps;
};

SortResult bubbleSortCounted(std::vector<int> arr)
{
    // TODO: implement bubble sort here.
    // Increment a comparison counter every time two elements are compared.
    // Increment a swap counter every time two elements are swapped.
    // Add a bool 'swapped' flag for early exit: if a full pass completes
    // with no swaps, the array is already sorted - stop early.

    int comparisons = 0;
    int swaps = 0;

    // your implementation here

    return { arr, comparisons, swaps };
}

void day01_exercise2()
{
    std::cout << "=== Day 01 Exercise 2 - Bubble sort trace ===\n";

    std::vector<int> arr1 = { 4, 2, 7, 1, 5 };
    SortResult r1 = bubbleSortCounted(arr1);

    std::cout << "Input:  4 2 7 1 5\n";
    std::cout << "Output: ";
    for (int x : r1.sorted) std::cout << x << " ";
    std::cout << "\n";
    std::cout << "Comparisons: " << r1.comparisons
        << "  (expected: " << (5 * 4 / 2) << " max)\n";
    std::cout << "Swaps:       " << r1.swaps << "\n\n";

    std::vector<int> arr30, arr60, arr120;
    for (int i = 30; i >= 1; i--) arr30.push_back(i);
    for (int i = 60; i >= 1; i--) arr60.push_back(i);
    for (int i = 120; i >= 1; i--) arr120.push_back(i);

    SortResult r30 = bubbleSortCounted(arr30);
    SortResult r60 = bubbleSortCounted(arr60);
    SortResult r120 = bubbleSortCounted(arr120);

    std::cout << "Scaling test (reversed arrays, worst case):\n";
    std::cout << "n=30:  " << r30.comparisons << " comparisons\n";
    std::cout << "n=60:  " << r60.comparisons << " comparisons  ratio vs n=30: "
        << (float)r60.comparisons / r30.comparisons << "\n";
    std::cout << "n=120: " << r120.comparisons << " comparisons  ratio vs n=60: "
        << (float)r120.comparisons / r60.comparisons << "\n";
    std::cout << "Expected ratio: ~4.0 (O(n^2) means doubling n quadruples work)\n\n";

    std::vector<int> sorted30;
    for (int i = 1; i <= 30; i++) sorted30.push_back(i);
    SortResult rSorted = bubbleSortCounted(sorted30);
    std::cout << "Early exit test (already sorted, n=30):\n";
    std::cout << "Comparisons: " << rSorted.comparisons
        << "  (with early exit: 29, without: " << (30 * 29 / 2) << ")\n\n";
}

//void check(const std::string& label, const std::string& expected, const std::string& answer)
//{
//    bool correct = (answer == expected);
//    std::cout << (correct ? "[PASS] " : "[FAIL] ")
//              << label
//              << "  ->  your answer: O(" << answer << ")"
//              << (correct ? "" : "  expected: O(" + expected + ")")
//              << "\n";
//}
//
//int funcA(const std::vector<int>& arr)
//{
//    int sum = 0;
//    for (int x : arr)
//        sum += x;
//    return sum;
//}
//
//bool funcB(const std::vector<int>& arr)
//{
//    for (int i = 0; i < (int)arr.size(); i++)
//        for (int j = i + 1; j < (int)arr.size(); j++)
//            if (arr[i] == arr[j]) return true;
//    return false;
//}
//
//int funcC(const std::vector<int>& sorted, int target)
//{
//    int low = 0, high = (int)sorted.size() - 1;
//    while (low <= high)
//    {
//        int mid = (low + high) / 2;
//        if      (sorted[mid] == target) return mid;
//        else if (sorted[mid] <  target) low  = mid + 1;
//        else                            high = mid - 1;
//    }
//    return -1;
//}
//
//void funcD(int n)
//{
//    for (int i = 0; i < n; i++)
//        for (int j = 0; j < n; j++)
//            for (int k = 0; k < n; k++)
//                (void)(i + j + k);
//}
//
//void funcE(const std::vector<int>& arr)
//{
//    for (int i = 0; i < (int)arr.size(); i++)
//        for (int j = 0; j < 100; j++)
//            (void)(arr[i] + j);
//}

//void day01_exercise1()
//{
//    std::cout << "=== Day 01 Exercise 1 - Complexity analysis ===\n";
//
//    check("funcA - single loop over array",       "n",     "n");
//    check("funcB - nested loop, j starts at i+1", "n^2",   "n^2");
//    check("funcC - binary search",                "log n", "log n");
//    check("funcD - triple nested loop",           "n^3",   "n^3");
//    check("funcE - outer n, inner always 100",    "n",     "n");
//
//    std::cout << "\n";
//
//    int maxIterations1024 = 10;
//    std::cout << "funcC on 1024 elements, max iterations: " << maxIterations1024
//              << "  (expected: " << (int)std::ceil(std::log2(1024)) << ")\n";
//
//    int maxIterations1M = 20;
//    std::cout << "funcC on 1048576 elements, max iterations: " << maxIterations1M
//              << "  (expected: " << (int)std::ceil(std::log2(1048576)) << ")\n\n";
//}

//void day01_exercise2();
//void day02_exercise1();

void day02_exercise2();
void day03_exercise1();
void day03_exercise2();
void day04_exercise1();
void day04_exercise2();
void day05_exercise1();
void day05_exercise2();
void day06_exercise1();
void day06_exercise2();
void day07_exercise1();
void day07_exercise2();
void day08_exercise1();
void day08_exercise2();
void day09_exercise1();

int main()
{
    // Uncomment each block as you progress through the course.

    // --- Day 01 ---
    /*day01_exercise1();*/
    day01_exercise2();

    // --- Day 02 ---
    // day02_exercise1();
    // day02_exercise2();

    // --- Day 03 ---
    // day03_exercise1();
    // day03_exercise2();

    // --- Day 04 ---
    // day04_exercise1();
    // day04_exercise2();

    // --- Day 05 ---
    // day05_exercise1();
    // day05_exercise2();

    // --- Day 06 ---
    // day06_exercise1();
    // day06_exercise2();

    // --- Day 07 ---
    // day07_exercise1();
    // day07_exercise2();

    // --- Day 08 ---
    // day08_exercise1();
    // day08_exercise2();

    // --- Day 09 ---
    // day09_exercise1();

    return 0;
}