# 10. K'th Largest Element in a Stream

- **Difficulty:** Medium
- **Reference:** [GeeksforGeeks — K'th largest element in a stream](https://www.geeksforgeeks.org/dsa/kth-largest-element-in-a-stream/)

---

## Problem Statement

Given an infinite stream of integers and an integer `k`, after each new integer arrives from the stream, report the **k'th largest element** among all elements seen so far. If fewer than `k` elements have been seen, output `-1`.

---

## Pattern Recognition & Intuition

### Why think "Heap" here?
In a **streaming** problem, you cannot re-sort all `N` elements seen so far on every arrival ($O(N \log N)$ per item -> $O(N^2 \log N)$ total), nor should you store all `N` elements in memory when `N` can grow indefinitely.

Notice that you **never** care about elements smaller than the current k'th largest — they can never become the k'th largest as larger elements arrive! You only need to remember the **top `k` largest elements** seen so far.

### Min-Heap vs Max-Heap Decision Rule
This is the classic **"counter-intuitive heap"** rule that every LeetCode candidate must internalize:
- Want the **K'th LARGEST** -> Use a **MIN-HEAP** of size `K`!
- Why? Among the top `K` largest elements, the **k'th largest overall** is the **smallest of those top `K`**. A Min-Heap puts that exact boundary element at the root (`O(1)` lookup via `getMin()`), and lets you evict it in `O(log K)` whenever a strictly larger element arrives!
- If you used a Max-Heap instead, you'd have to store all `N` elements and pop `K` times per query (`O(K log N)`).

### Key Invariants & Edge Cases
- **Size Invariant:** The Min-Heap size is always `<= k`.
- **Gatekeeper Rule:** When `heap.getSize() == k` and a new element `val` arrives:
  - If `val <= heap.getMin()`, ignore `val` (it cannot enter the top `k`).
  - If `val > heap.getMin()`, `extractMin()` and `insert(val)`.
- **Edge Cases:** Fewer than `k` elements seen (`return -1`), duplicate values (a duplicate equal to `getMin()` does not displace `getMin()`, while a duplicate strictly larger than `getMin()` does enter and push the old minimum out).

---

## Step-by-Step Trace

Using **Test 1** from `main()`:
- `k = 4`, `stream = [1, 2, 3, 4, 5, 6]`

| Step | Incoming `val` | Action | Min-Heap State (root first) | Output (`k`-th largest) |
| :--- | :---: | :--- | :--- | :---: |
| 1 | `1` | `size < 4` -> insert `1` | `[1]` | `-1` |
| 2 | `2` | `size < 4` -> insert `2` | `[1, 2]` | `-1` |
| 3 | `3` | `size < 4` -> insert `3` | `[1, 2, 3]` | `-1` |
| 4 | `4` | `size < 4` -> insert `4` | `[1, 2, 3, 4]` | `1` |
| 5 | `5` | `5 > min(1)` -> pop `1`, push `5` | `[2, 4, 3, 5]` | `2` |
| 6 | `6` | `6 > min(2)` -> pop `2`, push `6` | `[3, 4, 6, 5]` | `3` |

**Final Output:** `[-1, -1, -1, 1, 2, 3]`

---

## Time & Space Complexity

- **Time Complexity:** $O(\log K)$ per `add(val)` call, $O(N \log K)$ total for a stream of `N` elements.
  - At most one `extractMin()` and one `insert()` on a heap of size at most `K`.
- **Space Complexity:** $O(K)$ auxiliary space for the Min-Heap (independent of stream length `N`!).

---

## C++ Implementation

Source file: [10-kth-largest-in-a-stream.cpp](10-kth-largest-in-a-stream.cpp)

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

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
};

class KthLargestStream {
private:
    MinHeap minHeap;
    int k;

public:
    KthLargestStream(int kVal) {
        k = kVal;
    }

    int add(int val) {
        if (k <= 0) return -1;
        if (minHeap.getSize() < k) {
            minHeap.insert(val);
        } else if (val > minHeap.getMin()) {
            minHeap.extractMin();
            minHeap.insert(val);
        }

        if (minHeap.getSize() < k) {
            return -1;
        }
        return minHeap.getMin();
    }
};

vector<int> kthLargestInStream(vector<int>& stream, int k) {
    KthLargestStream tracker(k);
    vector<int> result;
    for (int i = 0; i < (int)stream.size(); i++) {
        result.push_back(tracker.add(stream[i]));
    }
    return result;
}

void printVector(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        if (i > 0) cout << ", ";
        cout << arr[i];
    }
    cout << "]";
}

int main() {
    vector<int> s1 = {1, 2, 3, 4, 5, 6};
    int k1 = 4;
    vector<int> res1 = kthLargestInStream(s1, k1);
    cout << "Test 1 (k = " << k1 << "): ";
    printVector(res1);
    cout << "\n";

    vector<int> s2 = {10, 20, 11, 70, 50, 40, 100, 5};
    int k2 = 3;
    vector<int> res2 = kthLargestInStream(s2, k2);
    cout << "Test 2 (k = " << k2 << "): ";
    printVector(res2);
    cout << "\n";

    return 0;
}
```

---

## LeetCode Pattern Takeaways

This **"Fixed-Size Min-Heap of Size K for Streaming Top-K"** is one of the highest-ROI interview patterns:
- **LeetCode 703** — *Kth Largest Element in a Stream* (exact match!)
- **LeetCode 215** — *Kth Largest Element in an Array* (heap approach)
- **LeetCode 295** — *Find Median from Data Stream* (combines a Max-Heap and Min-Heap for dynamic `k = n/2`)
- **LeetCode 1985** — *Find the Kth Largest Integer in the Array*
