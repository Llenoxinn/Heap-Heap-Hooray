# k Closest Elements

- **Difficulty:** Medium
- **Reference:** [GeeksforGeeks — Find k Closest Elements to a Given Value](https://www.geeksforgeeks.org/dsa/find-k-closest-elements-given-value/)

---

## Problem Statement

Given an integer array `arr`, an integer `k`, and a target value `x`, find the `k` elements in `arr` that are closest to `x` (minimizing `|arr[i] - x|`). If two elements have the same distance to `x`, prefer the smaller value. Return the `k` closest elements in ascending order.

*(Note: Some GeeksforGeeks problem variants exclude `x` itself if `x` appears in `arr`; this implementation follows the standard distance definition matching LeetCode 658 where `|x - x| = 0` is valid. Skipping `arr[i] == x` is a one-line filter if needed.)*

**Example:**
- Input: `arr = [10, 2, 14, 4, 7, 6]`, `k = 3`, `x = 5`
- Output: `[4, 6, 7]`

---

## Pattern Recognition & Intuition

- **Why think "Heap"?**
  We want the **K Smallest** elements ranked not by raw value, but by a **custom distance key** `(|val - x|, val)`. Notice that `arr` does not even need to be sorted! Anytime a problem asks for the "K closest / smallest score" items, think **Top-K Heap**.
- **Min-Heap vs Max-Heap Decision Rule:**
  Following the Top-K opposite-heap rule, finding the `k` **smallest distances** uses a **Max-Heap** of size `k`!
  - Why? The root holds the **worst (farthest)** of the `k` candidates currently kept.
  - When a new element arrives with a strictly smaller distance (or equal distance and smaller value), it beats the worst element at the root, so we `extractMax()` and `insert(cand)`.
- **Why it beats naive approaches:**
  - Sorting the entire array by `(|a - x|, a)`: $O(n \log n)$ time.
  - Max-Heap of size `k`: $O(n \log k)$ time and $O(k)$ space — works on unsorted arrays without modifying the input. (Note: if `arr` is *already* guaranteed sorted, binary search + two pointers can reach $O(\log n + k)$, whereas the Max-Heap works for **both** sorted and unsorted inputs.)
- **Key Invariant:**
  After scanning `arr[0..i]` (with `i >= k - 1`), the Max-Heap contains the `k` closest elements seen so far, and `maxHeap.getMax()` is the farthest (and largest-valued on ties) among those `k`.
- **Edge Cases:**
  - `k <= 0` or empty array: return empty result.
  - `k >= n`: return all elements sorted.
  - Tie-breaking: `cand` is closer than `worst` iff `cand.diff < worst.diff || (cand.diff == worst.diff && cand.val < worst.val)`.
  - Negative values and target outside array bounds: `abs(arr[i] - x)` handles negative numbers naturally.

---

## Step-by-Step Trace

Tracing **Test 1** from `main()`: `arr = [10, 2, 14, 4, 7, 6]`, `k = 3`, `x = 5`.
Each heap item is `(diff = |val - 5|, val)`, ordered so the **largest `(diff, val)`** is at the root.

1. `val = 10` → `(5, 10)`: size `< 3` → `insert`.
   - Heap root: `(5, 10)` | Contents: `[(5, 10)]`
2. `val = 2` → `(3, 2)`: size `< 3` → `insert`.
   - Heap root: `(5, 10)` | Contents: `[(5, 10), (3, 2)]`
3. `val = 14` → `(9, 14)`: size `< 3` → `insert`.
   - Heap root: `(9, 14)` | Contents: `[(9, 14), (3, 2), (5, 10)]`
4. `val = 4` → `(1, 4)`: closer than root `(9, 14)` → `extractMax()`, `insert((1, 4))`.
   - Heap root: `(5, 10)` | Contents: `[(5, 10), (3, 2), (1, 4)]`
5. `val = 7` → `(2, 7)`: closer than root `(5, 10)` → `extractMax()`, `insert((2, 7))`.
   - Heap root: `(3, 2)` | Contents: `[(3, 2), (1, 4), (2, 7)]`
6. `val = 6` → `(1, 6)`: closer than root `(3, 2)` → `extractMax()`, `insert((1, 6))`.
   - Heap root: `(2, 7)` | Contents: `[(2, 7), (1, 4), (1, 6)]`

Drain values: `[7, 4, 6]` → sort ascending → **`[4, 6, 7]`**.

---

## Time & Space Complexity

- **Time Complexity:** $O(n \log k + k \log k) = O(n \log k)$
  - Each of the `n` elements costs at most $O(\log k)$ for heap eviction + insertion.
  - Extracting `k` elements and sorting them in ascending order takes $O(k \log k)$.
- **Space Complexity:** $O(k)$ for the Max-Heap and output list.

---

## C++ Implementation

See [03-k-closest-elements.cpp](03-k-closest-elements.cpp).

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;

struct Candidate {
    int diff;
    int val;
};

class MaxHeap {
private:
    vector<Candidate> items;
    int size;

    bool isWorse(Candidate& a, Candidate& b) {
        if (a.diff != b.diff) {
            return a.diff > b.diff;
        }
        return a.val > b.val;
    }

    void heapifyUp(int curr) {
        Candidate x = items[curr];
        while (curr / 2 > 0 && isWorse(x, items[curr / 2])) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void maxHeapify(int curr) {
        Candidate x = items[curr];
        while (curr * 2 <= size) {
            int s = curr * 2;
            if (s + 1 <= size && isWorse(items[s + 1], items[s])) {
                s++;
            }
            if (isWorse(items[s], x)) {
                items[curr] = items[s];
                curr = s;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MaxHeap() {
        Candidate dummy;
        dummy.diff = 0;
        dummy.val = 0;
        items.push_back(dummy);
        size = 0;
    }

    void insert(Candidate x) {
        items.push_back(x);
        size++;
        heapifyUp(size);
    }

    Candidate getMax() {
        if (size == 0) {
            Candidate empty;
            empty.diff = 0;
            empty.val = 0;
            return empty;
        }
        return items[1];
    }

    Candidate extractMax() {
        if (size == 0) {
            Candidate empty;
            empty.diff = 0;
            empty.val = 0;
            return empty;
        }
        Candidate maxVal = items[1];
        items[1] = items[size];
        items.pop_back();
        size--;
        if (size > 0) {
            maxHeapify(1);
        }
        return maxVal;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
};

vector<int> findKClosestElements(vector<int>& arr, int k, int x) {
    vector<int> result;
    if (k <= 0 || arr.empty()) {
        return result;
    }

    int limit = min(k, (int)arr.size());
    MaxHeap maxHeap;

    for (int i = 0; i < (int)arr.size(); i++) {
        Candidate cand;
        cand.diff = abs(arr[i] - x);
        cand.val = arr[i];

        if (maxHeap.getSize() < limit) {
            maxHeap.insert(cand);
        } else {
            Candidate worst = maxHeap.getMax();
            bool candIsCloser = (cand.diff < worst.diff) ||
                                (cand.diff == worst.diff && cand.val < worst.val);
            if (candIsCloser) {
                maxHeap.extractMax();
                maxHeap.insert(cand);
            }
        }
    }

    while (!maxHeap.isEmpty()) {
        result.push_back(maxHeap.extractMax().val);
    }
    sort(result.begin(), result.end());
    return result;
}

void printArray(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        cout << arr[i];
        if (i + 1 < (int)arr.size()) {
            cout << ", ";
        }
    }
    cout << "]";
}

void runTest(string label, vector<int> arr, int k, int x) {
    cout << label << " (k = " << k << ", x = " << x << ")\n";
    cout << "  Input  : ";
    printArray(arr);
    cout << "\n";
    vector<int> closest = findKClosestElements(arr, k, x);
    cout << "  Closest: ";
    printArray(closest);
    cout << "\n\n";
}

int main() {
    runTest("Test 1 (Primary Unsorted Example)", {10, 2, 14, 4, 7, 6}, 3, 5);
    runTest("Test 2 (Sorted Array With Ties)", {1, 2, 3, 4, 5}, 4, 3);
    runTest("Test 3 (Target Outside Range)", {12, 16, 22, 30, 39, 42, 45}, 3, 50);
    runTest("Test 4 (Negatives & Ties)", {-10, -4, 0, 4, 8, 12}, 3, 2);
    return 0;
}
```

---

## LeetCode Pattern Takeaways

Custom-comparator Top-K heaps let you rank elements by **any derived metric** without changing the core heap structure:
- **LeetCode 658 — Find K Closest Elements:** Direct counterpart (sorted input allows $O(\log n + k)$ binary search, while Max-Heap handles unsorted inputs in $O(n \log k)$).
- **LeetCode 973 — K Closest Points to Origin:** Identical Max-Heap of size `k` using `x^2 + y^2` as `diff`.
- **LeetCode 692 — Top K Frequent Words:** Min-Heap of size `k` with a tie-breaker on lexicographical order.
