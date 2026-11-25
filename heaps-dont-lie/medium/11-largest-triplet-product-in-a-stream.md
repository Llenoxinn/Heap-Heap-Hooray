# 11. Largest Triplet Product in a Stream

- **Difficulty:** Medium
- **Reference:** [GeeksforGeeks — Largest triplet product in a stream](https://www.geeksforgeeks.org/dsa/largest-triplet-product-stream/)

---

## Problem Statement

Given a stream of non-negative integers `stream[]`, after reading each element `stream[i]`, find the **product of the 3 largest distinct-index elements** seen from index `0` to `i`. If fewer than 3 elements have been read so far, output `-1`.

---

## Pattern Recognition & Intuition

### Why think "Heap" here?
At every step in a stream, we want the product of the top 3 largest non-negative numbers seen so far.
- **Naive approach:** Store everything seen so far and re-sort on every insertion -> $O(N^2 \log N)$ time and $O(N)$ space.
- **Heap approach:** This is a specialized `K = 3` case of the **Streaming Top-K** pattern! All we ever need to keep in memory are the **3 largest numbers** seen so far.

### Min-Heap vs Max-Heap Decision Rule
There are two classic ways to solve this with a heap:
1. **Max-Heap of all elements:** Push `x`, pop top 3 (`a, b, c`), multiply, push `a, b, c` back. This takes $O(N \log N)$ time and $O(N)$ space.
2. **Min-Heap of size at most `3` (Optimal):** Keep only the 3 largest numbers in a Min-Heap!
   - When a new element `x` arrives and the heap already has 3 items, compare `x` with `minHeap.getMin()` (the smallest of the top 3).
   - If `x > minHeap.getMin()`, evict the minimum and insert `x`.
   - Since the heap size never exceeds `3`, every heap operation takes $O(\log 3) = O(1)$ time and $O(1)$ space!

### Key Invariants & Edge Cases
- **Size Invariant:** `topThree.getSize() <= 3` at all times.
- **Overflow Guard:** Three `int` values up to $10^5$ can produce $10^{15}$, which overflows a 32-bit `int`. Always multiply in `long long` (`1LL * a * b * c`).
- **What if negative numbers were allowed?** For non-negative streams (as in the classic problem), `max1 * max2 * max3` is always optimal. If negative numbers can appear, `min1 * min2 * max1` (two most-negative values times the largest positive value) could be larger, so you would also maintain a size-2 Max-Heap for the 2 smallest elements.

---

## Step-by-Step Trace

Using **Test 1** from `main()`:
- `stream = [1, 2, 3, 4, 5]`

| Step | Incoming `val` | Action | `topThree` Min-Heap (`keyAt(1..size)`) | Triplet Product |
| :---: | :---: | :--- | :--- | :---: |
| 1 | `1` | `size < 3` -> insert `1` | `[1]` | `-1` |
| 2 | `2` | `size < 3` -> insert `2` | `[1, 2]` | `-1` |
| 3 | `3` | `size < 3` -> insert `3` | `[1, 2, 3]` | `1 * 2 * 3 = 6` |
| 4 | `4` | `4 > min(1)` -> pop `1`, push `4` | `[2, 4, 3]` | `2 * 4 * 3 = 24` |
| 5 | `5` | `5 > min(2)` -> pop `2`, push `5` | `[3, 4, 5]` | `3 * 4 * 5 = 60` |

**Final Output:** `[-1, -1, 6, 24, 60]`

---

## Time & Space Complexity

- **Time Complexity:** $O(N)$ overall ($O(\log 3) = O(1)$ per incoming element).
- **Space Complexity:** $O(1)$ auxiliary space (the Min-Heap holds at most 3 elements, excluding the output array).

---

## C++ Implementation

Source file: [11-largest-triplet-product-in-a-stream.cpp](11-largest-triplet-product-in-a-stream.cpp)

```cpp
#include <iostream>
#include <vector>
using namespace std;

class MinHeap {
private:
    vector<int> items;
    int size;

    void heapifyUp(int curr) {
        int x = items[curr];
        while (curr / 2 > 0 && x < items[curr / 2]) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void minHeapify(int curr) {
        int x = items[curr];
        while (curr * 2 <= size) {
            int child = curr * 2;
            if (child + 1 <= size && items[child + 1] < items[child]) {
                child++;
            }
            if (items[child] < x) {
                items[curr] = items[child];
                curr = child;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MinHeap() {
        items.push_back(0);
        size = 0;
    }

    void insert(int x) {
        size++;
        if (size < (int)items.size()) {
            items[size] = x;
        } else {
            items.push_back(x);
        }
        heapifyUp(size);
    }

    int getMin() {
        if (size == 0) return -1;
        return items[1];
    }

    int extractMin() {
        if (size == 0) return -1;
        int minVal = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            minHeapify(1);
        }
        return minVal;
    }

    int keyAt(int idx) {
        return items[idx];
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
};

vector<long long> largestTripletProductStream(vector<int>& stream) {
    MinHeap topThree;
    vector<long long> result;

    for (int i = 0; i < (int)stream.size(); i++) {
        int val = stream[i];
        if (topThree.getSize() < 3) {
            topThree.insert(val);
        } else if (val > topThree.getMin()) {
            topThree.extractMin();
            topThree.insert(val);
        }

        if (topThree.getSize() < 3) {
            result.push_back(-1);
        } else {
            long long prod = 1LL * topThree.keyAt(1) * topThree.keyAt(2) * topThree.keyAt(3);
            result.push_back(prod);
        }
    }

    return result;
}

void printVector(vector<long long>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        if (i > 0) cout << ", ";
        cout << arr[i];
    }
    cout << "]";
}

int main() {
    vector<int> s1 = {1, 2, 3, 4, 5};
    vector<long long> res1 = largestTripletProductStream(s1);
    cout << "Test 1: ";
    printVector(res1);
    cout << "\n";

    vector<int> s2 = {10, 3, 5, 6, 20};
    vector<long long> res2 = largestTripletProductStream(s2);
    cout << "Test 2: ";
    printVector(res2);
    cout << "\n";

    return 0;
}
```

---

## LeetCode Pattern Takeaways

This **"Fixed-Size K=3 Min-Heap"** directly relates to:
- **LeetCode 628** — *Maximum Product of Three Numbers* (same idea, plus tracking 2 minimums when negatives exist)
- **LeetCode 414** — *Third Maximum Number*
- **LeetCode 703** — *Kth Largest Element in a Stream*
- **LeetCode 1464** — *Maximum Product of Two Elements in an Array*
