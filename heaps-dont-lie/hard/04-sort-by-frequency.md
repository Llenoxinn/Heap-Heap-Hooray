# Sort by Frequency

- **Difficulty:** Hard
- **Reference:** [GeeksforGeeks — Sort Elements by Frequency](https://www.geeksforgeeks.org/dsa/sort-elements-by-frequency/)

---

## 1. Problem Statement

Given an integer array `arr`, sort its elements in **decreasing order of their frequency**. If two distinct elements appear the same number of times, the **smaller element must come first**.

---

## 2. Pattern Recognition & Intuition

### Why think "Heap" here?
Sorting by frequency is a two-stage pipeline:
1. **Aggregate counts** using a hash map (`val -> freq`).
2. **Order unique `(val, freq)` entries** by custom priority: primary key `freq` descending, secondary key `val` ascending.

Whenever you aggregate frequencies first and then need items pulled out in custom priority order (especially when tie-breakers or partial top-$k$ extraction are involved), **Hash Map + Custom Heap** is the canonical pattern.

### Min-Heap vs. Max-Heap Decision Rule
- We want the **highest frequency** elements output **first**, so we build a **Max-Heap** keyed by:
  1. Higher `freq` wins (`a.freq > b.freq`).
  2. On a frequency tie, **smaller `val` wins** (`a.val < b.val`).
- Notice how a custom comparator function `hasHigherPriority(a, b)` cleanly separates "what does higher priority mean?" from the mechanical `heapifyUp` / `maxHeapify` logic.

### Why this beats naive approaches
- **Checking frequencies per element during array sort:** Repeatedly scanning or looking up frequencies inside a naive $O(N^2)$ loop is slow.
- **Sorting all $N$ elements directly with map lookup:** Costs $O(N \log N)$ comparisons with a hash lookup on every single swap.
- **Hash Map + Max-Heap over $U$ unique values:** Heap size is $U \le N$, so heap operations cost $O(U \log U)$ — significantly faster when there are many duplicates ($U \ll N$).

### Key Invariants & Edge Cases
- **Uniqueness invariant:** Each unique element enters the `MaxHeap` **exactly once**, paired with its total count. When popped, we emit `top.val` `top.freq` times consecutively so identical elements are grouped together.
- **All unique elements (`U = N`):** Every frequency is `1`, so the tie-breaker (`a.val < b.val`) degrades gracefully to a normal ascending sort.
- **Single element or empty array:** Handled without special cases.
- **Negative numbers:** `unordered_map` supports arbitrary negative/large integers without needing fixed bucket offsets.

---

## 3. Step-by-Step Trace

Using **Test 1** from `main()`: `arr = [5, 5, 4, 6, 4]`

1. **Build `freqMap`:**
   - `5 -> 2`
   - `4 -> 2`
   - `6 -> 1`

2. **Insert unique `(val, freq)` items into `MaxHeap`:**
   - Insert `(5, 2)` $\rightarrow$ heap: `[(5, 2)]`
   - Insert `(4, 2)` $\rightarrow$ tie on `freq == 2`, `4 < 5`, so `(4, 2)` bubbles up $\rightarrow$ heap: `[(4, 2), (5, 2)]`
   - Insert `(6, 1)` $\rightarrow$ `freq 1 < 2`, stays at leaf $\rightarrow$ heap: `[(4, 2), (5, 2), (6, 1)]`

3. **Extract from `MaxHeap` until empty:**

| Step | `extractMax()` | Why it won root | Append to `result` | Remaining Heap |
| :--- | :--- | :--- | :--- | :--- |
| 1 | `(4, 2)` | Highest `freq=2`, smaller `val` than `5` | `4, 4` | `[(5, 2), (6, 1)]` |
| 2 | `(5, 2)` | Highest remaining `freq=2` | `5, 5` | `[(6, 1)]` |
| 3 | `(6, 1)` | Only remaining entry `freq=1` | `6` | `[]` |

Final Output: `[4, 4, 5, 5, 6]`.

---

## 4. Time & Space Complexity

- **Time Complexity:** $O(N + U \log U)$, where $U$ is the number of distinct elements ($U \le N$).
  - Building `freqMap`: $O(N)$ average.
  - Inserting $U$ items and extracting $U$ items from `MaxHeap`: $O(U \log U)$.
  - Writing $N$ elements to `result`: $O(N)$.
- **Space Complexity:** $O(N)$
  - `freqMap` and `MaxHeap` take $O(U)$ space; the output array takes $O(N)$ space.

---

## 5. C++ Implementation

See [04-sort-by-frequency.cpp](04-sort-by-frequency.cpp).

```cpp
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

struct FreqItem {
    int val;
    int freq;
};

bool hasHigherPriority(FreqItem a, FreqItem b) {
    if (a.freq != b.freq) {
        return a.freq > b.freq; // Higher frequency first
    }
    return a.val < b.val; // Tie-breaker: smaller value first
}

class MaxHeap {
private:
    vector<FreqItem> items;
    int size;

    void heapifyUp(int curr) {
        FreqItem x = items[curr];
        while (curr / 2 > 0 && hasHigherPriority(x, items[curr / 2])) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void maxHeapify(int curr) {
        FreqItem x = items[curr];
        while (curr * 2 <= size) {
            int best = curr * 2;
            if (best + 1 <= size && hasHigherPriority(items[best + 1], items[best])) {
                best++;
            }
            if (hasHigherPriority(items[best], x)) {
                items[curr] = items[best];
                curr = best;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MaxHeap() {
        items.push_back({0, 0});
        size = 0;
    }

    bool insert(FreqItem item) {
        if (size + 1 < (int)items.size()) {
            items[++size] = item;
        } else {
            items.push_back(item);
            size++;
        }
        heapifyUp(size);
        return true;
    }

    FreqItem getMax() {
        if (size == 0) {
            return {0, 0};
        }
        return items[1];
    }

    FreqItem extractMax() {
        if (size == 0) {
            return {0, 0};
        }
        FreqItem top = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            maxHeapify(1);
        }
        return top;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
};

vector<int> sortByFrequency(vector<int>& arr) {
    unordered_map<int, int> freqMap;
    for (int i = 0; i < (int)arr.size(); i++) {
        freqMap[arr[i]]++;
    }

    MaxHeap heap;
    for (auto it = freqMap.begin(); it != freqMap.end(); it++) {
        heap.insert({it->first, it->second});
    }

    vector<int> result;
    while (!heap.isEmpty()) {
        FreqItem top = heap.extractMax();
        for (int c = 0; c < top.freq; c++) {
            result.push_back(top.val);
        }
    }

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
    cout << "]\n";
}

int main() {
    vector<int> arr1 = {5, 5, 4, 6, 4};
    vector<int> res1 = sortByFrequency(arr1);
    cout << "Test 1 ([5, 5, 4, 6, 4]): ";
    printArray(res1);

    vector<int> arr2 = {9, 9, 9, 2, 5};
    vector<int> res2 = sortByFrequency(arr2);
    cout << "Test 2 ([9, 9, 9, 2, 5]): ";
    printArray(res2);

    vector<int> arr3 = {2, 5, 2, 8, 5, 6, 8, 8};
    vector<int> res3 = sortByFrequency(arr3);
    cout << "Test 3 ([2, 5, 2, 8, 5, 6, 8, 8]): ";
    printArray(res3);

    return 0;
}
```

---

## 6. LeetCode Pattern Takeaways

The **Frequency Map + Custom-Comparator Heap** pattern solves:
- **LC 451 — Sort Characters By Frequency** (exact same pattern on `char`)
- **LC 347 — Top K Frequent Elements** (Min-Heap of size $k$ over `freqMap`)
- **LC 692 — Top K Frequent Words** (Min-Heap of size $k$ with lexicographical tie-breaker)
- **LC 1636 — Sort Array by Increasing Frequency** (Min-Heap by `freq`, tie-break larger `val` first)
- **LC 767 — Reorganize String** (Max-Heap by remaining frequency, alternating top two characters)
