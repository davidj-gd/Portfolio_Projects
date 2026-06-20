#include <iostream>
#include <string>
#include <vector>
#include <cmath>

void check(const std::string& label, const std::string& expected, const std::string& answer)
{
    bool correct = (answer == expected);
    std::cout << (correct ? "[PASS] " : "[FAIL] ")
        << label
        << "  ->  your answer: O(" << answer << ")"
        << (correct ? "" : "  expected: O(" + expected + ")")
        << "\n";
}

int funcA(const std::vector<int>& arr)
{
    int sum = 0;
    for (int x : arr)
        sum += x;
    return sum;
}

bool funcB(const std::vector<int>& arr)
{
    for (int i = 0; i < (int)arr.size(); i++)
        for (int j = i + 1; j < (int)arr.size(); j++)
            if (arr[i] == arr[j]) return true;
    return false;
}

int funcC(const std::vector<int>& sorted, int target)
{
    int low = 0, high = (int)sorted.size() - 1;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        if (sorted[mid] == target) return mid;
        else if (sorted[mid] < target) low = mid + 1;
        else                            high = mid - 1;
    }
    return -1;
}

void funcD(int n)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                (void)(i + j + k);
}

void funcE(const std::vector<int>& arr)
{
    for (int i = 0; i < (int)arr.size(); i++)
        for (int j = 0; j < 100; j++)
            (void)(arr[i] + j);
}

void day01_exercise1()
{
    std::cout << "=== Day 01 Exercise 1 - Complexity analysis ===\n";

    // Replace "?" with your answer. Valid answers: 1, log n, n, n log n, n^2, n^3
    check("funcA - single loop over array", "n", "?");
    check("funcB - nested loop, j starts at i+1", "n^2", "?");
    check("funcC - binary search", "log n", "?");
    check("funcD - triple nested loop", "n^3", "?");
    check("funcE - outer n, inner always 100", "n", "?");

    std::cout << "\n";

    // For funcC on a sorted array of 1024 elements: log2(1024) = 10
    int maxIterations1024 = 0;  // fill this in
    std::cout << "funcC on 1024 elements, max iterations: " << maxIterations1024
        << "  (expected: " << (int)std::ceil(std::log2(1024)) << ")\n";

    // For funcC on a sorted array of 1048576 elements: 1048576 = 2^20, so log2 = 20
    int maxIterations1M = 0;    // fill this in
    std::cout << "funcC on 1048576 elements, max iterations: " << maxIterations1M
        << "  (expected: " << (int)std::ceil(std::log2(1048576)) << ")\n\n";
}