#include <iostream>
#include <vector>
#include <deque>
#include <array>
#include <algorithm>
#include <numeric>
#include <string>

// Part (a): vector - store N integers and print in reverse
void part_a() {
    std::cout << "--- Part A: Vector (Reverse Output) ---\n";

    int n;
    std::cout << "Enter N: ";
    std::cin >> n;

    std::vector<int> nums(n);
    std::cout << "Enter " << n << " integers: ";
    for (int i = 0; i < n; i++) {
        std::cin >> nums[i];
    }

    // Using reverse iterators - clean and idiomatic STL
    std::cout << "Reversed: ";
    for (auto it = nums.rbegin(); it != nums.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n\n";
}

// Part (b): deque - user enters a sequence of operations
void part_b() {
    std::cout << "--- Part B: Deque (Operation Sequence) ---\n";
    std::cout << "Enter operations: PUSH_FRONT <val>, PUSH_BACK <val>, POP_FRONT, DONE\n";

    std::deque<int> dq;
    std::string op;

    while (std::cin >> op) {
        if (op == "PUSH_FRONT") {
            int val;
            std::cin >> val;
            dq.push_front(val);
            std::cout << "  push_front(" << val << ") done\n";
        } else if (op == "PUSH_BACK") {
            int val;
            std::cin >> val;
            dq.push_back(val);
            std::cout << "  push_back(" << val << ") done\n";
        } else if (op == "POP_FRONT") {
            if (!dq.empty()) {
                std::cout << "  pop_front() removed: " << dq.front() << "\n";
                dq.pop_front();
            } else {
                std::cout << "  pop_front() failed: deque is empty\n";
            }
        } else if (op == "DONE") {
            break;
        } else {
            std::cout << "  Unknown operation: " << op << "\n";
        }
    }

    std::cout << "\nFinal deque contents: ";
    if (dq.empty()) {
        std::cout << "(empty)";
    } else {
        for (const auto& val : dq) {
            std::cout << val << " ";
        }
    }
    std::cout << "\nSize: " << dq.size() << "\n\n";
}

// Part (c): std::array - fixed compile-time size, user provides the values
// The whole point of std::array vs vector is that size is a compile-time constant
void part_c() {
    std::cout << "--- Part C: std::array (Fixed-Size, User Input, Sum with STL) ---\n";

    const int SIZE = 5;
    std::array<int, SIZE> arr;

    std::cout << "Enter " << SIZE << " integers: ";
    for (int i = 0; i < SIZE; i++) {
        std::cin >> arr[i];
    }

    std::cout << "Array elements: ";
    for (const auto& x : arr) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // std::accumulate from <numeric> - idiomatic STL sum
    int total = std::accumulate(arr.begin(), arr.end(), 0);
    std::cout << "Sum (via std::accumulate): " << total << "\n";

    auto [mn, mx] = std::minmax_element(arr.begin(), arr.end());
    std::cout << "Min: " << *mn << ", Max: " << *mx << "\n";
}

int main() {
    part_a();
    part_b();
    part_c();
    return 0;
}